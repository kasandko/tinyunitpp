/**************************************************************************************************
 * @file
 * @author Konovalov Aleksander
 * @copyright This file is part of library TinyUnit++ and has the same license. @see LICENSE.txt
 * @brief It is main include file.
 *
 * This file contains TinyUnit++ functional implementations.
 *************************************************************************************************/

#include "tupp.hpp"

#include <iostream>
#include <cmath>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <exception>

const std::string PROG_NAME = "TinyUnit++";
const unsigned int PROG_VER_MAJ = 0u;
const unsigned int PROG_VER_MIN = 2u;
const unsigned int PROG_VER_COR = 0u;
const std::string PROG_COPYRIGHT = "Copyright (c) 2024: Konovalov Aleksander, BSD-2-Clause license.";
const std::string PROG_URL = "https://github.com/kasandko/tinyunitpp";

struct case_params
{
    case_params() = default;

    std::optional<size_t> line;
    std::string msg;
    std::string additional_msg;
    std::string expected;
    std::optional<bool> assert_result;

    bool has_msg() const { return !msg.empty(); }
    bool has_additional() const { return !additional_msg.empty(); }
    bool has_expected() const { return !expected.empty(); }
    bool has_result() const { return assert_result.has_value(); }
    bool get_result() const { return assert_result.value_or(false); }
};

struct status
{
    enum class status_code
    {
        _UNDEFINED,
        SUCCESS,
        INVALID_KEY,
        INVALID_KEY_USAGE,
        TESTS_FAIL,
        BAD_CMD,
        RUN_HELP,
        RUN_VERSION,
        RUN_TEST,
    };

    status() = delete;
    status(status_code code, const std::string & msg)
        : code(code)
        , msg(msg)
    {
    }

    status(status_code code)
        : code(code)
    {
    }

    status_code code = status_code::_UNDEFINED;
    std::string msg;
};

class tupp_exception : public std::exception
{
public:
    tupp_exception() = default;
};

class tupp_internal
{
public:

    tupp_internal() = default;

    void set_line(size_t line);
    void set_msg(const std::string & msg);
    void set_additional_msg(const std::string & additional_msg);
    void set_expected(const std::string & expected);
    void assert(bool v);
    void apply_case();
    int run(int argc, char* argv[]);

    void add_test(const std::function<void()> & func, const std::string & name);

    void clear_case_params()
    {
        _case_params = std::nullopt;
    }

private:

    enum class message_type
    {
        DEFAULT,
        TEST_NAME,
        TEST_MESSAGE,
        HEADER,
        FAIL,
        SUCCESS,
        REPORT,
    };

    void init_case_params();
    status parse_cmd(int argc, char* argv[]);
    static std::string parse_cmd_key(const std::string & arg);
    status apply_cmd_params();
    int handle_status(const status & st);
    void print(message_type type, const std::string & message, bool cr) const;
    static std::string make_tab(size_t tabs_count);

    static void show_help();
    static void show_version();
    static void show_error(const std::string & error_msg);
    int run_tests();

private:

    std::optional<case_params> _case_params;
    std::vector<std::string> _case_messages;
    bool _case_result = true;

    std::unordered_map<std::string, std::vector<std::string>> _cmd_params;
    std::unordered_set<std::string> _tests_to_run;
    std::unordered_map<std::string, std::function<void()>> _all_tests;

    struct
    {
        bool hide_all_messages = false;
        bool hide_header = false;
        bool hide_report = false;
        bool hide_test_messages = false;
        bool hide_test_names = false;
        bool continue_after_assert = false;
    } _config;
};


void tupp_internal::set_line(size_t line)
{
    init_case_params();
    _case_params->line = line;
}

void tupp_internal::set_msg(const std::string & msg)
{
    init_case_params();
    _case_params->msg = msg;
}

void tupp_internal::set_additional_msg(const std::string & additional_msg)
{
    init_case_params();
    _case_params->additional_msg = additional_msg;
}

void tupp_internal::set_expected(const std::string & expected)
{
    init_case_params();
    _case_params->expected = expected;
}

void tupp_internal::assert(bool v)
{
    _case_params->assert_result = v;
}

void tupp_internal::apply_case()
{
    if (!_case_params)
        return;

    struct cleaner
    {
        tupp_internal * internal;
        explicit cleaner(tupp_internal * internal) : internal(internal) {}
        ~cleaner() { internal->clear_case_params(); }
    } obj_cleaner(this);

    std::string msg;
    if (!_case_params->has_result())
    {
        if (_case_params->has_msg())
            msg += _case_params->msg;

        if (_case_params->has_additional())
        {
            if (!msg.empty())
                msg += " ";
            msg += _case_params->additional_msg;
        }

        if (msg.empty())
            return;

        msg = "Message: " + msg;
    }
    else
    {
        if (_case_params->get_result())
            return;

        _case_result = false;

        msg = "Fail";
        if (_case_params->has_msg())
            msg += " " + _case_params->msg;

        if (_case_params->has_expected())
            msg += " (expected: " + _case_params->expected + ")";

        if (_case_params->has_additional())
            msg += ": " + _case_params->additional_msg;
    }

    if (_case_params->line)
        msg += " Line: " + std::to_string(*_case_params->line);

    _case_messages.push_back(msg);

    if (_case_params->has_result() && !_config.continue_after_assert)
        throw tupp_exception();
}

int tupp_internal::run(int argc, char* argv[])
{
    status st = parse_cmd(argc, argv);
    if (st.code != status::status_code::SUCCESS)
        return handle_status(st);

    st = apply_cmd_params();
    return handle_status(st);
}

void tupp_internal::add_test(const std::function<void()> & func, const std::string & name)
{
    _all_tests.emplace(name, func);
}

void tupp_internal::init_case_params()
{
    if (!_case_params)
        _case_params = case_params();
}

status tupp_internal::parse_cmd(int argc, char* argv[])
{
    std::string key;
    for (int i = 1 /* Skip exe file. */; i < argc; ++i)
    {
        std::string s_argv(argv[i]);
        if (const std::string new_key = parse_cmd_key(s_argv); !new_key.empty())
        {
            if (!key.empty())
                _cmd_params.emplace(key, std::vector<std::string>());
            key = new_key;
        }
        else
        {
            if (key.empty())
                return {status::status_code::BAD_CMD, "Invalid key: '" + s_argv + "'"};

            auto it = _cmd_params.find(key);
            if (it == _cmd_params.end())
                it = _cmd_params.emplace(key, std::vector<std::string>()).first;
            it->second.push_back(s_argv);
        }
    }

    if (!key.empty())
        _cmd_params.emplace(key, std::vector<std::string>());

    return {status::status_code::SUCCESS};
}

std::string tupp_internal::parse_cmd_key(const std::string & arg)
{
    if (arg.empty())
        return {};

    if (arg.size() == 2u && arg[0u] == '-' && arg[1u] != '-')
        return std::string(1u, arg[1u]);

    if (arg.size() > 2u && arg[0u] == '-' && arg[1u] == '-')
        return arg.substr(2u);

    return {};
}

status tupp_internal::apply_cmd_params()
{
    status::status_code result = status::status_code::_UNDEFINED;
    for (const auto & [key, values] : _cmd_params)
    {
        if (key == "h" || key == "help")
        {
            return result == status::status_code::_UNDEFINED
                ? status(status::status_code::RUN_HELP)
                : status(status::status_code::INVALID_KEY_USAGE, "Can not use '" + key + "' in this context");
        }

        if (key == "v" || key == "version")
        {
            return result == status::status_code::_UNDEFINED
                ? status(status::status_code::RUN_VERSION)
                : status(status::status_code::INVALID_KEY_USAGE, "Can not use '" + key + "' in this context");
        }

        if (key == "t" || key == "test")
        {
            for (const auto & value : values)
                _tests_to_run.emplace(value);
        }
        else if (key == "s" || key == "silent")
        {
            if (values.empty())
                return {status::status_code::INVALID_KEY, "Needed value for '" + key + "'"};

            if (values.size() > 1u)
                return {status::status_code::INVALID_KEY_USAGE, "Multiple use of key '" + key + "'"};

            int silent_lvl;
            try
            {
                size_t pos;
                silent_lvl = std::stoi(values[0u], &pos);
            }
            catch(const std::exception &)
            {
                return {status::status_code::INVALID_KEY, "Invalid value for '" + key + "'"};
            }

            if (silent_lvl == 1000)
            {
                _config.hide_all_messages = true;
            }
            else
            {
                if (silent_lvl / 100 == 1)
                    _config.hide_report = true;

                silent_lvl %= 100;

                if (silent_lvl / 10 == 1)
                    _config.hide_header = true;

                silent_lvl %= 10;

                if (silent_lvl == 1)
                    _config.hide_test_messages = true;
                if (silent_lvl == 2)
                {
                    _config.hide_test_messages = true;
                    _config.hide_test_names = true;
                }
            }
        }
        else if (key == "a" || key == "continue_after_assert")
        {
            if (!values.empty())
                return {status::status_code::INVALID_KEY, "Key '" + key + "' doesn't have any value"};
            _config.continue_after_assert = true;
        }
        else
        {
            return {status::status_code::INVALID_KEY, "Unknown key '" + key + "'"};
        }

        result = status::status_code::RUN_TEST;
    }

    if (result == status::status_code::_UNDEFINED)
        result = status::status_code::RUN_TEST;

    return result;
}

void tupp_internal::print(message_type type, const std::string & message, bool cr) const
{
    if (_config.hide_all_messages)
        return;

    std::string msg;
    switch (type)
    {
    case message_type::FAIL:
    case message_type::SUCCESS:
        if (_config.hide_test_names)
            return;
        msg = message;
        break;

    case message_type::TEST_NAME:
        if (_config.hide_test_names)
            return;
        msg = make_tab(1u) + message;
        break;

    case message_type::HEADER:
        if (_config.hide_header)
            return;
        msg = "#### " + message + " ####";
        break;

    case message_type::TEST_MESSAGE:
        if (_config.hide_test_messages)
            return;
        msg = make_tab(2u) + message;
        break;

    case message_type::REPORT:
        if (_config.hide_report)
            return;
        msg = make_tab(1u) + message;
        break;

    case message_type::DEFAULT:
    default:
        msg = message;
        break;
    }

    std::cout << msg;
    if (cr)
        std::cout << std::endl;
}

std::string tupp_internal::make_tab(size_t tabs_count)
{
    static constexpr size_t TAB_SIZE = 2u;
    return std::string(tabs_count * TAB_SIZE, ' ');
}

int tupp_internal::handle_status(const status & st)
{
    switch (st.code)
    {
    case status::status_code::BAD_CMD:
        show_error(st.msg);
        return 100;
    case status::status_code::INVALID_KEY:
        show_error(st.msg);
        return 101;
    case status::status_code::INVALID_KEY_USAGE:
        show_error(st.msg);
        return 102;
    case status::status_code::RUN_HELP:
        show_help();
        return 0;
    case status::status_code::RUN_TEST:
        return run_tests();
    case status::status_code::RUN_VERSION:
        show_version();
        return 0;
    case status::status_code::SUCCESS:
        return 0;
    case status::status_code::_UNDEFINED:
        show_error("Unknown error");
        return 200;
    case status::status_code::TESTS_FAIL:
        return 1;
    default:
        return 0;
    }
}

void tupp_internal::show_help()
{
    std::cout << "-a --continue_after_assert  Continue to run test after fail assert." << std::endl;
    std::cout << "-h --help                   Show this help." << std::endl;
    std::cout << "-s --silent_level [level]   Hide some messages during tests execute." << std::endl;
    std::cout << "                              level:" << std::endl;
    std::cout << "                              0: Show all messages." << std::endl;
    std::cout << "                              1: Hide test messages" << std::endl;
    std::cout << "                              2: Hide test messages and test results." << std::endl;
    std::cout << "                              +10: Hide headers." << std::endl;
    std::cout << "                              +100: Hide report." << std::endl;
    std::cout << "                              1000: Hide all messages." << std::endl;
    std::cout << "                              Example: 11 - hide test messages and headers." << std::endl;
    std::cout << "-t --test [test_name]       Execute test with specified name." << std::endl;
    std::cout << "-v --version                Show version." << std::endl;
}

void tupp_internal::show_version()
{
    std::cout << PROG_NAME
        << " v" << PROG_VER_MAJ << "." << PROG_VER_MIN << "." << PROG_VER_COR << std::endl;
    std::cout << PROG_COPYRIGHT << std::endl;
    std::cout << PROG_URL << std::endl;
}

void tupp_internal::show_error(const std::string & error_msg)
{
    std::cout << PROG_NAME << std::endl;
    std::cout << error_msg << std::endl;
}

int tupp_internal::run_tests()
{
    print(message_type::HEADER, "START", true);
    size_t failed_count = 0u;
    for (const auto & [test_name, test_func] : _all_tests)
    {
        if (_tests_to_run.count(test_name) > 0u || _tests_to_run.empty())
        {
            print(message_type::TEST_NAME, "TEST '" + test_name + "': ", false);
            try
            {
                _case_params = std::nullopt;
                test_func();
            }
            catch (const tupp_exception &)
            {
                // Nothing do.
            }
            catch (const std::exception & e)
            {
                const std::string msg = "  Exception: " + std::string(e.what());
                _case_messages.push_back(msg);
                _case_result = false;
            }

            if (_case_result)
            {
                print(message_type::SUCCESS, "SUCCESS", true);
            }
            else
            {
                print(message_type::FAIL, "FAIL", true);
                ++failed_count;
            }

            for (const std::string & msg : _case_messages)
                print(message_type::TEST_MESSAGE, msg, true);

            _case_messages.clear();
            _case_result = true;
        }
    }

    print(message_type::HEADER, "FINISH", true);
    const size_t tests_count = _tests_to_run.empty() ? _all_tests.size() : _tests_to_run.size();
    std::string report_msg;
    report_msg += "Run: " + std::to_string(tests_count) + "/" + std::to_string(_all_tests.size());
    report_msg += ", Fail: " + std::to_string(failed_count);
    report_msg += ", Pass: " + std::to_string(tests_count - failed_count);
    print(message_type::REPORT, report_msg, true);

    if (failed_count > 0u)
        return handle_status(status(status::status_code::TESTS_FAIL));
    else
        return handle_status(status(status::status_code::SUCCESS));
}


tupp::tupp()
    : _internal(new tupp_internal())
{
}

tupp::~tupp()
{
    delete _internal;
}

tupp & tupp::instance()
{
    static tupp instance;
    return instance;
}

tupp_internal * tupp::internal()
{
    return instance()._internal;
}

int tupp::run(int argc, char* argv[])
{
    tupp_internal * _internal = internal();
    return _internal->run(argc, argv);
}

void tupp::add_test(const TestFunc & test_func, const std::string & name)
{
    tupp_internal * _internal = internal();
    _internal->add_test(test_func, name);
}

void tupp::assert(bool v, const std::string & msg, const std::string & additional_msg, size_t line)
{
    tupp_internal * _internal = internal();
    _internal->set_msg(msg);
    if (!additional_msg.empty())
        _internal->set_additional_msg(additional_msg);
    _internal->set_line(line);
    _internal->assert(v);
    _internal->apply_case();
}

void tupp::assert_tf(bool v, bool expected, const std::string & msg, const std::string & additional_msg, size_t line)
{
    tupp_internal * _internal = internal();
    _internal->set_msg(msg);
    if (!additional_msg.empty())
        _internal->set_additional_msg(additional_msg);
    _internal->set_line(line);
    _internal->set_expected(expected ? "true" : "false");
    _internal->assert(v == expected);
    _internal->apply_case();
}

void tupp::assert_flt(float a, float b, const std::string & msg, const std::string & additional_msg, size_t line)
{
    assert(
        std::fabs(a - b) <= FLOAT_CHECKING_ACCURACY,
        msg, additional_msg, line
    );
}

void tupp::message(const std::string & msg, size_t line)
{
    tupp_internal * _internal = internal();
    _internal->set_msg(msg);
    _internal->set_line(line);
    _internal->apply_case();
}
