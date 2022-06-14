#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

// библиотеки Qt
#include <QMainWindow>
#include <QTextBrowser>
#include <QString>
#include <QLabel>
#include <QDebug>
#include <QRegularExpression>

// библиотеки С++
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <exception>

// файлы проекта
#include "converterjson.h"
#include "searchserver.h"

//--------------------------------------------------------------------------------------

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 * Реализует окно приложения, и его взаимодействие с логикой программы
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

     std::shared_ptr<ConverterJSON> convertJson = nullptr;
     std::shared_ptr<SearchServer> searchServer = nullptr;
     std::shared_ptr<std::vector<std::string>> requests = nullptr;

public:
     QLabel* qLabel = nullptr;
     QTextEdit* textEdit = nullptr;


     // методы

     /**
     * @brief MainWindow конструктор
     * инициализирует std::shared_ptr членов класса и членов членов класса
     * @param parent указатель на объект родительского класса
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * деструктор
     */
    ~MainWindow();

    /**
     * @brief set_initial
     * проверяет готовность приложения к работе, наличие конфигурации, если нет, просит ввести
     * запускает индексацию файлов, если конфигурация есть
     * необходимо запускать после того, как класс связан с графическим интерфейсом
     * @return
     */
    void set_initial();

    /**
     * @brief check_filename
     * проверяет расширение файла. должно быть .txt
     * @param filename имя файла
     */
    void check_filename(std::string filename);

    /**
     * @brief check_keys
     * проверяет наличие в структуре конфигурации всех необходимых ключей
     * @param key ключ, который нужно проверить
     * @param inpt введенное пользователем имя ключа
     */
    void check_keys(std::string key, std::string inpt);

    /**
     * @brief check_file
     * проверяет наличие файла в папке resources
     * @param file путь к файлу
     */
    void check_file(std::string file);

    /**
     * @brief check_max_resp
     * проверяет что значение максимального кол-ва файлов в поисковом запросе
     * не меньше 0
     * @param max_resp значение максимального кол-ва файлов в поисковом запросе,
     * введенное пользователем
     */
    void check_max_resp(int max_resp);

    /**
     * @brief check_requests
     * проверка наличия запросов
     * @param requests вектор запросов
     */
    void check_requests(std::shared_ptr<std::vector<std::string>> requests);

private:
    Ui::MainWindow *ui;

public slots:

    /**
     * @brief get_config
     * слот кнопки GET CONFIG
     * выводит в рабочее окно приложения текущую конфигурацию
     * в случае ее отсутствия - шаблон конфигурации
     */
    void get_config()
    {
        // изменение информационного окна программы
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

        // вывод сообщения в рабочее окно
        html_string = text_to_html.str();
        textEdit->document()->setHtml(QString(html_string.c_str()));
    }

    /**
     * @brief set_config
     * слот кнопки SET CONFIG
     * передает новую, или обновляет старую конфигурацию, в соответствии с введенной
     * пользователем в рабочем окне программы
     * запускает индексацию файлов источников из папки resources
     */
    void set_config()
    {
        // изменение информационного окна программы
       qLabel->setText("set config");
       qLabel->setStyleSheet("color: rgb(0, 0, 0)");

       std::stringstream text;
       std::string for_keys;
       auto configRecord = std::make_shared<config_record>();

       // получение данных из рабочего окна приложения
       QString from_screen = textEdit->document()->toPlainText();
       std::string str_from_screen = from_screen.toStdString();

       text << str_from_screen;

       // проверка ввода ключей файла конфигурации на релевантность
       for(int i = 0; i < 4; i++)
       {
           text >> for_keys;

           try
           {
               // проверка имен ключей, введенных пользователем
               if(i == 0) check_keys("name:", for_keys);
               else if(i == 1) check_keys("version:", for_keys);
               else if(i == 2) check_keys("max_responses:", for_keys);
               else if(i == 3) check_keys("files:", for_keys);
           }
           catch (const std::exception& x) // если не соответствуют названия ключей
           {
               std::cerr << "Caught exception" << x.what() << std::endl;

               // формирование сообщения для информационного окна
               std::string massage = for_keys;
               massage += " - incorrect name of key!";

               // вывод в рабочее окно предыдущей конфигурации, или шаблона
               get_config();

               // изменение информационного окна программы
               qLabel->setText(QString(massage.c_str()));
               qLabel->setStyleSheet("color: rgb(255, 0, 0)");

               return;
           }

           // заполнение структуры конфигурации приложения configRecord
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

           // формирование сообщения для информационного окна
           std::string massage = "max_responses";
           massage += " - incorrect value!";

           // вывод в рабочее окно предыдущей конфигурации, или шаблона
           get_config();

           // изменение информационного окна программы
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

               // формирование сообщения для информационного окна
               std::string massage = file;
               massage += " - incorrect name of file!";

               // вывод в рабочее окно предыдущей конфигурации, или шаблона
               get_config();

               // изменение информационного окна программы
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

               // формирование сообщения для информационного окна
               std::string massage = file;
               massage += " - abscent on directory!";

               // вывод в рабочее окно предыдущей конфигурации, или шаблона
               get_config();

               // изменение информационного окна программы
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
       textEdit->document()->setHtml(QString("Document base updated."));
    }

    /**
     * @brief set_request
     * слот кнопки SET REQUEST
     * очищает рабочее окно для ввода запроса
     */
    void set_request()
    {
        // изменение информационного окна программы
        qLabel->setText("set request");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        // очистка рабочего окна программы
        textEdit->clear();
    }

    /**
     * @brief send_request
     * слот кнопки SEND REQUEST
     * отправляет введенный пользователем запрос в массив запросов requests
     */
    void send_request()
    {
        // изменение информационного окна программы
        qLabel->setText("request sent");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        // сохранение запроса в векторе запросов
        QString screen_text = textEdit->toPlainText();
        requests->push_back(screen_text.toStdString());
    }

    /**
     * @brief get_answer
     * слот кнопки ANSWER
     * записывает запросы в файл requests.json
     * запускает многопоточный поиск по всем текущим запросам
     * записывает результаты в файл answers.json
     * выводит результаты поиска в рабочее окно
     */
    void get_answer()
    {
        // изменение информационного окна программы
        qLabel->setText("results");
        qLabel->setStyleSheet("color: rgb(0, 0, 0)");

        std::stringstream text_to_html;
        std::string html_string;

        // проверка наличия запроса
        try
        {
           check_requests(requests);
        }
        catch (const std::exception& x) // если значение max_responses меньше 1
        {
            std::cerr << "Caught exception" << x.what() << std::endl;

            // формирование сообщения для информационного окна
            std::string massage = "request is empty\ndon't forget SEND REQUEST";

            // очистка рабочего окна программы
            textEdit->clear();

            // изменение информационного окна программы
            qLabel->setText(QString(massage.c_str()));
            qLabel->setStyleSheet("color: rgb(255, 0, 0)");
            return;
        }
        // запись запроса в файл requests.json
        convertJson->SetRequests(requests);

        // поиск и сохранение результата в файл answers.json
        convertJson->PutAnswers(searchServer->Search(convertJson->GetRequests()));
        std::shared_ptr<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > > answers = convertJson->GetAnswers();

        // вывод результата в рабочее окно программы
        text_to_html << "answers:" << "<br><br>";

        for(int i = 0; i < answers->size(); i++)
        {
            // формирование номера запроса
            if(i < 9) text_to_html << ("request00" + std::to_string(i + 1));
            else if(i < 99) text_to_html <<  ("request0" + std::to_string(i + 1));
            else text_to_html <<  ("request" + std::to_string(i + 1));
            text_to_html << "<br>";

            // если запрос результативный, выводятся его результаты  result: true, и данные по файлам
            if(answers->at(i).first == "true")
            {
                text_to_html << "result: "<< "true" << "<br>";
                for(auto pairr : answers->at(i).second)
                {
                    text_to_html << "docid: " << pairr.first << ", rank: " << pairr.second << "<br>";
                }
            }
            // иначе выводится result: false
            else
            {
              text_to_html << "result: "<< "false" << "<br>";
            }
            text_to_html << "<br>";
        }

        // вывод результата в рабочее окно программы
        html_string = text_to_html.str();
        textEdit->document()->setHtml(QString(html_string.c_str()));

        // удаление отработанного запроса из вектора запросов
        requests->clear();
    }

};
#endif // MAINWINDOW_H
