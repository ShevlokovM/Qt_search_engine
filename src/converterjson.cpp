//
// Created by 111 on 08.05.2022.
//

// библиотеки Qt
#include <QDebug>
#include <QLabel>

// библиотеки С++
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <exception>

// сторонние библиотеки
#include <nlohmann/json.hpp>

// заголовочные фйлы проекта
#include "converterjson.h"

//----------------------------------------------------------------------------------

/**
* @brief GetConfig
* Метод получает данные из файла config.json
* @return возвращает указатель на структуру, содержащую текущую конфигурацию приложения
*/
std::shared_ptr<config_record> ConverterJSON::GetConfig()
{
    std::ifstream config_file("config.json");
    auto curr_config = std::make_shared<config_record>();

    // заполнение структуры config_record
    if(config_file.is_open() && !config_file.eof())
    {
        nlohmann::json json_whole_file;
        nlohmann::json json_config;
        nlohmann::json json_files;

        // копируем весь файл
        config_file >> json_whole_file;
        config_file.close();

        // разбираем первый уровень
        json_config = json_whole_file["config"];
        json_files = json_whole_file["files"];

        // разбираем "config"
        curr_config->name = json_config["name"];
        curr_config->version = json_config["version"];
        curr_config->max_responses = json_config["max_responses"];

        // разбираем "files"
        for(auto itr = json_files.begin(); itr != json_files.end(); itr++)
        {
            curr_config->files.push_back(*itr);
        }
    }
    // если файл config.json пустой - ошибка
    else
    {
        try
        {
            throw std::exception();
        }
        catch (const std::exception& x)
        {
            std::cerr << "Caught exception: file config.json isn't open" << x.what() << std::endl;
        }
    }

    return curr_config;
}

/**
* @brief SetConfig
* Метод записывает файл config.json
* @param conf_rec структура файла конфигурации проекта
* @return
*/
void ConverterJSON::SetConfig(std::shared_ptr<config_record> conf_rec)
{
    std::ofstream config_file("config.json");

    nlohmann::json json_whole_file;
    nlohmann::json json_files;
    nlohmann::json json_config;

    // переносим данные из config_record в json

    // записываем список файлов
    for(auto file : conf_rec->files)
    {
        json_files.push_back(file);
    }    

    // записываем параметры конфигурации
    json_config["name"] = conf_rec->name;
    json_config["version"] = conf_rec->version;
    json_config["max_responses"] = conf_rec->max_responses;

    // записываем корень файла
    json_whole_file["files"] = json_files;
    json_whole_file["config"] = json_config;

    // добавляем json в файл config.json
    config_file << json_whole_file;
    config_file.close();
}

/**
* @brief GetTextDocuments
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
std::shared_ptr<std::vector<std::string> > ConverterJSON::GetTextDocuments()
{
    auto files_content = std::make_shared<std::vector<std::string> >();

    // получаем текущую конфигурацию
    std::shared_ptr<config_record> curr_config = GetConfig();

    // разбираем список файлов текущей конфигурации
    for(auto fil_e : curr_config->files)
    {
        // делаем путь из имени файла
        std::string file_path;
        for(auto ch : fil_e)
        {
            file_path += ch;
            if(ch == '\\' ) file_path += '\\';
        }

        // достаем содержимое файла, сохраняем в строку, и кладем в возвращаемый вектор
        std::ifstream file_open(file_path);        
        std::string file_content = "";
        std::string temp;

        if(file_open.is_open())
        {
            while(true)
            {
                file_open >> temp;
                file_content += temp + " ";
                if(file_open.eof()) break;
            }
            files_content->push_back(file_content);
        }
        // если файл не открылся - ошибка
        else
        {
            std::string message = "File ";
            message += fil_e;
            message += " isn't open.";
            try
            {
                throw std::exception();
            }
            catch (const std::exception& x)
            {
                std::cerr << "Caught exception: " << message << x.what() << std::endl;
            }
        }
    }

    return files_content;
}

/**
* @brief GetResponsesLimit
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
int ConverterJSON::GetResponsesLimit()
{
    std::ifstream config_file("config.json");
     nlohmann::json config;

    if(config_file.is_open())
    {
        nlohmann::json config_file_content;
        config_file >> config_file_content;
        config = config_file_content["config"];
    }
    else
    {
        try
        {
            throw std::exception();
        }
        catch (const std::exception& x)
        {
            std::cerr << "Caught exception: file config.json isn't open" << x.what() << std::endl;
        }
    }

    return config["max_responses"];
}

/**
* @brief SetRequests
* Метод сохраняет запросы в файл requests.json
* @param requests вектор, содержащий все текущие запросы
* @return
*/
void ConverterJSON::SetRequests( std::shared_ptr<std::vector<std::string>> requests)
{
    std::ofstream requests_file("requests.json");

    if(requests_file.is_open())
    {
        nlohmann::json request_file_content;
        for(auto request : *requests.get())
        {
            request_file_content["requests"].push_back(request);
        }
        requests_file << request_file_content;
        requests_file.close();
    }
    else
    {
        try
        {
            throw std::exception();
        }
        catch (const std::exception& x)
        {
            std::cerr << "Caught exception: file requests.json isn't open" << x.what() << std::endl;
        }
    }
}

/**
* @brief GetRequests
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector<std::string> ConverterJSON::GetRequests()
{
    std::vector<std::string> requests;
    std::ifstream requests_file("requests.json");

    if(requests_file.is_open())
    {
    nlohmann::json requests_file_content;
    requests_file >> requests_file_content;

    for (int i = 0; i < requests_file_content["requests"].size(); i++)
        {
            requests.push_back(requests_file_content["requests"][i]);
        }

    requests_file.close();
    }
    else
    {
        try
        {
            throw std::exception();
        }
        catch (const std::exception& x)
        {
            std::cerr << "Caught exception: file requests.json isn't open" << x.what() << std::endl;
        }
    }

    return requests;
}

/**
* @brief PutAnswers
* Метод записывает в файл answers.json результаты поисковых запросов
* @param answers указатель на вектор, содержащий ответы на все текущие запросы
*/
void ConverterJSON::PutAnswers(std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > answers)
{
    std::ofstream answers_file("answers.json");

    // чистим файл с ответами
    if(answers_file.is_open())
    {
        answers_file << "";
        answers_file.close();
    }
    else
    {
        std::ifstream temp("answers.json");
        temp.close();
    }

    // записываем результаты всех запросов в файл

    answers_file.open("answers.json");
    nlohmann::json answers_file_content;
    nlohmann::json json_answers;

    // разбираем список ответов на текущие запросы
    for(int i = 0; i < answers->size(); i++)
    {
        nlohmann::json json_request;

        // делаем имя запроса с номером
        std::string key;
        if(i < 9) key = "request00" + std::to_string(i + 1);
        else if(i < 99) key =  "request0" + std::to_string(i + 1);
        else key =  "request" + std::to_string(i + 1);

        std::vector<std::pair<int, float>> record = answers->at(i);

        // если ответ на запрос нулевой
        if(record.empty())
        {
            json_request["result"] = "false";
        }
        // если поиск увенчался успехом
        else {
            json_request["result"] = "true";
            nlohmann::json all_files_results;

            // отграничивает кол-во выведеных результатов в запросе значением max_responses из config.json
            int constraint = record.size() < GetResponsesLimit() ? record.size() : GetResponsesLimit();

            // переводим в json результаты запроса по каждому файлу, и кладем в json all_files_results
            for (int j = 0; j < constraint; j++)
            {
                nlohmann::json one_file_result;

                one_file_result["docid"] = record[j].first;
                one_file_result["rank"] = record[j].second;

                all_files_results.push_back(one_file_result);
            }
            json_request["relevance"] = all_files_results;
        }
        json_answers[key] = json_request;
    }
    answers_file_content["answers"] = json_answers;
    answers_file << answers_file_content;

    answers_file.close();
}

/**
* @brief GetAnswers
* Метод получает результаты поиска из файла answers.json
* @return указатель на вектор, содержащий ответы на все текущие запросы
*/
std::shared_ptr<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > > ConverterJSON::GetAnswers()
{
    auto result = std::make_shared<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > >();
    std::ifstream answers_file("answers.json");

    // разбираем json из файла answers.json
    if(answers_file.is_open())
    {
        // разбираем первый уровень
        nlohmann::json answers;
        answers_file >> answers;

        // разбираем второй уровень
        nlohmann::json requests;
        requests = answers["answers"];

        //  разбираем запросы
        for(auto request : requests)
        {
            std::pair<std::string, std::vector<std::pair<int, float> > > one_request_result;
            std::vector<std::pair<int, float>> one_request_relevances;

            // если по  запросу есть результаты
            if(request["result"] == "true")
            {
                one_request_result.first = "true";

                // разбираем результаты запросов по файлам, и складываем в one_request_relevances
                for(auto file_relevance : request["relevance"])
                {
                    std::pair<int, float> one_file_relevance;

                    one_file_relevance.first = file_relevance["docid"];
                    one_file_relevance.second = file_relevance["rank"];

                    one_request_relevances.push_back(one_file_relevance);
                }

            }
            // если по запросу не получено результатов
            else
            {
                one_request_result.first = "false";
            }

            // кладем результат по одному запросу в вектор результатов всех текущих запросов
            one_request_result.second = one_request_relevances;
            result->push_back(one_request_result);
        }
    }
    else
    {
        try
        {
            throw std::exception();
        }
        catch (const std::exception& x)
        {
            std::cerr << "Caught exception: file answers.json isn't open" << x.what() << std::endl;
        }
    }

    return result;
}

