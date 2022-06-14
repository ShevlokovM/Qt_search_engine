#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#pragma once

// библиотеки С++
#include <string>
#include <vector>
#include <map>
#include <memory>

//----------------------------------------------------------------------------------------

/**
 * @brief The Entry struct
 * Структура отражающая релевантность одного файла поисковому запросу
*/
struct Entry
{
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);}
};

//-------------------------------------------------------------------------------------------

/**
* @brief The InvertedIndex class
* Класс, отвечающий за индексацию содержимого целевых для поискового запроса файлов,
* с целью ускорения поиска.
*/
class InvertedIndex
{
    std::vector<std::string> docs;

public:
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    // методы

    /**
     * @brief InvertedIndex
     * конструктор
     */
    InvertedIndex() = default;     
     
    /**
    * @brief UpdateDocumentBase
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param input_docs содержимое документов
    */
    void UpdateDocumentBase(std::shared_ptr<std::vector<std::string> > input_docs);

     /**
     * @brief GetWordCount
     * Метод определяет количество вхождений слова word в загруженной базе документов
     * @param word слово, частоту вхождений которого необходимо определить
     * @return возвращает подготовленный список с частотой слов
     */
     std::vector<Entry> GetWordCount(const std::string& word);
};


#endif // INVERTEDINDEX_H
