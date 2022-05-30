

#include <QRegularExpression>

#include <memory>
#include <exception>
#include <sstream>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "searchserver.h"
#include "converterjson.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    convertJson = std::make_shared<ConverterJSON>();
    std::shared_ptr<InvertedIndex> _index = std::make_shared<InvertedIndex>();
    searchServer = std::make_shared<SearchServer>(_index);
    requests = std::make_shared<std::vector<std::string> >();    
}

void MainWindow::set_initial()
{
    // проверка наличия данных конфигурации
    std::shared_ptr<config_record> curr_config = convertJson->GetConfig();

    if(curr_config->max_responses && curr_config->name.size() && curr_config->version.size())
    {
        //индексация базы данных
        searchServer->GetInvertedIndex()->UpdateDocumentBase(convertJson->GetTextDocuments());
        textEdit->clear();
        textEdit->document()->setHtml(QString("Document base updated."));
    }
    else
    {
        qLabel->setText("please, set configuration");
        qLabel->setStyleSheet("color: rgb(255, 0, 0)");

        std::stringstream text_to_html;
        std::string html_string;

        text_to_html << "name: " << curr_config->name << "<br>" << "version: " <<  curr_config->version << "<br>" << "max_responses: " << curr_config->max_responses << "<br>" << "files: " << "<br>";
        for(auto file_name : curr_config->files)
        {
            text_to_html << file_name << "<br>";
        }
        html_string = text_to_html.str();
        //text_to_html >> html_string;
        std::shared_ptr<QString> qString = std::make_shared<QString>(html_string.c_str());
        //qString->fromStdString(html_string);
        textEdit->clear();
        textEdit->document()->setHtml(*qString);
    }
}

void MainWindow::check_keys(std::string key, std::string inpt)
{
    if(key != inpt)
    {
        throw std::exception();
    }
}

void MainWindow::check_max_resp(int max_resp)
{
    if(max_resp < 1)
    {
        throw std::invalid_argument(" max_responses");
    }
}

void MainWindow::check_filename(std::string file)
{
    QRegularExpression exp("\\w+\\.txt$");

    if(!exp.match(QString(file.c_str())).hasMatch())
    {
         throw std::exception();
    }
}

void MainWindow::check_file(std::string file)
{
    std::string file_path;
    for(auto ch : file)
    {
        file_path += ch;
        if(ch == '\\' ) file_path += '\\';
    }
    std::ifstream source_file(file_path);

    if(!source_file.is_open())
    {
        throw std::exception();
    }

    source_file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

