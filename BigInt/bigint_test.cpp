#include <string>
#include <utility>
#include "bigint.h"
#include "gtest/gtest.h"

class BigIntTest : public testing::Test {
protected:
    BigInt a, b;
};

TEST_F(BigIntTest, Test_of_Comparison_Operators) {
    a = BigInt(0);
    b = BigInt(2);

    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a == b);

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != a);

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(a < a);

    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= a);

    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a > a);

    EXPECT_FALSE(a >= b);
    EXPECT_TRUE(a >= a);
}

TEST_F(BigIntTest, Test_of_Unary_Operators_with_Positive_Number) {
    a = BigInt(123456);

    EXPECT_EQ(~a, BigInt(-123457));

    EXPECT_EQ(-a, BigInt(-123456));

    EXPECT_EQ(+a, BigInt(123456));
}

TEST_F(BigIntTest, Test_of_Unary_Operators_with_Negative_Number) {
    std::string a_str = "-9559486967043264371284761783";
    BigInt a(a_str);

    std::string inv = "9559486967043264371284761782";
    std::string u_minus = "9559486967043264371284761783";

    EXPECT_EQ(~a, BigInt(inv));

    EXPECT_EQ(-a, BigInt(u_minus));

    EXPECT_EQ(+a, BigInt(a_str));
}

TEST_F(BigIntTest, Test_of_Binary_Operators_with_Positive_Numbers) {
    a = BigInt(123456);
    b = BigInt(7743234);

    std::string mul = "955948696704";

    EXPECT_EQ(a + b, BigInt(7866690));

    EXPECT_EQ(a - b, BigInt(-7619778));
    EXPECT_EQ(b - a, BigInt(7619778));

    EXPECT_EQ(a * b, BigInt(mul));

    EXPECT_EQ(a / b, BigInt(0));
    EXPECT_EQ(b / a, BigInt(62));

    EXPECT_EQ(a % b, BigInt(123456));
    EXPECT_EQ(b % a, BigInt(88962));

    EXPECT_EQ(a ^ b, BigInt(7849282));

    EXPECT_EQ(a & b, BigInt(8704));

    EXPECT_EQ(a | b, BigInt(7857986));
}

TEST_F(BigIntTest, Test_of_Binary_Operators_with_Positive_and_Negative_Numbers) {
    a = BigInt(-3891);
    std::string b_str = "192038712312635613123312312668221";
    b = BigInt(b_str);

    std::string add = "192038712312635613123312312664330";
    std::string sub1 = "-192038712312635613123312312672112";
    std::string sub2 = "192038712312635613123312312672112";
    std::string mul = "-747222629608465170662808208592047911";
    std::string div2 = "-49354590674026114912185122762";
    std::string xor_ = "-192038712312635613123312312672016";
    std::string and_ = "192038712312635613123312312668173";

    EXPECT_EQ(a + b, BigInt(add));

    EXPECT_EQ(a - b, BigInt(sub1));
    EXPECT_EQ(b - a, BigInt(sub2));

    EXPECT_EQ(a * b, BigInt(mul));

    EXPECT_EQ(a / b, BigInt(0));
    EXPECT_EQ(b / a, BigInt(div2));

    EXPECT_EQ(a % b, BigInt(-3891));
    EXPECT_EQ(b % a, BigInt(1279));

    EXPECT_EQ(a ^ b, BigInt(xor_));

    EXPECT_EQ(a & b, BigInt(and_));

    EXPECT_EQ(a | b, BigInt(-3843));
}

TEST_F(BigIntTest, Test_of_Binary_Operators_with_Negative_Numbers) {
    a = BigInt(-22);
    b = BigInt(-44);

    EXPECT_EQ(a + b, BigInt(-66));

    EXPECT_EQ(a - b, BigInt(22));
    EXPECT_EQ(b - a, BigInt(-22));

    EXPECT_EQ(a * b, BigInt(968));

    EXPECT_EQ(a / b, BigInt(0));
    EXPECT_EQ(b / a, BigInt(2));

    EXPECT_EQ(a % b, BigInt(-22));
    EXPECT_EQ(b % a, BigInt(0));

    EXPECT_EQ(a ^ b, BigInt(62));

    EXPECT_EQ(a & b, BigInt(-64));

    EXPECT_EQ(a | b, BigInt(-2));
}

TEST_F(BigIntTest, Test_of_Binary_Operators_with_Zero_and_Positive_Number) {
    a = BigInt(0);
    b = BigInt(2);

    EXPECT_EQ(a + b, BigInt(2));

    EXPECT_EQ(a - b, BigInt(-2));
    EXPECT_EQ(b - a, BigInt(2));

    EXPECT_EQ(a * b, BigInt(0));

    EXPECT_EQ(a / b, BigInt(0));
    EXPECT_ANY_THROW(b / a);

    EXPECT_EQ(a % b, BigInt(0));
    EXPECT_ANY_THROW(b % a);

    EXPECT_EQ(a ^ b, BigInt(2));

    EXPECT_EQ(a & b, BigInt(0));

    EXPECT_EQ(a | b, BigInt(2));
}

TEST_F(BigIntTest, Test_of_Binary_Operators_with_Zero_and_Negative_Number) {
    a = BigInt(0);
    b = BigInt(-2);

    EXPECT_EQ(a + b, BigInt(-2));

    EXPECT_EQ(a - b, BigInt(2));
    EXPECT_EQ(b - a, BigInt(-2));

    EXPECT_EQ(a * b, BigInt(0));

    EXPECT_EQ(a / b, BigInt(0));
    EXPECT_ANY_THROW(b / a);

    EXPECT_EQ(a % b, BigInt(0));
    EXPECT_ANY_THROW(b % a);

    EXPECT_EQ(a ^ b, BigInt(-2));

    EXPECT_EQ(a & b, BigInt(0));

    EXPECT_EQ(a | b, BigInt(-2));
}

TEST_F(BigIntTest, Test_of_Copy_and_Move_Assignment_Operators) {
    a = BigInt(123456);
    b = BigInt(7743234);

    a = b;
    EXPECT_TRUE(a == b);
    EXPECT_TRUE((a = a) == a);
    a = BigInt(123456);

    a = std::move(b);
    b = BigInt(7743234);
    EXPECT_TRUE(a == b);
}

TEST_F(BigIntTest, Test_of_Increment_and_Decrement_Operators_with_Positive_Number) {
    a = BigInt(123456);

    EXPECT_EQ(++a, BigInt(123457));
    EXPECT_EQ(a++, BigInt(123458));

    EXPECT_EQ(--a, BigInt(123457));
    EXPECT_EQ(a--, BigInt(123456));
}

TEST_F(BigIntTest, Test_of_Increment_and_Decrement_Operators_with_Negative_Number) {
    a = BigInt(-3891);

    EXPECT_EQ(++a, BigInt(-3890));
    EXPECT_EQ(a++, BigInt(-3889));

    EXPECT_EQ(--a, BigInt(-3890));
    EXPECT_EQ(a--, BigInt(-3891));
}

TEST_F(BigIntTest, Test_of_Assignment_Operators_with_Positive_Numbers) {
    a = BigInt(123456);
    b = BigInt(7743234);

    std::string mul = "955948696704";

    EXPECT_EQ(a += b, BigInt(7866690));
    a = BigInt(123456);
    EXPECT_EQ(b += a, BigInt(7866690));
    b = BigInt(7743234);

    EXPECT_EQ(a -= b, BigInt(-7619778));
    a = BigInt(123456);
    EXPECT_EQ(b -= a, BigInt(7619778));
    b = BigInt(7743234);

    EXPECT_EQ(a *= b, BigInt(mul));
    a = BigInt(123456);
    EXPECT_EQ(b *= a, BigInt(mul));
    b = BigInt(7743234);

    EXPECT_EQ(a /= b, BigInt(0));
    a = BigInt(123456);
    EXPECT_EQ(b /= a, BigInt(62));
    b = BigInt(7743234);

    EXPECT_EQ(a %= b, BigInt(123456));
    a = BigInt(123456);
    EXPECT_EQ(b %= a, BigInt(88962));
    b = BigInt(7743234);

    EXPECT_EQ(a ^= b, BigInt(7849282));
    a = BigInt(123456);
    EXPECT_EQ(b ^= a, BigInt(7849282));
    b = BigInt(7743234);

    EXPECT_EQ(a &= b, BigInt(8704));
    a = BigInt(123456);
    EXPECT_EQ(b &= a, BigInt(8704));
    b = BigInt(7743234);

    EXPECT_EQ(a |= b, BigInt(7857986));
    a = BigInt(123456);
    EXPECT_EQ(b |= a, BigInt(7857986));
}

TEST_F(BigIntTest, Test_of_Assignment_Operators_with_Positive_and_Negative_Numbers) {
    a = BigInt(-3891);
    std::string b_str = "192038712312635613123312312668221";
    b = BigInt(b_str);

    std::string add = "192038712312635613123312312664330";
    std::string sub1 = "-192038712312635613123312312672112";
    std::string sub2 = "192038712312635613123312312672112";
    std::string mul = "-747222629608465170662808208592047911";
    std::string div2 = "-49354590674026114912185122762";
    std::string xor_ = "-192038712312635613123312312672016";
    std::string and_ = "192038712312635613123312312668173";

    EXPECT_EQ(a += b, BigInt(add));
    a = BigInt(-3891);
    EXPECT_EQ(b += a, BigInt(add));
    b = BigInt(b_str);

    EXPECT_EQ(a -= b, BigInt(sub1));
    a = BigInt(-3891);
    EXPECT_EQ(b -= a, BigInt(sub2));
    b = BigInt(b_str);

    EXPECT_EQ(a *= b, BigInt(mul));
    a = BigInt(-3891);
    EXPECT_EQ(b *= a, BigInt(mul));
    b = BigInt(b_str);

    EXPECT_EQ(a /= b, BigInt(0));
    a = BigInt(-3891);
    EXPECT_EQ(b /= a, BigInt(div2));
    b = BigInt(b_str);

    EXPECT_EQ(a %= b, BigInt(-3891));
    a = BigInt(-3891);
    EXPECT_EQ(b %= a, BigInt(1279));
    b = BigInt(b_str);

    EXPECT_EQ(a ^= b, BigInt(xor_));
    a = BigInt(-3891);
    EXPECT_EQ(b ^= a, BigInt(xor_));
    b = BigInt(b_str);

    EXPECT_EQ(a &= b, BigInt(and_));
    a = BigInt(-3891);
    EXPECT_EQ(b &= a, BigInt(and_));
    b = BigInt(b_str);

    EXPECT_EQ(a |= b, BigInt(-3843));
    a = BigInt(-3891);
    EXPECT_EQ(b |= a, BigInt(-3843));
}

TEST_F(BigIntTest, Test_of_Assignment_Operators_with_Negative_Numbers) {
    a = BigInt(-22);
    b = BigInt(-44);

    EXPECT_EQ(a += b, BigInt(-66));
    a = BigInt(-22);
    EXPECT_EQ(b += a, BigInt(-66));
    b = BigInt(-44);

    EXPECT_EQ(a -= b, BigInt(22));
    a = BigInt(-22);
    EXPECT_EQ(b -= a, BigInt(-22));
    b = BigInt(-44);

    EXPECT_EQ(a *= b, BigInt(968));
    a = BigInt(-22);
    EXPECT_EQ(b *= a, BigInt(968));
    b = BigInt(-44);

    EXPECT_EQ(a /= b, BigInt(0));
    a = BigInt(-22);
    EXPECT_EQ(b /= a, BigInt(2));
    b = BigInt(-44);

    EXPECT_EQ(a %= b, BigInt(-22));
    a = BigInt(-22);
    EXPECT_EQ(b %= a, BigInt(0));
    b = BigInt(-44);

    EXPECT_EQ(a ^= b, BigInt(62));
    a = BigInt(-22);
    EXPECT_EQ(b ^= a, BigInt(62));
    b = BigInt(-44);

    EXPECT_EQ(a &= b, BigInt(-64));
    a = BigInt(-22);
    EXPECT_EQ(b &= a, BigInt(-64));
    b = BigInt(-44);

    EXPECT_EQ(a |= b, BigInt(-2));
    a = BigInt(-22);
    EXPECT_EQ(b |= a, BigInt(-2));
}

TEST_F(BigIntTest, Test_of_Type_Casting_Operators) {
    a = BigInt(122123);
    int a_int = (int)a;
    EXPECT_EQ(a_int, 122123);

    std::string s = "918237128394876738475671231";
    a = BigInt(s);
    std::string a_string = (std::string)a;
    EXPECT_EQ(a_string, s);
}

TEST_F(BigIntTest, Test_of_Function_SIZE) {
    std::string s = "1234567890123456789012345678901234567890";
    a = BigInt(s);
    EXPECT_EQ(a.size(), 41);

    b = BigInt(-1231);
    EXPECT_EQ(b.size(), 5);
}

TEST_F(BigIntTest, Test_of_Input_and_Output_Operators) {
    std::stringstream o;
    std::string a_string = "1231231";
    a = BigInt(a_string);
    o << a;
    std::string s;
    o >> s;
    EXPECT_EQ(a, BigInt(s));

    o << s;
    o >> a;
    EXPECT_EQ(a, BigInt(s));
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}