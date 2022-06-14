#ifndef PROJECT_THREADS_H
#define PROJECT_THREADS_H

/**
 * библиотека функций проекта, вызываемых в потоках
 */

#pragma once

// стандартные библиотеки C++
#include<memory>

// заголовочные файлы проекта
#include "invertedindex.h"

//--------------------------------------------------------------------------------

/**
 * @brief process_one_req
 * функция, обрабатывающая поисковый запрос
 * @param _index указатель на объект, в одном из членов которого лежит индексированная бд
 * @param result указатель на вектор результатов всех текущих запросов
 * @param one_req строка, содержащая запрос
 */
void process_one_req(std::shared_ptr<InvertedIndex> _index, std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > > result, std::string one_req);

/**
 * @brief process_text
 * функция индексирующая содержание файла, и записывающая его в map для хранения в индексированном виде
 * @param freq_dictionary map, хранящий индексированные данные по всем словам всех целеых файлов
 * @param file_content содержание одного файла
 * @param i номер файла
 */
void process_text(std::map<std::string, std::vector<Entry>>& freq_dictionary, std::string& file_content, int i);
#endif // PROJECT_THREADS_H
