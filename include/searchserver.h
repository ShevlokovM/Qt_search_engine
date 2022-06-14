#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#pragma once

// библиотеки С++
#include <memory>

// заголовочные файлы проекта
#include "invertedindex.h"

//------------------------------------------------------------------------------------------

/**
 * @brief The RelativeIndex struct
 * структура отображающая релевантность одного файла одному слову в запросе
 */
struct RelativeIndex{
    size_t doc_id;
    float rank;

    RelativeIndex() : rank(0), doc_id(0) {};
    RelativeIndex(int id) : doc_id(id), rank(0) {};

    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

//---------------------------------------------------------------------------------------------

/**
 * @brief The SearchServer class
 * класс отвечающий за обработку поискового запроса
 */
class SearchServer {

    std::shared_ptr<InvertedIndex> _index;

public:
    /**
     * @brief GetInvertedIndex
     * геттер
     * @return указатель на объект, в котором лежит индексированная бд
     */
    std::shared_ptr<InvertedIndex> GetInvertedIndex() { return _index; }

    /**
     * @brief SearchServer
    * @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в запросе
    */
    SearchServer(std::shared_ptr<InvertedIndex> idx) : _index(idx){ };
    
    
    /**
     * @brief Search
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла requests.json
    * @return возвращает отсортированный список релевантных ответов для заданных запросов
    */
    std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > Search(const std::vector<std::string>& queries_input);

};



#endif // SEARCHSERVER_H
