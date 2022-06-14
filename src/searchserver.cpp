//
// Created by 111 on 09.05.2022.
//

// библиотеки С++
#include <string>
#include <vector>
#include <thread>
#include <memory>

// заголовочные файлы проекта
#include "searchserver.h"
#include "converterjson.h"
#include "poolofthreads.h"

//-------------------------------------------------------------------------------------------

/**
 * @brief Search
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла requests.json
* @return возвращает отсортированный список релевантных ответов для заданных запросов
*/
std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > SearchServer::Search(const std::vector<std::string>& queries_input)
{  
    // вектор для записи результатов всех текущих поисковых запросов
    auto result = std::make_shared<std::vector<std::vector<std::pair<int, float> > > > ();

    // обработка всех текущих поисковых запросов
    for(auto request_text : queries_input)
    {
        // запуск потока, обрабатывающего каждый отдельный поисковый запрос
       std::thread thrd = std::thread([this, result, request_text] ()
       {
           process_one_req(this->_index, result, request_text);
       });
        thrd.join();
     }

        return result;
}
