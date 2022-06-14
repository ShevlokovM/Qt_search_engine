
// библиотеки Qt
#include <QRegularExpression>

// библиотеки С++
#include <memory>
#include <exception>
#include <sstream>

// заголовочные файлы проекта
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "searchserver.h"
#include "converterjson.h"

//-------------------------------------------------------------------------------------------

/**
* @brief MainWindow конструктор
* инициализирует std::shared_ptr членов класса и членов членов класса
* @param parent указатель на объект родительского класса
*/
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

/**
 * @brief set_initial
 * проверяет готовность приложения к работе, наличие конфигурации, если нет, просит ввести
 * запускает индексацию файлов, если конфигурация есть
 * необходимо запускать после того, как класс связан с графическим интерфейсом
 * @return
 */
void MainWindow::set_initial()
{
    // проверка наличия данных конфигурации
    std::shared_ptr<config_record> curr_config = convertJson->GetConfig();

    // если данные есть
    if(curr_config->max_responses && curr_config->name.size() && curr_config->version.size())
    {
        //индексация базы данных
        searchServer->GetInvertedIndex()->UpdateDocumentBase(convertJson->GetTextDocuments());
        textEdit->clear();
        textEdit->document()->setHtml(QString("Document base updated."));
    }
    else
    {
        // изменение информационного окна программы
        qLabel->setText("please, set configuration");
        qLabel->setStyleSheet("color: rgb(255, 0, 0)");

        // вывод в рабочее окно шаблона конфигурации
        std::stringstream text_to_html;
        std::string html_string;

        // формирование строки html
        text_to_html << "name: " << curr_config->name << "<br>" << "version: " <<  curr_config->version << "<br>" << "max_responses: " << curr_config->max_responses << "<br>" << "files: " << "<br>";
        for(auto file_name : curr_config->files)
        {
            text_to_html << file_name << "<br>";
        }

        html_string = text_to_html.str();

        // вывод строки html в рабоче окно
        textEdit->document()->setHtml(QString(html_string.c_str()));
    }
}

/**
 * @brief check_keys
 * проверяет наличие в структуре конфигурации всех необходимых ключей
 * @param key ключ, который нужно проверить
 * @param inpt введенное пользователем имя ключа
 */
void MainWindow::check_keys(std::string key, std::string inpt)
{
    if(key != inpt)
    {
        throw std::exception();
    }
}

/**
 * @brief check_max_resp
 * проверяет что значение максимального кол-ва файлов в поисковом запросе
 * не меньше 0
 * @param max_resp значение максимального кол-ва файлов в поисковом запросе,
 * введенное пользователем
 */
void MainWindow::check_max_resp(int max_resp)
{
    if(max_resp < 1)
    {
        throw std::invalid_argument(" max_responses");
    }
}

/**
 * @brief check_filename
 * проверяет расширение файла. должно быть .txt
 * @param filename имя файла
 */
void MainWindow::check_filename(std::string file)
{
    QRegularExpression exp("\\w+\\.txt$");

    if(!exp.match(QString(file.c_str())).hasMatch())
    {
         throw std::exception();
    }
}

/**
 * @brief check_file
 * проверяет наличие файла в папке resources
 * @param file путь к файлу
 */
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

/**
 * @brief check_requests
 * проверка наличия запросов
 * @param requests вектор запросов
 */
void MainWindow::check_requests(std::shared_ptr<std::vector<std::string>> requests)
{
    if(!requests->size())
    {
        throw std::exception();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

