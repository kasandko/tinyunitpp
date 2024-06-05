/**************************************************************************************************
 * @file
 * @author Konovalov Aleksander
 * @copyright This file is part of library TinyUnit++ and has the same license. @see LICENSE.txt
 * @brief It is main include file.
 * 
 * This file contains macros for checking values and class with static methods for provide
 * test system functionals.
 *************************************************************************************************/

#ifndef _TUPP_HPP_
#define _TUPP_HPP_

#include <string>
#include <functional>

/// @brief Macro for checking the equality of two variables.
#ifdef TUPP_ASSERT
#undef TUPP_ASSERT
#endif
#define TUPP_ASSERT(V_A, V_B, ...)                                                  \
    tupp::t_assert(V_A == V_B, #V_A  " == "  #V_B, __LINE__, ##__VA_ARGS__)

/// @brief Macro for checking the inequality of two variables.
#ifdef TUPP_N_ASSERT
#undef TUPP_N_ASSERT
#endif
#define TUPP_N_ASSERT(V_A, V_B, ...)                                                \
    tupp::t_assert(V_A != V_B, #V_A  " != "  #V_B, __LINE__, ##__VA_ARGS__)

/// @brief Macro for check that a variable is true.
#ifdef TUPP_ASSERT_TRUE
#undef TUPP_ASSERT_TRUE
#endif
#define TUPP_ASSERT_TRUE(V, ...)                                                    \
    tupp::t_assert_tf(V, true, #V, __LINE__, ##__VA_ARGS__)

/// @brief Macro for check that a variable is false.
#ifdef TUPP_ASSERT_FALSE
#undef TUPP_ASSERT_FALSE
#endif
#define TUPP_ASSERT_FALSE(V, ...)                                                   \
    tupp::t_assert_tf(V, false, #V, __LINE__, ##__VA_ARGS__)

/// @brief Macro for print a some message.
#ifdef TUPP_MESSAGE
#undef TUPP_MESSAGE
#endif
#define TUPP_MESSAGE(MSG)                                                           \
    tupp::message(MSG, __LINE__)

/// @brief Macro for checking the equality of two float variables.
///
/// The accuracy of the verification is contained in @see tupp::FLOAT_CHECKING_ACCURACY.
#ifdef TUPP_ASSERT_F
#undef TUPP_ASSERT_F
#endif
#define TUPP_ASSERT_F(V_A, V_B, ...)                                                \
    tupp::t_assert_flt(V_A, V_B, #V_A " == " #V_B, __LINE__, ##__VA_ARGS__)

/// @brief Macro for test addition.
/// @param TEST_NAME - This is a test function name. It is also will be used as test name.
#ifdef TUPP_ADD_TEST
#undef TUPP_ADD_TEST
#endif
#define TUPP_ADD_TEST(TEST_NAME)                                                    \
    tupp::add_test(&TEST_NAME, #TEST_NAME)

class tupp_internal;

/// @brief Main class with functional for make unit tests.
class tupp
{
public:

    static constexpr float FLOAT_CHECKING_ACCURACY = 0.00001f;

    tupp(const tupp &) = delete;
    tupp(const tupp &&) = delete;
    tupp & operator=(tupp &) = delete;
    tupp & operator=(tupp &&) = delete;

    using TestFunc = std::function<void()>;

    /// @brief Method for run tests.
    /// @param argc - count of command line arguments.
    /// @param argv - array of size `argc` with command line arguments.
    /// @return %ERRORLEVEL% - result of passing tests and works this program.
    ///     0 - successful completion of tests,
    ///     1 - one or more tests are fails,
    ///     100 - bad command line arguments,
    ///     101 - error "invalid key",
    ///     102 - error "invalid key usage".
    static int run(int argc, char* argv[]);

    /// @brief Method for show message.
    /// @param msg - displayed message.
    /// @param line - code line number.
    ///
    /// This method is used by `TUPP_MESSAGE` macro.
    static void message(const std::string & msg, size_t line);

    /// @brief Method for test addition.
    /// @param test_func - pointer of test function. Signature of this function: @see TestFunc.
    /// @param name - test name.
    ///
    /// This method is used by `TUPP_ADD_TEST` macro.
    static void add_test(const TestFunc & test_func, const std::string & name);

    /// @brief Method for check that `v` is not false. Used for assertion macros.
    /// @param v - result of a test passing. Assertion is activated if this paramenter is `false`.
    /// @param msg - displayed message. This message field used for auto generating and displayed
    ///     checked code string.
    /// @param line - code line number.
    /// @param additionals - additionals displayed message parts. This parameters for user defined
    ///     messages.
    ///
    /// This method is used by `TUPP_ASSERT` and `TUPP_N_ASSERT` macros.
    template <typename... TMsg>
    static void t_assert(bool v, const std::string & msg, size_t line, const TMsg & ... additionals)
    {
        assert(v, msg, make_additional(additionals...), line);
    }

    /// @brief Method for check that `v` is equal `expected`. Used for boolean assertion macros.
    /// @param v - passed tested boolean value.
    /// @param expected - expected value.
    /// @param msg displayed message. This message field used for auto generating and displayed
    ///     checked code string.
    /// @param line - code line number.
    /// @param additionals - additionals displayed message parts. This parameters for user defined
    ///     messages.
    ///
    /// Assertion is activated if `v` is not equal `expected`. This method is used by
    /// `TUPP_ASSERT_TRUE` and `TUPP_ASSERT_FALSE` macros.
    template <typename... TMsg>
    static void t_assert_tf(bool v, bool expected, const std::string & msg, size_t line, const TMsg & ... additionals)
    {
        assert_tf(v, expected, msg, make_additional(additionals...), line);
    }

    /// @brief Method for check equality of two float variables. Used for float assertion macros.
    /// @param a - first tested variable.
    /// @param b - second tested variable.
    /// @param msg displayed message. This message field used for auto generating and displayed
    ///     checked code string.
    /// @param line - code line number.
    /// @param additionals - additionals displayed message parts. This parameters for user defined
    ///     messages.
    ///
    /// Assertion is activated if `a` is different from `b` by `FLOAT_CHECKING_ACCURACY`.
    template <typename... TMsg>
    static void t_assert_flt(float a, float b, const std::string & msg, size_t line, const TMsg & ... additionals)
    {
        assert_flt(a, b, msg, make_additional(additionals...), line);
    }

private:

    static void assert(bool v, const std::string & msg, const std::string & additional_msg, size_t line);
    static void assert_tf(bool v, bool expected, const std::string & msg, const std::string & additional_msg, size_t line);
    static void assert_flt(float a, float b, const std::string & msg, const std::string & additional_msg, size_t line);

    template <typename... TMsg>
    static std::string make_additional(const TMsg & ... additionals)
    {
        if constexpr (sizeof... (additionals) > 0u)
            return (... + std::string(additionals));
        else
            return {};
    }

    static tupp_internal * internal();
    static tupp & instance();

    tupp();
    ~tupp();

    tupp_internal * _internal;
};

#endif // _TUPP_HPP_
