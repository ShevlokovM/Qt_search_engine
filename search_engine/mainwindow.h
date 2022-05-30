#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QTextBrowser>
#include <QString>
#include <QLabel>
#include <QDebug>
#include <QRegularExpression>

#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <exception>

#include "converterjson.h"
#include "searchserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

     std::shared_ptr<ConverterJSON> convertJson = nullptr;
     std::shared_ptr<SearchServer> searchServer = nullptr;
     std::shared_ptr<std::vector<std::string>> requests = nullptr;

public:
     QLabel* qLabel = nullptr;
     QTextEdit* textEdit = nullptr;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_initial();
    void check_filename(std::string filename);
    void check_keys(std::string key, std::string inpt);
    void check_file(std::string file);
    void check_max_resp(int max_resp);

private:
    Ui::MainWindow *ui;


public slots:

    void get_config()
    {
        qLabel->setText("get config");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        std::stringstream text_to_html;
        std::string html_string;

        std::shared_ptr<config_record> curr_config = convertJson->GetConfig();
        text_to_html << "name: " << curr_config->name << "<br>" << "version: " <<  curr_config->version << "<br>" << "max_responses: " << curr_config->max_responses << "<br>" << "files: " << "<br>";
        for(auto file_name : curr_config->files)
        {
            file_name.erase(0, 12);
            text_to_html << file_name << "<br>";
        }
        html_string = text_to_html.str();
        //text_to_html >> html_string;
        std::shared_ptr<QString> qString = std::make_shared<QString>(html_string.c_str());
        //qString->fromStdString(html_string);
        textEdit->clear();
        textEdit->document()->setHtml(*qString);
    }

    void set_config()
    {
       qLabel->setText("set config");
       qLabel->setStyleSheet("color: rgb(0, 0, 0)");

       std::stringstream text;
       std::string for_keys;
       std::shared_ptr<config_record> configRecord = std::make_shared<config_record>();
       QString from_screen = textEdit->document()->toPlainText();
       std::string str_from_screen = from_screen.toStdString();

       text << str_from_screen;

       // проверка ввода ключей файла конфигурации на релевантность
       for(int i = 0; i < 4; i++)
       {
           text >> for_keys;

           try
           {
               if(i == 0) check_keys("name:", for_keys);
               else if(i == 1) check_keys("version:", for_keys);
               else if(i == 2) check_keys("max_responses:", for_keys);
               else if(i == 3) check_keys("files:", for_keys);
           }
           catch (const std::exception& x) // если не соответствуют названия ключей
           {
               std::cerr << "Caught exception" << x.what() << std::endl;
               std::string massage = for_keys;
               massage += " - incorrect name of key!";
               get_config();
               qLabel->setText(QString(massage.c_str()));
               qLabel->setStyleSheet("color: rgb(255, 0, 0)");
               return;
           }           

           if(i == 0) text >> configRecord->name;
           else if(i == 1) text >> configRecord->version;
           else if(i == 2) text >> configRecord->max_responses;
       }

       try // проверка значения максимального кол-ва файлов в результатах запроса
       {
          check_max_resp(configRecord->max_responses);
       }
       catch (const std::invalid_argument& x) // если значение max_responses меньше 1
       {
           std::cerr << "Caught exception" << x.what() << std::endl;
           std::string massage = "max_responses";
           massage += " - incorrect value!";
           get_config();
           qLabel->setText(QString(massage.c_str()));
           qLabel->setStyleSheet("color: rgb(255, 0, 0)");
           return;
       }


       while(!text.eof()) // парсинг списка файлов
       {
           std::string file;
           text >> file;
           if(file == "") break;

           // проверка ввода имени файлов на релевантность
           try
           {
               check_filename(file);
           }
           catch (const std::exception& x)
           {
               std::cerr << "Caught exception" << x.what() << std::endl;
               std::string massage = file;
               massage += " - incorrect name of file!";
               get_config();

               qLabel->setText(QString(massage.c_str()));
               qLabel->setStyleSheet("color: rgb(255, 0, 0)");
               return;
           }

           // проверка наличия файлов в репозитории
           std::string file_path = ".\\resources\\";
           file_path += file;
           try
           {
               check_file(file_path);
           }
           catch (const std::exception& x)
           {
               std::cerr << "Caught exception" << x.what() << std::endl;
               std::string massage = file;
               massage += " - abscent on directory!";
               get_config();

               qLabel->setText(QString(massage.c_str()));
               qLabel->setStyleSheet("color: rgb(255, 0, 0)");
               return;
           }

           if(file_path != ".\\resources\\")
           configRecord->files.push_back((file_path));
       }

       // запись проверенных данных в файл конфигурации
       convertJson->SetConfig(configRecord);

       // запуск индексации базы файлов
       searchServer->GetInvertedIndex()->UpdateDocumentBase(convertJson->GetTextDocuments());
       textEdit->clear();
       textEdit->document()->setHtml(QString("Document base updated."));
    }

    void set_request()
    {
        qLabel->setText("set request");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        textEdit->clear();
    }

    void send_request()
    {
        qLabel->setText("request sent");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        QString screen_text = textEdit->toPlainText();
        requests->push_back(screen_text.toStdString());
    }

    void get_answer()
    {
        qLabel->setText("results");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        std::stringstream text_to_html;
        std::string html_string;

        convertJson->SetRequests(requests);
        convertJson->PutAnswers(searchServer->search(convertJson->GetRequests()));
        std::shared_ptr<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > > answers = convertJson->GetAnswers();


        text_to_html << "answers:" << "<br><br>";

        for(int i = 0; i < answers->size(); i++)
        {
            if(i < 9) text_to_html << ("request00" + std::to_string(i + 1));
            else if(i < 99) text_to_html <<  ("request0" + std::to_string(i + 1));
            else text_to_html <<  ("request" + std::to_string(i + 1));
            text_to_html << "<br>";

            if(answers->at(i).first == "true")
            {
                text_to_html << "result: "<< "true" << "<br>";
                for(auto pairr : answers->at(i).second)
                {
                    text_to_html << "docid: " << pairr.first << ", rank: " << pairr.second << "<br>";
                }
            }
            else
              text_to_html << "result: "<< "false" << "<br>";
            text_to_html << "<br>";
        }

        html_string = text_to_html.str();
        std::shared_ptr<QString> qString = std::make_shared<QString>(html_string.c_str());

        textEdit->clear();
        textEdit->document()->setHtml(*qString);

        requests->clear();
    }

};
#endif // MAINWINDOW_H
