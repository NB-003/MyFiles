#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include "ReadTuple.h"

template<typename... Args>
class Parser {
private:
    class Iterator {
    public:
        Iterator(std::ifstream*);

        // All operators for tuple run implementation
        Iterator& operator++();
        std::tuple<Args...>& operator*();
        bool operator==(const Iterator&) const;
        bool operator!=(const Iterator&) const;
    private:
        std::ifstream* fin_ptr;
        std::tuple<Args...> curLine;
        size_t index;
    };
public:
    explicit Parser(const char*, size_t);
    explicit Parser(std::string&, size_t);

    Iterator begin();
    Iterator end();

    std::tuple<Args...> operator[](size_t);
private:
    std::ifstream fin;
    size_t offset;

    void Reset();
};

template<typename... Args>
void Parser<Args...>::Reset() {
    std::string str;
    fin.clear();
    fin.seekg(0, std::ifstream::beg);
    for(int i = 0; i < offset; i++)
        getline(fin, str);
}

template<typename... Args>
Parser<Args...>::Parser(const char* path, size_t offset): fin(path) {
    if(!fin.is_open())
        throw std::invalid_argument("Can't open file");

    std::string str;
    int linesNumber;
    for(linesNumber = 0; getline(fin, str); linesNumber++);

    if(offset >= linesNumber)
        throw std::invalid_argument("Offset is too big");

    this->offset = offset;

    Reset();
}

template<typename... Args>
Parser<Args...>::Parser(std::string& path, size_t offset): Parser(path.c_str(), offset) {}

template<typename... Args>
Parser<Args...>::Iterator::Iterator(std::ifstream* fin): fin_ptr(fin) {
    if(fin_ptr != nullptr) {
        index = 0;
        (*fin_ptr) >> curLine;
    }
    else index = -1;
}

template<typename... Args>
class Parser<Args...>::Iterator& Parser<Args...>::Iterator::operator++() {
    if(fin_ptr == nullptr) return *this;

    index++;
    (*fin_ptr) >> curLine;

    if(fin_ptr->eof()) {
        fin_ptr = nullptr;
        index = -1;
    }

    return *this;
}

template<typename... Args>
std::tuple<Args...>& Parser<Args...>::Iterator::operator*() {
    return curLine;
}

template<typename... Args>
bool Parser<Args...>::Iterator::operator==(const class Parser<Args...>::Iterator& it) const {
    return fin_ptr == it.fin_ptr && index == it.index;
}

template<typename... Args>
bool Parser<Args...>::Iterator::operator!=(const class Parser<Args...>::Iterator& it) const {
    return !(*this == it);
}

template<typename... Args>
class Parser<Args...>::Iterator Parser<Args...>::begin() {
    return Iterator(&fin);
}

template<typename... Args>
class Parser<Args...>::Iterator Parser<Args...>::end() {
    return Iterator(nullptr);
}

template<typename... Args>
std::tuple<Args...> Parser<Args...>::operator[](size_t i) {
    Iterator it = begin();

    for(size_t j = 0; j < i; ++j, ++it);

    Reset();
    return *it;
}
