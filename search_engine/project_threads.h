#ifndef PROJECT_THREADS_H
#define PROJECT_THREADS_H

#pragma once

#include<memory>

#include "invertedindex.h"

void process_one_req(std::shared_ptr<InvertedIndex> _index, std::shared_ptr<std::vector<std::vector<std::pair<int, float> > > >, std::string one_req);

void process_text(std::map<std::string, std::vector<Entry>>& freq_dictionary, std::string& file_content, int i);
#endif // PROJECT_THREADS_H
