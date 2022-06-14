#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#pragma once

// библиотеки С++
#include <string>
#include <vector>
#include <memory>

//-------------------------------------------------------------------------------------------

/**
* @brief config_record struct
* Структура записи файла config.json
*/
struct config_record
{
    std::string name = "";
    std::string version = "";
    int max_responses = 5;

    std::vector<std::string> files;
};

//---------------------------------------------------------------------------------------

/**
* @brief ConverterJSON class
* Класс для работы с JSON-файлами. Методы класса преобразуют хранимые данные из JSON в контейнеры,
*  понятные для C++, для последующей обработки, и обратно.
*/
class ConverterJSON
{
public:
    /**
    * @brief
    * конструктор
    */
    ConverterJSON() = default;

    /**
    * @brief GetConfig
    * Метод получает данные из файла config.json
    * @return возвращает указатель на структуру, содержащую текущую конфигурацию приложения
    */
    std::shared_ptr<config_record> GetConfig();

    /**
    * @brief SetConfig
    * Метод записывает файл config.json
    * @param conf_rec структура файла конфигурации проекта
    * @return
    */
    void SetConfig(std::shared_ptr<config_record> conf_rec);

    /**
    * @brief SetRequests
    * Метод сохраняет запросы в файл requests.json
    * @param requests вектор, содержащий все текущие запросы
    * @return
    */
    void SetRequests(std::shared_ptr<std::vector<std::string>> requests);

    /**
    * @brief GetTextDocuments
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    std::shared_ptr<std::vector<std::string> > GetTextDocuments();

    /**
    * @brief GetResponsesLimit
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * @return
    */
    int GetResponsesLimit();

    /**
    * @brief GetRequests
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();

    /**
    * @brief PutAnswers
    * Метод записывает в файл answers.json результаты поисковых запросов
    * @param answers указатель на вектор, содержащий ответы на все текущие запросы
    */
    void PutAnswers(std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > >
    answers);

    /**
    * @brief GetAnswers
    * Метод получает результаты поиска из файла answers.json
    * @return указатель на вектор, содержащий ответы на все текущие запросы
    */
    std::shared_ptr<std::vector<std::pair<std::string, std::vector<std::pair<int, float> > > > > GetAnswers();

};


#endif // CONVERTERJSON_H
