//
// Created by 111 on 09.05.2022.
//

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <unordered_set>
#include <thread>
#include <memory>

// заголовочные файлы проекта
#include "searchserver.h"
#include "converterjson.h"
#include "project_threads.h"


/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/


std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > SearchServer::search(const std::vector<std::string>& queries_input)
{  
    auto result = std::make_shared<std::vector<std::vector<std::pair<int, float> > > > ();
    for(auto request_text : queries_input)
    {
        // threads[i] = std::thread([this, &result, i, &lhs, &rhs](){
       // multiply_threading(result, i, lhs, rhs);

       std::thread thrd = std::thread([this, result, request_text] ()
       {
           process_one_req(this->_index, result, request_text);
       });
        thrd.join();
     }
         /* Записывает результат работы в файл answers.json в соответствии с форматом. */
        return result;
}
