#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#pragma once

#include <memory>

// заголовочные файлы проекта
#include "InvertedIndex.h"

struct RelativeIndex{
    size_t doc_id;
    float rank;

    RelativeIndex() : rank(0) {};
    RelativeIndex(int id) : doc_id(id), rank(0) {};

    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};


class SearchServer {

    std::shared_ptr<InvertedIndex> _index;
public:
    // по сути геттер
    std::shared_ptr<InvertedIndex> GetInvertedIndex() { return _index; }

            /**
            * @param idx в конструктор класса передаётся ссылка на класс
            InvertedIndex,
            * чтобы SearchServer мог узнать частоту слов встречаемых в
            запросе
            */
    SearchServer(std::shared_ptr<InvertedIndex> idx) : _index(idx){ };
    
    
            /**
            * Метод обработки поисковых запросов
            * @param queries_input поисковые запросы взятые из файла
            requests.json
            * @return возвращает отсортированный список релевантных ответов для
            заданных запросов
            */
    std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > search(const std::vector<std::string>& queries_input);


    
};



#endif // SEARCHSERVER_H
