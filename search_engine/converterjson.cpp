//
// Created by 111 on 08.05.2022.
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <exception>

#include <QDebug>
#include <QLabel>

#include <nlohmann/json.hpp>
#include "converterjson.h"

// Метод получает данные из файла config.json

std::shared_ptr<config_record> ConverterJSON::GetConfig()
{
    std::ifstream config_file("config.json");
    auto curr_config = std::make_shared<config_record>();

    if(config_file.is_open() && !config_file.eof())
    {
        nlohmann::json dict1;
        nlohmann::json dict2;
        nlohmann::json dict3;

        config_file >> dict1;
        config_file.close();

        dict2 = dict1["config"];
        dict3 = dict1["files"];
        curr_config->name = dict2["name"];
        curr_config->version = dict2["version"];
        curr_config->max_responses = dict2["max_responses"];
        nlohmann::json::iterator itr = dict3.begin();

        for(itr; itr != dict3.end(); itr++)
        {
            curr_config->files.push_back(*itr);
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
            std::cerr << "Caught exception: file config.json isn't open" << x.what() << std::endl;
        }
    }

    return curr_config;
}

 // Метод записывает файл config.json
void ConverterJSON::SetConfig(std::shared_ptr<config_record> conf_rec)
{
    std::ofstream config_file("config.json");

    nlohmann::json dict1;
    nlohmann::json dict2;
    nlohmann::json dict3;

    for(auto file : conf_rec->files)
    {
        dict2.push_back(file);
    }

    dict1["files"] = dict2;
    dict3["name"] = conf_rec->name;
    dict3["version"] = conf_rec->version;
    dict3["max_responses"] = conf_rec->max_responses;
    dict1["config"] = dict3;

    config_file << dict1;
    config_file.close();
}

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/


std::shared_ptr<std::vector<std::string> > ConverterJSON::GetTextDocuments()
{
    auto files_content = std::make_shared<std::vector<std::string> >();
    std::shared_ptr<config_record> curr_config = GetConfig();

    for(auto fil : curr_config->files)
    {
        std::string file_path;
        for(auto ch : fil)
        {
            file_path += ch;
            if(ch == '\\' ) file_path += '\\';
        }
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
        else
        {
            std::string message = "File ";
            message += fil;
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
            * Метод считывает поле max_responses для определения предельного
            * количества ответов на один запрос
            * @return
            */
int ConverterJSON::GetResponsesLimit()
{
    std::ifstream config_file("config.json");
     nlohmann::json dict2;

    if(config_file.is_open())
    {
        nlohmann::json dict1;
        config_file >> dict1;
        dict2 = dict1["config"];
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

    return dict2["max_responses"];
}

// Метод сохраняет запросы в файл requests.json
void ConverterJSON::SetRequests( std::shared_ptr<std::vector<std::string>> requests)
{
    std::ofstream requests_file("requests.json");

    if(requests_file.is_open())
    {
        nlohmann::json dict;
        for(auto request : *requests.get())
        {
            dict["requests"].push_back(request);
        }
        requests_file << dict;
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
            * Метод получения запросов из файла requests.json
            * @return возвращает список запросов из файла requests.json
            */
std::vector<std::string> ConverterJSON::GetRequests()
{
    std::vector<std::string> requests;
    std::ifstream requests_file("requests.json");

    if(requests_file.is_open())
    {
    nlohmann::json dict;
    requests_file >> dict;
    requests = dict["requests"];
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
* Положить в файл answers.json результаты поисковых запросов
*/
void ConverterJSON::PutAnswers(std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > answers)
{
    std::ofstream answers_file("answers.json");
    //ofs.open("test.txt", std::ofstream::out | std::ofstream::trunc); // возможно вообще только этого достаточно
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
    answers_file.open("answers.json");

    nlohmann::json dict5;
    nlohmann::json dict1;

    for(int i = 0; i < answers->size(); i++)
    {
        nlohmann::json dict2;

        std::string key;
        if(i < 9) key = "request00" + std::to_string(i + 1);
        else if(i < 99) key =  "request0" + std::to_string(i + 1);
        else key =  "request" + std::to_string(i + 1);

        std::vector<std::pair<int, float>> record = answers->at(i);
        if(record.empty())
        {
            dict2["result"] = "false";
        }
        else {
            dict2["result"] = "true";
            nlohmann::json dict3;

            // отграничивает кол-во выведеных результатов в запросе значением max_responses из config.json
            int constraint = record.size() < GetResponsesLimit() ? record.size() : GetResponsesLimit();

            for (int j = 0; j < constraint; j++)
            {
                nlohmann::json dict4;
                dict4["docid"] = record[j].first;
                dict4["rank"] = record[j].second;
                dict3.push_back(dict4);
            }
            dict2["relevance"] = dict3;
        }
        dict1[key] = dict2;
    }
    dict5["answers"] = dict1;
    answers_file << dict5;

    answers_file.close();
}

// вытаскивает результат поиска из файла answers.json
std::shared_ptr<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > > ConverterJSON::GetAnswers()
{
    auto result = std::make_shared<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > >();
    std::ifstream answers_file("answers.json");

    if(answers_file.is_open())
    {
        nlohmann::json answers;
        answers_file >> answers;

        nlohmann::json requests;
        requests = answers["answers"];

        for(auto request : requests)
        {
            std::pair<std::string, std::vector<std::pair<int, float> > > one_request_result;
            std::vector<std::pair<int, float>> one_request_relevances;

            if(request["result"] == "true")
            {
                one_request_result.first = "true";

                for(auto file_relevance : request["relevance"])
                {
                    std::pair<int, float> one_file_relevance;
                    one_file_relevance.first = file_relevance["docid"];
                    one_file_relevance.second = file_relevance["rank"];
                    one_request_relevances.push_back(one_file_relevance);
                }

            }
            else
            {
                one_request_result.first = "false";
            }

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

