#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <functional>

template<size_t index, typename... Args>
class PrintTuple {
public:
    static void print(std::tuple<Args...> const& t, std::ostream& os) {
        PrintTuple<index - 1, Args...>::print(t, os);
        PrintTupleElement(std::get<index>(t), os);
    }
private:
    template<typename T>
    static void PrintTupleElement(T&& t, std::ostream& os) {
        os << t;
        if(index != sizeof...(Args) - 1)
            os << ", ";
        else
            os << std::endl;
    }
};

template<typename... Args>
class PrintTuple<0, Args...> {
public:
    static void print(std::tuple<Args...> const& t, std::ostream& os) {
        PrintTupleElement(std::get<0>(t), os);
    }
private:
    template<typename T>
    static void PrintTupleElement(T&& t, std::ostream& os) {
        os << t << ", ";
    }
};

template<typename... Args>
std::ostream& operator<<(std::ostream& os, std::tuple<Args...> const& tuple) {
    PrintTuple<sizeof...(Args) - 1, Args...>::print(tuple, os);
    return os;
}
