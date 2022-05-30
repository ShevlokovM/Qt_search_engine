#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H


#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>


struct Entry
{
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);}
};

class InvertedIndex
{
    std::vector<std::string> docs;
public:
    std::map<std::string, std::vector<Entry>> freq_dictionary;

public:
   
    
    InvertedIndex() = default;

     //void process_text(std::stringstream text, int i);
     
     
                    /**
                * Обновить или заполнить базу документов, по которой будем совершать
                поиск*
                @param texts_input содержимое документов
                */

    void UpdateDocumentBase(std::shared_ptr<std::vector<std::string> > input_docs);

                    /**
                     * * Метод определяет количество вхождений слова word в загруженной базе
                документов
                * @param word слово, частоту вхождений которого необходимо определить
                * @return возвращает подготовленный список с частотой слов
                */

    std::vector<Entry> GetWordCount(const std::string& word);

};


#endif // INVERTEDINDEX_H
