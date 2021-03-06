
// библиотеки С++
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <unordered_set>
#include <thread>
#include <memory>
#include <mutex>

// заголовочные файлы проекта
#include "searchserver.h"
#include "converterjson.h"
#include "poolofthreads.h"

//------------------------------------------------------------------------------------------

std::mutex lock_inexed_map;
std::mutex lock_saerch_result;

//------------------------------------------------------------------------------------------

/**
 * @brief process_one_req
 * функция, обрабатывающая поисковый запрос
 * @param _index указатель на объект, в одном из членов которого лежит индексированная бд
 * @param result указатель на вектор результатов всех текущих запросов
 * @param one_req строка, содержащая запрос
 */
void process_one_req(std::shared_ptr<InvertedIndex> _index, std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > result, std::string one_req)
{
    std::stringstream temp_text;
    std::unordered_set<std::string> unic_words;
    temp_text << one_req;
    while(!temp_text.eof())
    {
        /* Разбивает поисковый запрос на отдельные слова. */
        std::string _word;
        temp_text >> _word;

        /* Формирует из них список уникальных. */
        unic_words.insert(_word);
    }
        /* Сортирует слова в порядке увеличения частоты встречаемости: от самых
        редких до самых частых. По возрастанию значения поля count поля
        freq_dictionary. */

    std::map<int, std::vector<std::string>> sorted_by_freq;

    for(auto wrd : unic_words)
    {
        int sum = 0;
        for(auto pair : _index->freq_dictionary[wrd])
        {
            sum += pair.count;
        }

        sorted_by_freq[sum].push_back(wrd);
     }

        /* По первому, самому редкому слову из списка находит все документы, в которых
встречается слово. */
     std::unordered_set<int> docs_set;

       std::string first_word = *sorted_by_freq.begin()->second.begin();
       std::vector<Entry> * word_store_data = &_index->freq_dictionary[first_word];

       for(auto entry : *word_store_data)
       {
           docs_set.insert(entry.doc_id);
       }


       /* Далее ищет соответствия следующего слова и этого списка документов. Так по
каждому следующему слову. Список документов на каждой итерации должен
уменьшаться или, по крайней мере, не увеличиваться. */
     std::unordered_set<int> docs_temp_set;

     bool miss_first_word = true;
     for(auto itr = sorted_by_freq.begin(); itr !=sorted_by_freq.end(); itr++)
     {
         if(itr == sorted_by_freq.begin())
         {
             if(itr->second.size() != 1)
             {
                 for(int i = 1; i < itr->second.size(); i++)
                 {
                     std::vector<Entry> * vec = &_index->freq_dictionary[itr->second[i]];
                     for(auto entry : *vec)
                     {
                         if(docs_set.count(entry.doc_id)) docs_temp_set.insert(entry.doc_id);
                     }
                 }
             }
             else
             {
                 miss_first_word = false;
             }
         }
         else
         {
             for(int i = 0; i < itr->second.size(); i++)
             {
                 std::vector<Entry> * vec = &_index->freq_dictionary[itr->second[i]];
                 for(auto entry : *vec)
                 {
                     if(docs_set.count(entry.doc_id)) docs_temp_set.insert(entry.doc_id);
                 }
             }
         }

         if(miss_first_word)
         {
             std::vector<int> temp;

             for(auto doc_id: docs_set)
             {
                 if(!docs_temp_set.count(doc_id))
                     temp.push_back(doc_id);
             }

             for(auto num : temp)
             {
                docs_set.erase(num);
             }
         }
         else
         {
             miss_first_word = true;
         }

         docs_temp_set.clear();
     }
                     /* Если в итоге не осталось ни одного документа, то выводит количество
             найденных документов, равное 0. В результат ответа записывает false. */

                     /* Если документы найдены, рассчитывает по каждому из них релевантность и
             выводит её в поле rank в ответе на запрос. Для этого для каждой страницы
             рассчитывается абсолютная релевантность — сумма всех count всех найденных
             в документе слов из коллекции freq_dictionary, и относительная релеваентность, которая делится на
             максимальное значение абсолютной релевантности для всех найденных. */

         std::vector<RelativeIndex> request_relativ_data;
         int max_relevant = 0;
         if(docs_set.size())
         {
             for(auto doc_id : docs_set)
             {
                 RelativeIndex relativeIndex(doc_id);
                 for(auto word : unic_words)
                 {
                     for(auto entry :_index->freq_dictionary[word])
                     {
                         if(entry.doc_id == doc_id)
                         {
                             relativeIndex.rank += entry.count;
                             break;
                         }
                     }
                 }
                 if(relativeIndex.rank > max_relevant) max_relevant = relativeIndex.rank;
                 request_relativ_data.push_back(relativeIndex);
             }
         }
         else
         {
             // исключение
         }

        /* тут рассчитываем относительную релевантность, и сортируем в порядке ее возрастания */
           std::map<float, std::vector<int> > inverted_map;

           for(auto ri : request_relativ_data)
           {
               inverted_map[ri.rank / max_relevant].push_back(ri.doc_id);
           }

           /* Сортирует страницы по убыванию релевантности: от большей к меньшей. */

           std::vector<std::pair<int, float>> one_req_result;


           for(auto pairr : inverted_map)
           {
               for(auto doc_id : pairr.second)
               {
                   std::pair<int, float> pr(doc_id, pairr.first);
                   one_req_result.insert(one_req_result.begin(), pr);
               }
           }

           lock_saerch_result.lock();
           result->push_back(one_req_result);
           lock_saerch_result.unlock();
}

/**
 * @brief process_text
 * функция индексирующая содержание файла, и записывающая его в map для хранения в индексированном виде
 * @param freq_dictionary map, хранящий индексированные данные по всем словам всех целеых файлов
 * @param file_content содержание одного файла
 * @param i номер файла
 */
void process_text(std::map<std::string, std::vector<Entry>>& freq_dictionary, std::string& file_content, int i)
{
    std::stringstream text;
    std::string _word;

    // разбираем содержание файла по словам
    text << file_content;

    while(!text.eof())
    {
        text >> _word;

        lock_inexed_map.lock();

        // если слова еще нет в freq_dictionary
        if(!freq_dictionary.count(_word) && _word != "")
        {
            // просто делаем новый объект Entry (хранит данные по релевантности файла слову), с кол-вом слов - 1
            Entry entry = {(size_t) i, 1};
            // и делаем новый элемент в freq_dictionary, кладем в значение вектор с одним элементом Entry
            freq_dictionary[_word].push_back(entry);
        }
        // если слово уже было индексировано ранее
        else if(_word != "")
        {
            // получаем доступ к вектору данных по файлам по этому слову
            std::vector<Entry>* word_store_data = &freq_dictionary[_word];
            bool find_file = false;

            // ищем, были ли уже данные по файлу, который мы разбираем сейчас
            for(int n = 0; n < word_store_data->size(); n++)
            {
               if(word_store_data->at(n).doc_id == i)
               {
                   // если уже были, обновляем кол-во слов в текущем файле
                   word_store_data->at(n).count++;
                   find_file = true;
                   break;
               }               
            }

            // если не были, делаем новый Entry с doc_id текущего файла, и добавляем его в базу freq_dictionary
            if(!find_file)
            {
                Entry word_in_file;

                word_in_file.count = 1;
                word_in_file.doc_id = i;

                word_store_data->push_back(word_in_file);
            }
        }
        lock_inexed_map.unlock();
        _word = "";
    }
}
