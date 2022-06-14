
// библиотеки С++
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <thread>

// заголовочные файлы проекта
#include "invertedindex.h"
#include "poolofthreads.h"

//----------------------------------------------------------------------------------

/**
* @brief UpdateDocumentBase
* Обновить или заполнить базу документов, по которой будем совершать поиск
* @param input_docs содержимое документов
*/
void InvertedIndex::UpdateDocumentBase(std::shared_ptr<std::vector<std::string> > input_docs)
{
    std::vector<std::thread*> threads;
    std::stringstream text;    

    // разбираем вектор с содержимым разных целевых файлов, индексируем содержание
    for(int i = 0; i < input_docs->size(); i++)
    {
        std::string file_content = input_docs->at(i);

        // запуск потока, индексирующего содержание одного файла
        // результат кладем в map this->freq_dictionary
        std::thread thrd = std::thread([this, &file_content, i] ()
        {
            process_text(this->freq_dictionary, file_content, i);
        });

        thrd.join();
        text.str() = "";
    }
}

/**
* @brief GetWordCount
* Метод определяет количество вхождений слова word в загруженной базе документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
    if(freq_dictionary.count(word)) return freq_dictionary[word];
    else return std::vector<Entry> {};
}
