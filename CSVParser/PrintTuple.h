#pragma once
#include <iostream>
#include <tuple>

template<size_t index, typename... Args>
class PrintTuple {
public:
    static void print(std::ostream& os, std::tuple<Args...> const& tuple) {
        PrintTuple<index - 1, Args...>::print(os, tuple);
        PrintTupleElement(os, std::get<index>(tuple));
    }
private:
    template<typename T>
    static void PrintTupleElement(std::ostream& os, T&& t) {
        os << t;
        if(index != sizeof...(Args) - 1)
            os << ",";
    }
};

template<typename... Args>
class PrintTuple<0, Args...> {
public:
    static void print(std::ostream& os, std::tuple<Args...> const& tuple) {
        PrintTupleElement(os, std::get<0>(tuple));
    }
private:
    template<typename T>
    static void PrintTupleElement(std::ostream& os, T&& t) {
        os << t << ",";
    }
};

template<typename... Args>
std::ostream& operator<<(std::ostream& os, std::tuple<Args...> const& tuple) {
    PrintTuple<sizeof...(Args) - 1, Args...>::print(os, tuple);
    return os;
}
