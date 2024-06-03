#include "tupp.hpp"

void test_fails()
{
    int a = 5;
    int b = 8;
    TUPP_ASSERT(a, b);

    int c = 8;
    int d = 8;
    TUPP_N_ASSERT(c, d);

    float e = 7.0f;
    float f = 9.0f;
    TUPP_ASSERT_F(e, f);

    bool g = true;
    bool h = false;
    TUPP_ASSERT_FALSE(g);
    TUPP_ASSERT_TRUE(h);
}

void test_success()
{
    TUPP_MESSAGE("abcde");

    int a = 5;
    int b = 5;
    TUPP_ASSERT(a, b);

    int c = 8;
    int d = 9;
    TUPP_N_ASSERT(c, d);

    float e = 9.0f;
    float f = 9.0f;
    TUPP_ASSERT_F(e, f);

    bool g = false;
    bool h = true;
    TUPP_ASSERT_FALSE(g);
    TUPP_ASSERT_TRUE(h);
}

void test_fails_2()
{
    int a = 5;
    int b = 8;
    TUPP_ASSERT(a, b);
}

int main(int argc, char* argv[])
{
    TUPP_ADD_TEST(test_fails);
    TUPP_ADD_TEST(test_success);
    TUPP_ADD_TEST(test_fails_2);
    return tupp::run(argc, argv);
}
