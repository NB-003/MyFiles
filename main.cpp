#include "bigint.h"

using namespace std;

int main() {
    BigInt a;
    cin >> a;
    string b = (string)a;
    cout << b << endl;
    BigInt o = ~a;
    cout << o;
    return 0;
}
