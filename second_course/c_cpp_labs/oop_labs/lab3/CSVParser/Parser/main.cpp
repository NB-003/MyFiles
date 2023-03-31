#include "Parser.h"
#include "PrintTuple.h"
#include <iostream>

using namespace std;

int main() {
    using tuple = std::tuple<string, double, int, string, size_t>;
    Parser<string, double, int, string, size_t> parser("../Recources/input.csv", 3);

    cout << parser[0] << endl;
    cout << parser[3] << endl;
    cout << parser[1] << endl;

    for(tuple t1: parser) {
        cout << t1 << endl;
    }
}
