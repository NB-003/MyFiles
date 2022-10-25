#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class BigInt {
public:
    BigInt() = default;
    explicit BigInt(int);
    explicit BigInt(std::string&);
    BigInt(const BigInt&);
    BigInt(BigInt&&) noexcept;
    ~BigInt() = default;

    // Copy assignment and move assignment operators
    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt&&) noexcept;

    // Bitwise negation operator
    BigInt operator~() const;

    // Pre-/Post- increment and decrement operators
    BigInt& operator++();
    const BigInt operator++(int);
    BigInt& operator--();
    const BigInt operator--(int);

    // Assignment operators
    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator^=(const BigInt&);
    BigInt& operator&=(const BigInt&);
    BigInt& operator|=(const BigInt&);

    // Unary plus/minus operators
    BigInt operator+() const;
    BigInt operator-() const;

    // Comparison operators
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    // Type-casting operators
    explicit operator int() const;
    explicit operator std::string() const;

    // Size in bytes
    size_t size() const;

    // Supporting functions for bitwise operations
    friend std::vector<bool> to_bin(const BigInt&);
    friend BigInt from_bin(std::vector<bool>);

    // Supporting functions for arithmetic operations
    friend void abs_add(const BigInt&, const BigInt&, BigInt&);
    friend void abs_sub(const BigInt&, const BigInt&, BigInt&);

    // Arithmetic operators
    friend BigInt operator+(const BigInt&, const BigInt&);
    friend BigInt operator-(const BigInt&, const BigInt&);
    friend BigInt operator*(const BigInt&, const BigInt&);
    friend BigInt operator/(const BigInt&, const BigInt&);
    friend BigInt operator%(const BigInt&, const BigInt&);

    // Bitwise operators
    friend BigInt operator^(const BigInt&, const BigInt&);
    friend BigInt operator&(const BigInt&, const BigInt&);
    friend BigInt operator|(const BigInt&, const BigInt&);

    // Input/output operators
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    friend std::istream& operator>>(std::istream&, BigInt&);
private:
    bool unary_minus = false;
    std::string digits;
};
