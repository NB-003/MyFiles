# Лабораторная работа 1. Большое целое число BigInt

## Общая информация:
В этой задаче требуется реализовать большое целое число произвольной точности и набор операций над ним.

## Задача:
1. Реализовать большое целое  с заданным интерфейсом (см. раздел “Реализация”). Выбрать способ хранения и изменения размера содержимого.
2. Вместо malloc использовать new, вместо free использовать delete
3. Методы строки можно посмотреть здесь: https://en.cppreference.com/w/cpp/string/basic_string
4. Тщательно задокументировать публичные члены класса на языке, приближенном к техническому английскому.
5. Написать юнит-тесты на все публичные методы класса с помощью любой специализированной библиотеки.

## Реализация:
class BigInt {
public:
    BigInt();
    BigInt(int);
    BigInt(std::string); // бросать исключение std::invalid_argument при ошибке
    BigInt(const BigInt&);
    ~BigInt();

    BigInt& operator=(const BigInt&);  //возможно присваивание самому себе!

    BigInt operator~() const;

    BigInt& operator++();
    const BigInt operator++(int) const;
    BigInt& operator--();
    const BigInt operator--(int) const;

    BigInt& operator+=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator^=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator&=(const BigInt&);
    BigInt& operator|=(const BigInt&);

    BigInt operator+() const;  // unary +
    BigInt operator-() const;  // unary -

    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    operator int() const;
    operator std::string() const;

    size_t size() const;  // size in bytes
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);


std::ostream& operator<<(std::ostream& o, const BigInt& i);

## Повышенная сложность (по желанию):
1. Реализовать семантику перемещения:
    BigInt(BigInt&&);
    BigInt& operator=(BigInt&&);

2. Реализовать оператор ввода из потока с корректной обработкой ошибок:
    std::istream& operator>>(std::istream& o, BigInt& i);
