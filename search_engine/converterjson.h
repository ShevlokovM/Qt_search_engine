#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H


#pragma once

#include <string>
#include <vector>
#include <memory>


/**
* Структура записи файла config.json
*/


struct config_record
{
    std::string name = "";
    std::string version = "";
    int max_responses = 5;

    std::vector<std::string> files;
};

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON
{
public:
    ConverterJSON() = default;

    // Метод получает данные из файла config.json

    std::shared_ptr<config_record> GetConfig();

    // Метод записывает файл config.json
   void SetConfig(std::shared_ptr<config_record> conf_rec);


   // Метод сохраняет запросы в файл requests.json
   void SetRequests(std::shared_ptr<std::vector<std::string>> requests);

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
   std::shared_ptr<std::vector<std::string> > GetTextDocuments();

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit();
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/

    std::vector<std::string> GetRequests();


/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void PutAnswers(std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > >
    answers);

    // вытаскивает результат поиска из файла answers.json
    std::shared_ptr<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > > GetAnswers();

};


#endif // CONVERTERJSON_H
