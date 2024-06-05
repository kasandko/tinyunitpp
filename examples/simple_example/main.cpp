#include "tupp.hpp"

void example_test_assert()
{
    int a = 5;
    int b = 5;
    int c = 6;

    TUPP_MESSAGE("Example assert.");
    TUPP_ASSERT(a, b);
    TUPP_N_ASSERT(a, c);
}

void example_test_float()
{
    float a = 5.0f;
    float b = 5.0f;

    TUPP_MESSAGE("Example float.");
    TUPP_ASSERT_F(a, b);
}

void example_test_bool()
{
    bool a = true;
    bool b = false;

    TUPP_MESSAGE("Example bool.");
    TUPP_ASSERT_TRUE(a);
    TUPP_ASSERT_FALSE(b);
}

int main(int argc, char* argv[])
{
    TUPP_ADD_TEST(example_test_assert);
    TUPP_ADD_TEST(example_test_float);
    TUPP_ADD_TEST(example_test_bool);

    return tupp::run(argc, argv);
}
