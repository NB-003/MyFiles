#include "bigint.h"

BigInt::BigInt(int num) {
    if(num < 0) {
        unary_minus = true;
        num *= -1;
    }

    // Puts numbers in digits in reverse order
    do {
        digits.insert(digits.end(), num % 10);
        num /= 10;
    } while(num);
}

BigInt::BigInt(std::string& str) {
    // We copy our string to not change it if we need it in the future.
    std::string str_copy = str;

    if(str_copy[0] == '-' || str_copy[0] == '+') {
        if(str_copy[0] == '-')
            unary_minus = true;
        str_copy.erase(0, 1);
    }

    if(str_copy.empty())
        throw std::invalid_argument(str);

    str_copy.erase(0, std::min(str_copy.find_first_not_of('0'), str_copy.size() - 1));

    // We put numbers in digits in reverse order and throw std::invalid_argument() if an error occurs.
    for(int i = 0; i < str_copy.size(); i++) {
        if(!isdigit(str_copy[i]))
            throw std::invalid_argument(str);
        digits.insert(digits.begin(), str_copy[i] - '0');
    }

    if(digits.size() == 1 && digits[0] == 0)
        unary_minus = false;
}

// Copy constructor
BigInt::BigInt(const BigInt& a) {
    unary_minus = a.unary_minus;
    digits = a.digits;
}

// Move constructor
BigInt::BigInt(BigInt&& a) noexcept {
    unary_minus = a.unary_minus;
    digits = a.digits;

    a.unary_minus = false;
    a.digits = "";
}

// Copy assignment operator
BigInt &BigInt::operator=(const BigInt& a) {
    unary_minus = a.unary_minus;
    digits = a.digits;
    return *this;
}

// Move assignment operator
BigInt &BigInt::operator=(BigInt&& a) noexcept {
    if(this != &a) {
        unary_minus = a.unary_minus;
        digits = a.digits;

        a.unary_minus = false;
        a.digits = "";
    }
    return *this;
}

// Converts a number to its binary representation
std::vector<bool> to_bin(const BigInt &a) {
    BigInt a1(a);
    std::vector<bool> bin;

    // We increment a number here because negative numbers are represented as signed 2's complement
    if(a.unary_minus)
        a1++;
    std::reverse(a1.digits.begin(), a1.digits.end());

    while(a1.digits[0] != 0) {
        if(!a.unary_minus)
            bin.insert(bin.begin(), (a1.digits[a1.digits.size() - 1]) & 1);
        else
            bin.insert(bin.begin(), !((a1.digits[a1.digits.size() - 1]) & 1));

        // We convert a number from decimal to binary by dividing a number by 2 and adding remainders
        for(int i = 0, tmp = 0; i < a1.digits.size(); i++) {
            int digit = ((a1.digits[i]) >> 1) + tmp;
            tmp = ((a1.digits[i]) & 1) * 5;
            a1.digits[i] = digit;
        }
        a1.digits.erase(0, std::min(a1.digits.find_first_not_of('\0'), a1.digits.size() - 1));
    }

    /* We fill vector up to size that is a multiple of eight because
     * each vector element is a bit of a number and 1 byte (minimum for data storage) has 8 bits */
    bin.insert(bin.begin(), 8 - (bin.size() % 8), a.unary_minus);

    return bin;
}

// Converts a binary literal to a number
BigInt from_bin(std::vector<bool> bin) {
    BigInt a;

    /* If first bit is 0, then the number is positive,
     * if not then the number is negative, and we need to read signed 2's complement*/
    if(bin[0] == 0) {
        for(int i = 1; i < bin.size() - 1; i++) {
            a = (a + BigInt(bin[i])) * BigInt(2);
        }
        // To not lose last bit
        if(bin[bin.size() - 1])
            a++;
    }
    else {
        for(int i = 1; i < bin.size() - 1; i++) {
            a = (a + BigInt(!bin[i])) * BigInt(2);
        }
        if(!bin[bin.size() - 1])
            a++;
        a.unary_minus = true;
        a--;
    }
    return a;
}

// Bitwise negation operator
BigInt BigInt::operator~() const {
    std::vector<bool> bin = to_bin(*this);

    for(int i = 0; i < bin.size(); i++) {
        bin[i] = !bin[i];
    }

    return from_bin(bin);
}

// Pre-increment operator
BigInt &BigInt::operator++() {
    *this += BigInt(1);
    return *this;
}

// Post-increment operator
const BigInt BigInt::operator++(int) {
    return ++(*this);
}

// Pre-decrement operator
BigInt &BigInt::operator--() {
    *this -= BigInt(1);
    return *this;
}

// Post-decrement operator
const BigInt BigInt::operator--(int) {
    return --(*this);
}

// Addition assignment operator
BigInt &BigInt::operator+=(const BigInt& a) {
    *this = *this + a;
    return *this;
}

// Subtraction assignment operator
BigInt &BigInt::operator-=(const BigInt& a) {
    *this = *this - a;
    return *this;
}

// Multiplication assignment operator
BigInt &BigInt::operator*=(const BigInt& a) {
    *this = *this * a;
    return *this;
}

// Division assignment operator
BigInt &BigInt::operator/=(const BigInt& a) {
    *this = *this / a;
    return *this;
}

// Modulo assignment operator
BigInt &BigInt::operator%=(const BigInt& a) {
    *this = *this % a;
    return *this;
}

// Bitwise XOR assignment operator
BigInt &BigInt::operator^=(const BigInt& a) {
    *this = *this ^ a;
    return *this;
}

// Bitwise AND assignment operator
BigInt &BigInt::operator&=(const BigInt& a) {
    *this = *this & a;
    return *this;
}

// Bitwise OR assignment operator
BigInt &BigInt::operator|=(const BigInt& a) {
    *this = *this | a;
    return *this;
}

// Unary plus operator
BigInt BigInt::operator+() const {
    return *this;
}

// Unary minus operator
BigInt BigInt::operator-() const {
    BigInt other(*this);
    other.unary_minus = !unary_minus;
    return other;
}

// Equal-to operator
bool BigInt::operator==(const BigInt& a) const {
    return digits == a.digits && unary_minus == a.unary_minus;
}

// Not-equal-to operator
bool BigInt::operator!=(const BigInt& a) const {
    return !(a == *this);
}

// Less-than operator
bool BigInt::operator<(const BigInt& a) const {
    if(unary_minus != a.unary_minus)
        return unary_minus;

    if(unary_minus) {
        BigInt a1 = a;
        BigInt other = *this;
        a1.unary_minus = other.unary_minus = false;
        return a1 < other;
    }

    if(digits.size() != a.digits.size())
        return digits.size() < a.digits.size();

    for(int i = digits.size() - 1; i >= 0; i--) {
        if(digits[i] != a.digits[i])
            return digits[i] < a.digits[i];
    }

    return false;
}

// Greater-than operator
bool BigInt::operator>(const BigInt& a) const {
    return !(*this <= a);
}

// Less-than-or-equal-to operator
bool BigInt::operator<=(const BigInt& a) const {
    return (*this < a) || (*this == a);
}

// Greater-than-or-equal-to operator
bool BigInt::operator>=(const BigInt& a) const {
    return !(*this < a);
}

// Type-casting operator from BigInt to int
BigInt::operator int() const {
    std::vector<bool> bin = to_bin(*this);

    // We take the last 32 bits of number (or less, if possible)
    if(bin.size() >= 32)
        bin = std::vector<bool>(bin.end() - 32, bin.end());

    BigInt other = from_bin(bin);
    std::reverse(other.digits.begin(), other.digits.end());

    for(int i = 0; i < other.digits.size(); i++) {
        other.digits[i] += '0';
    }

    int num = std::stoi(other.digits);
    if(other.unary_minus)
        num *= -1;

    return num;
}

// Type-casting operator from BigInt to std::string
BigInt::operator std::string() const {
    std::string str;
    for(int i = 0; i < digits.size(); i++) {
        str.insert(str.begin(), digits[i] + '0');
    }

    if(unary_minus)
        str.insert(str.begin(), '-');

    return str;
}

// Size in bytes
size_t BigInt::size() const {
    // 1 byte for unary_minus and all other bytes for each digit
    return digits.size() + 1;
}

// Adds absolute values of two numbers
void abs_add(const BigInt& a, const BigInt& b, BigInt& res) {
    int prev_add = 0;
    for(int i = 0; i < std::max(a.digits.size(), b.digits.size()); i++) {
        int add = prev_add;

        if(i < a.digits.size())
            add += a.digits[i];
        if(i < b.digits.size())
            add += b.digits[i];

        res.digits.insert(res.digits.end(), add % 10);

        // If increasing the order of the number
        prev_add = add / 10;
    }
    if(prev_add)
        res.digits.insert(res.digits.end(), prev_add);
}

// Subtracts absolute value of one number from absolute value of another
void abs_sub(const BigInt& a, const BigInt& b, BigInt& res) {
    int prev_sub = 0;
    for(int i = 0; i < std::max(a.digits.size(), b.digits.size()); i++) {
        int sub = prev_sub;

        if(i < a.digits.size())
            sub += a.digits[i];
        if(i < b.digits.size())
            sub -= b.digits[i];

        // If lowering the order of the number
        if(sub < 0) {
            prev_sub = -1;
            sub += 10;
        }
        else
            prev_sub = 0;
        res.digits.insert(res.digits.begin(), sub);
    }
    if(prev_sub)
        res.digits.insert(res.digits.begin(), prev_sub);

    res.digits.erase(0, std::min(res.digits.find_first_not_of('\0'), res.digits.size() - 1));
    std::reverse(res.digits.begin(), res.digits.end());
}

// Addition operator
BigInt operator+(const BigInt& a, const BigInt& b) {
    BigInt res;

    if(!a.unary_minus && !b.unary_minus)
        abs_add(a, b, res);

    else if(!a.unary_minus && b.unary_minus) {
        if(a >= -b)
            abs_sub(a, b, res);
        else {
            res.unary_minus = true;
            abs_sub(b, a, res);
        }
    }

    else if(a.unary_minus && !b.unary_minus) {
        if(-a <= b)
            abs_sub(b, a, res);
        else {
            res.unary_minus = true;
            abs_sub(a, b, res);
        }
    }

    else if(a.unary_minus && b.unary_minus) {
        res.unary_minus = true;
        abs_add(a, b, res);
    }

    return res;
}

// Subtraction operator
BigInt operator-(const BigInt& a, const BigInt& b) {
    BigInt res;

    if(!a.unary_minus && !b.unary_minus) {
        if(a >= b)
            abs_sub(a, b, res);
        else {
            res.unary_minus = true;
            abs_sub(b, a, res);
        }
    }

    else if(a.unary_minus && !b.unary_minus) {
        res.unary_minus = true;
        abs_add(a, b, res);
    }

    else if(!a.unary_minus && b.unary_minus) {
        abs_add(a, b, res);
    }

    else if(a.unary_minus && b.unary_minus) {
        if(a >= b)
            abs_sub(b, a, res);
        else {
            res.unary_minus = true;
            abs_sub(a, b, res);
        }
    }

    return res;
}

// Multiplication operator
BigInt operator*(const BigInt& a, const BigInt& b) {
    BigInt res;

    for(int i = 0; i < a.digits.size(); i++) {
        int prev_add = 0;
        std::string digits;

        for(int j = 0; j < i; j++)
            digits.insert(digits.begin(), '0');

        // Multiplying one digit of first number by all digits of second number
        for(int j = 0; j < b.digits.size(); j++) {
            int add = prev_add + (a.digits[i]) * (b.digits[j]);

            prev_add = add / 10;

            digits.insert(digits.begin(), add % 10 + '0');
        }

        if(prev_add)
            digits.insert(digits.begin(), prev_add + '0');

        res += BigInt(digits);
    }

    if(a.unary_minus ^ b.unary_minus)
        res.unary_minus = true;

    if(res.digits.size() == 1 && res.digits[0] == 0)
        res.unary_minus = false;

    return res;
}

// Division operator
BigInt operator/(const BigInt& a, const BigInt& b) {
    BigInt res;

    if(b.digits.size() == 1 && b.digits[0] == 0)
        throw std::invalid_argument("Division by zero");

    BigInt rest(a.digits[a.digits.size() - 1]);

    BigInt b1(b);
    b1.unary_minus = false;

    int i = a.digits.size() - 2;

    do {
        BigInt divident(rest);
        bool increase_order_of_res = false;

        while(divident < b1 && i >= 0) {
            divident = divident * BigInt(10) + BigInt(a.digits[i--]);

            // To not lose any order increase (for example we can lose all zeros from digits when dividing)
            if(increase_order_of_res)
                res *= BigInt(10);

            increase_order_of_res = true;
        }

        int divider = 0;
        rest = BigInt(divident);

        while(rest >= b1) {
            divider++;
            rest -= b1;
        }

        res = res * BigInt(10) + BigInt(divider);

    } while(i >= 0);

    if((a.unary_minus ^ b.unary_minus) && !(res.digits.size() == 1 && res.digits[0] == 0))
        res.unary_minus = true;


    return res;
}

// Modulo operator (functionally is almost the same as previous one, difference is that rest is replaced by res)
BigInt operator%(const BigInt& a, const BigInt& b) {
    BigInt res;

    if(b.digits.size() == 1 && b.digits[0] == 0)
        throw std::invalid_argument("Modulo by zero");

    res = BigInt(a.digits[a.digits.size() - 1]);

    BigInt b1(b);
    b1.unary_minus = false;

    int i = a.digits.size() - 2;

    do {
        BigInt divident(res);

        while(divident < b1 && i >= 0)
            divident = divident * BigInt(10) + BigInt(a.digits[i--]);

        res = BigInt(divident);

        while(res >= b1)
            res -= b1;

    } while(i >= 0);

    if(res.digits.size() != 1 || res.digits[0] != 0)
        res.unary_minus = a.unary_minus;

    return res;
}

// Bitwise XOR operator
BigInt operator^(const BigInt& a, const BigInt& b) {
    std::vector<bool> a_bin = to_bin(a);
    std::vector<bool> b_bin = to_bin(b);

    if(a_bin.size() < b_bin.size())
        a_bin.insert(a_bin.begin(), b_bin.size() - a_bin.size(), a.unary_minus);
    else if(a_bin.size() > b_bin.size())
        b_bin.insert(b_bin.begin(), a_bin.size() - b_bin.size(), b.unary_minus);

    for(int i = 0; i < a_bin.size(); i++) {
        a_bin[i] = a_bin[i] ^ b_bin[i];
    }

    return from_bin(a_bin);
}

// Bitwise AND operator
BigInt operator&(const BigInt& a, const BigInt& b) {
    std::vector<bool> a_bin = to_bin(a);
    std::vector<bool> b_bin = to_bin(b);

    if(a_bin.size() < b_bin.size())
        a_bin.insert(a_bin.begin(), b_bin.size() - a_bin.size(), a.unary_minus);
    else if(a_bin.size() > b_bin.size())
        b_bin.insert(b_bin.begin(), a_bin.size() - b_bin.size(), b.unary_minus);

    for(int i = 0; i < a_bin.size(); i++) {
        a_bin[i] = a_bin[i] && b_bin[i];
    }

    return from_bin(a_bin);
}

// Bitwise OR operator
BigInt operator|(const BigInt& a, const BigInt& b) {
    std::vector<bool> a_bin = to_bin(a);
    std::vector<bool> b_bin = to_bin(b);

    if(a_bin.size() < b_bin.size())
        a_bin.insert(a_bin.begin(), b_bin.size() - a_bin.size(), a.unary_minus);
    else if(a_bin.size() > b_bin.size())
        b_bin.insert(b_bin.begin(), a_bin.size() - b_bin.size(), b.unary_minus);

    for(int i = 0; i < a_bin.size(); i++) {
        a_bin[i] = a_bin[i] || b_bin[i];
    }

    return from_bin(a_bin);
}

// Output operator
std::ostream& operator<<(std::ostream& o, const BigInt& a) {
    if(a.unary_minus)
        o << '-';

    for(int i = a.digits.size() - 1; i >= 0; i--)
        o << (int)a.digits[i];

    return o;
}

// Input operator
std::istream& operator>>(std::istream& o, BigInt& a) {
    std::string str;
    o >> str;

    a.unary_minus = false;

    if(str[0] == '-' || str[0] == '+') {
        if(str[0] == '-')
            a.unary_minus = true;
        str.erase(str.begin());
    }

    str.erase(0, std::min(str.find_first_not_of('0'), str.size() - 1));

    for(int i = 0; i < str.size(); i++) {
        if(!isdigit(str[i]))
            throw std::invalid_argument(str);
        a.digits.insert(a.digits.begin(), str[i] - '0');
    }

    if(str == "0")
        a.unary_minus = false;

    return o;
}
