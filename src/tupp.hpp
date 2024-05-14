#ifndef _TUPP_HPP_
#define _TUPP_HPP_

#include <string>
#include <functional>

#ifdef TUPP_ASSERT
#undef TUPP_ASSERT
#endif
#define TUPP_ASSERT(V_A, V_B, ...)                                                  \
    tupp::t_assert(V_A == V_B, #V_A ## " == " ## #V_B, __LINE__, __VA_ARGS__);

#ifdef TUPP_N_ASSERT
#undef TUPP_N_ASSERT
#endif
#define TUPP_N_ASSERT(V_A, V_B, MSG, ...)                                           \
    tupp::t_assert(V_A != V_B, #V_A ## " != " ## #V_B, __LINE__, __VA_ARGS__);

#ifdef TUPP_ASSERT_TRUE
#undef TUPP_ASSERT_TRUE
#endif
#define TUPP_ASSERT_TRUE(V, MSG, ...)                                               \
    tupp::t_assert_tf(V, true, #V, __LINE__, __VA_ARGS__);

#ifdef TUPP_ASSERT_FALSE
#undef TUPP_ASSERT_FALSE
#endif
#define TUPP_ASSERT_FALSE(V, MSG, ...)                                              \
    tupp::t_assert_tf(V, false, #V, __LINE__, __VA_ARGS__);

#ifdef TUPP_MESSAGE
#undef TUPP_MESSAGE
#endif
#define TUPP_MESSAGE(MSG)                                                           \
    tupp::message(MSG, __LINE__)

#ifdef TUPP_ASSERT_F
#undef TUPP_ASSERT_F
#endif
#define TUPP_ASSERT_F(V_A, V_B, MSG, ...)                                           \
    tupp::t_assert_flt(V_A, V_B, #V_A ## " == " ## #V_B, __LINE__, __VA_ARGS__);

class tupp_internal;

class tupp
{
public:

    using TestFunc = std::function<void()>;

    static int run(int argc, char* argv[]);

    static void assert(bool v, const std::string & msg, const std::string & additional_msg, size_t line);
    static void assert_tf(bool v, bool expected, const std::string & msg, const std::string & additional_msg, size_t line);
    static void assert_flt(float a, float b, const std::string & msg, const std::string & additional_msg, size_t line);
    static void message(const std::string & msg, size_t line);

    static void add_test(const TestFunc & test_func, const std::string & name);

    template <typename... TMsg>
    static std::string make_additional(const TMsg & ... additionals)
    {
        std::string additional;
        additional += additionals...;
        return additional;
    }

    template <typename... TMsg>
    static void t_assert(bool v, const std::string & msg, size_t line, const TMsg & ... additionals)
    {
        assert(v, msg, make_additional(additionals...), line);
    }

    template <typename... TMsg>
    static void t_assert_tf(bool v, bool expected, const std::string & msg, size_t line, const TMsg & ... additionals)
    {
        assert_tf(v, expected, msg, make_additional(additionals...), line);
    }

    template <typename... TMsg>
    static void t_assert_flt(float a, float b, const std::string & msg, size_t line, const TMsg & ... additionals)
    {
        assert_flt(a, b, msg, make_additional(additionals...), line);
    }

private:

    static tupp_internal * internal();
    static tupp & instance();

    tupp();
    ~tupp();
    tupp(const tupp &) = delete;
    tupp(const tupp &&) = delete;
    tupp & operator=(tupp &) = delete;
    tupp & operator=(tupp &&) = delete;

    tupp_internal * _internal;
};

#endif // _TUPP_HPP_
