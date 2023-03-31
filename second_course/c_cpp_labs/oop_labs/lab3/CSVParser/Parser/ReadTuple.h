#pragma once
#include <iostream>
#include <sstream>
#include <tuple>

// Since this function was repeating both in instantiated and partially specialized structs templates,
// I decided to take this function out of struct and define it on its own.
template<typename T>
void ReadTupleElement(std::istringstream& linestream, T&& t) {
    std::string t_string;
    getline(linestream, t_string, ',');

    std::istringstream t_stringstream(t_string);
    t_stringstream >> t;
    if(t_stringstream.fail())
        throw std::invalid_argument("Invalid argument 't' format");
}

template<size_t index, typename... Args>
struct ReadTuple {
    // These functions must be in class/struct
    // because using partial specialization in templated functions is not allowed
    static void read(std::istringstream& linestream, std::tuple<Args...>& tuple) {
        ReadTuple<index - 1, Args...>::read(linestream, tuple);
        ReadTupleElement(linestream, std::get<index>(tuple));
    }
};

template<typename... Args>
struct ReadTuple<0, Args...> {
    static void read(std::istringstream& linestream, std::tuple<Args...>& tuple) {
        ReadTupleElement(linestream, std::get<0>(tuple));
    }
};

template<typename... Args>
std::istream& operator>>(std::istream& is, std::tuple<Args...>& tuple) {
    std::string line;
    getline(is, line);
    if(line.empty()) return is;

    std::istringstream linestream(line);
    ReadTuple<sizeof...(Args) - 1, Args...>::read(linestream, tuple);
    return is;
}
