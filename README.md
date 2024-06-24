# TinyUnit++

[Russian documentation](README_RU.md)

TinyUnit++ - it's the minimalistic unit test system for C++. Features:
* The source code consists of two files.
* Assert types:
    * Equality or not quality test for two variables.
    * Boolean variables and expression test.
    * Equality test for two float point variables (with some accuracy).
* Adding a custom message to any assert's methods and macros.
* Output any message.
* TinyUnit++ - it's static library. This is allow to integrate this system anywhere.
* It has mode for skip error assert tests.
* It has mode for run tests specified which was command line.
* Some silent modes. (without a console output, but raise %ERRORLEVEL% in case of unsuccessful
    tests).
* This system has not any depends with the exception of the standard c++ library.

## Motivation
The test system was originally created for individual goals as library which can be embedded in
different projects entirely so as not to have unnecessary dependencies. The price to pay for this
opportunity - lack of opportunities which has other unit test systems such as: mature assert
system, architectural fixture and mock support, hierarchical tests, grouping tests and etc.
Consequently if you need something from the above this system this system is not suitable for you.

## Simple example

Let's create a folder `tupp_example`. Put to its files `tupp.cpp`, `tupp.h` from `src` folder.
After this create file `main.cpp` in the same folder with with the following code:

```cpp
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
```

After this compile this example by commands:
```
g++ -c ./main.cpp -o ./main.o -std=c++17
g++ -c ./tupp.cpp -o ./tupp.o -std=c++17
g++ ./tupp.o ./main.o -o ./main -std=c++17
```

Now let's launch `main`. We should get the following result:
```
#### START ####
  TEST 'example_test_bool': SUCCESS
    Message: Example bool. Line: 28
  TEST 'example_test_float': SUCCESS
    Message: Example float. Line: 19
  TEST 'example_test_assert': SUCCESS
    Message: Example assert. Line: 9
#### FINISH ####
  Run: 3/3, Fail: 0, Pass: 3
```

## Macros description

### TUPP_ASSERT

```
TUPP_ASSERT(V_A, V_B, [MSG, [MSG, [...]]]);
```

This macro allows to test two values ​​for equality. It has following arguments:
* `V_A`, `V_B` - Comparing values (or variables). A type of variables doesn't matter. The main thing
    is that they can be comparable.
* `MSG` - Additional messages (you can set any quantity: messages will be merged into one). The
    message can be a string or a string variable.

This assert will be triggered if `V_A` is not equal `V_B`.

Example:

```cpp
int a = 5, b = 5, c = 6;
TUPP_ASSERT(a, b, "Exmaple message"); // Will execute successfully.
TUPP_ASSERT(a, c); // Will be triggered.
```

### TUPP_N_ASSERT

```
TUPP_N_ASSERT(V_A, V_B, [MSG, [MSG, [...]]]);
```

This macro allows to test two values ​​for not equality. It has following arguments:
* `V_A`, `V_B` - Comparing values (or variables). A type of variables doesn't matter. The main thing
    is that they can be comparable.
* `MSG` - Additional messages (you can set any quantity: messages will be merged into one). The
    message can be a string or a string variable.

This assert will be triggered if `V_A` is equal `V_B`.

Example:

```cpp
int a = 5, b = 5, c = 6;
TUPP_N_ASSERT(a, c, "Exmaple message"); // Will execute successfully.
TUPP_N_ASSERT(a, b); // Will be triggered.
```

### TUPP_ASSERT_TRUE

```
TUPP_ASSERT_TRUE(V, [MSG, [MSG, [...]]]);
```

This macro allows to check getting boolean value for truth. It has following arguments:
* `V` - Checked value (or variable). A type of variable or expression can be casting to `bool`.
* `MSG` - Additional messages (you can set any quantity: messages will be merged into one). The
    message can be a string or a string variable.

This assert will be triggered if `V` equal `false`.

Example:

```cpp
bool a = true, b = false;
TUPP_ASSERT_TRUE(a, "Exmaple message"); // Will execute successfully.
TUPP_ASSERT_TRUE(b); // Will be triggered.
```

### TUPP_ASSERT_FALSE

```
TUPP_ASSERT_FALSE(V, [MSG, [MSG, [...]]]);
```

This macro allows to check getting boolean value for untruth. It has following arguments:
* `V` - Checked value (or variable). A type of variable or expression can be casting to `bool`.
* `MSG` - Additional messages (you can set any quantity: messages will be merged into one). The
    message can be a string or a string variable.

This assert will be triggered if `V` equal `true`.

Example:

```cpp
bool a = true, b = false;
TUPP_ASSERT_FALSE(b, "Exmaple message"); // Will execute successfully.
TUPP_ASSERT_FALSE(a); // Will be triggered.
```

### TUPP_MESSAGE

```
TUPP_MESSAGE(MSG);
```

This macro allows to display any messages or string to console. It has following arguments:
* `MSG` - Output string or string variable. A type of variable can be implicitly casting to
    `std::string`.

Example:

```cpp
std::string msg = "Message 1";

TUPP_MESSAGE(msg);
TUPP_MESSAGE("Message 2");
```

### TUPP_ASSERT_F

```
TUPP_ASSERT_F(V_A, V_B, [MSG, [MSG, [...]]]);
```

This macro allows to test two floating point values ​​for equality with some accuracy. We can to
compare two values with only `float` type now. Accuracy is specified by the constant
`tupp::FLOAT_CHECKING_ACCURACY`. It has following arguments:
* `V_A`, `V_B` - Comparing values (or variables). A type of variables must be implicitly casting to
    `float`.
* `MSG` - Additional messages (you can set any quantity: messages will be merged into one). The
    message can be a string or a string variable.

This assert will be triggered if `V_A` is different from `V_B` more than
`tupp::FLOAT_CHECKING_ACCURACY`.

Example:

```cpp
float a = 5.0f, b = 5.0f, c = 6.0f;
float d = 5.0f + tupp::FLOAT_CHECKING_ACCURACY / 2.0f;
TUPP_ASSERT(a, b, "Exmaple message"); // Will execute successfully.
TUPP_ASSERT(a, c); // Will be triggered.
TUPP_ASSERT(a, d); // Will execute successfully.
```

### TUPP_ADD_TEST

```
TUPP_ADD_TEST(TEST_NAME);
```

This macro allows to register a test in the library. The test must be represented by a function
that have following signature:

```
void (void)
```

It has following arguments:
* `TEST_NAME` - Name of the function which is the test. This name also will be name of the test.

Example:

```cpp
void example_test()
{
    // ...
}

int main(int argc, char* argv[])
{
    TUPP_ADD_TEST(example_test);

    // ...
}

```

## Methods description

### tupp::run

```cpp
int run(int argc, char* argv[]);
```

Method for run tests. It's assumed that this method will be called after all tests registration
in the end of function `int main(int argc, char* argv[])`. Argumets of the method will be passed to
the `main` function arguments `argc` and `argv`. And A result of the method will be return by this
function.

It has following arguments:
* `argv`, `argc` - Array of strings - command line arguments and its count.

Method return one of result codes which are described in "Result codes description" section.

Example:
```cpp
int main(int argc, char* argv[])
{
    // Tests registration.

    return tupp::run(argc, argv);
}
```

### tupp::message

```cpp
void message(const std::string & msg, size_t line);
```

Method for display any messages to console. It is used in `TUPP_MESSAGE` macro. Arguments:
* `msg` - Displayed message.
* `line` - Potentially: number of string where this method is called.

Instead of this method it is recommended to use `TUPP_MESSAGE` macro.

### tupp::add_test

```cpp
void add_test(const TestFunc & test_func, const std::string & name);
```

It register a test in the library. The test must be represented by function or any callable object
which has the folowing signature:

```
void (void)
```

Arguments:
* `test_func` - Pointer to the function or any callable object which is represented the test.
* `name` - Name of the test. It will be used in messages displaying and as the value of the command
    line key `--test (-t)`.

This method makes sense to use if name of the function must not match the test name or if any
callable object is used as test.

Example:

```cpp
void example_test_func()
{
    // ...
}

int main(int argc, char* argv[])
{
    tupp::add_test(&example_test_func, "example_test");

    // ...
}

```

### tupp::t_assert

```cpp
void t_assert(bool v, const std::string & msg, size_t line, const TMsg & ... additionals)
```

Arguments:
* `v` - Checked value. A assert will be triggered if it is equal `false`.
* `msg` - Potentially: a message which will be created by macro based on a expression used
    (inserted as string).
* `line` - Potentially: number of string where this method is called.
* `additionals` - Additional messages (you can to use any count: messages will be joined in one).

This method is used by macros `TUPP_ASSERT` and `TUPP_N_ASSERT`. It isn't recommended to use this
method in pure form but is may be useful for special assert macros addition.

### tupp::t_assert_tf

```cpp
void t_assert_tf(bool v, bool expected, const std::string & msg, size_t line,
    const TMsg & ... additionals)
```

Arguments:
* `v` - Checked value. A assert will be triggered if it isn't equal `expected`.
* `expected` - Expected value.
* `msg` - Potentially: a message which will be created by macro based on a expression used
    (inserted as string).
* `line` - Potentially: number of string where this method is called.
* `additionals` - Additional messages (you can to use any count: messages will be joined in one).

This method is used by macros `TUPP_ASSERT_TRUE` and `TUPP_ASSERT_FALSE`. It isn't recommended to
use this method in pure form but is may be useful for special assert macros addition.

### tupp::t_assert_flt

```cpp
void t_assert_flt(float a, float b, const std::string & msg, size_t line,
    const TMsg & ... additionals)
```

Arguments:
* `a`, `b` - Checked by equality values.  A assert will be triggered if `a` is different from `b`
    more than `tupp::FLOAT_CHECKING_ACCURACY`.
* `msg` - Potentially: a message which will be created by macro based on a expression used
    (inserted as string).
* `line` - Potentially: number of string where this method is called.
* `additionals` - Additional messages (you can to use any count: messages will be joined in one).

This method is used by macro `TUPP_ASSERT_F`. It isn't recommended to use this method in pure form
but is may be useful for special assert macros addition.

## Other API elements description

* `tupp::FLOAT_CHECKING_ACCURACY` - Constant with allowed difference between two tested `float`
    values by macro `TUPP_ASSERT_F` and method `tupp::t_assert_flt`.
* `tupp::TestFunc` - Description of test function signature.

## Modes and features

### Skip error assert tests mode

If this mode is activated (see "Command line arguments description" below), if a test has some
assert tests one of which is triggered test executing will be continued.

Example:
```cpp
void example_test()
{
    TUPP_ASSERT_TRUE(false);

    TUPP_MESSAGE("Execution continued");
}
```

In this example `TUPP_ASSERT_TRUE(false)` is triggered. If skip error assert tests mode is not
activated than test executing will be stopped on this macro. If this mode is active - executing
will be continue and in this example will be displayed "Execution continued".

### Messages which messages that TinyUnit++ can display

TinyUnit++ has folowing message types:
* **FAIL, SUCCESS** -  Test result.
* **TEST_NAME** - Test name.
* **HEADER** - Title. Separates by `####`. Informs about starting and finishing tests execution.
* **TEST_MESSAGE** - Test messages. This type includes all messages which are generated assert
    functions and macros and messages are displayed by `TUPP_MESSAGE`.
* **REPORT** - Report. Is is displayed after all tests executing. Says how many tests was running,
    how many tests was passed successfully and etc.
* **DEFAULT** - Messages that do not fit any of the above types. There aren't such messages
    currently.

### Silent mode.

Silent mode - it's mode of library working with different count of displayed messages. From output
all messages to no output at all.

Silent mode has some tunable levels and relies on the described above message types system. The
level specified by a positive number. Some level values can be summed thus forming a level that
includes the effects of both added levels.

Levels:
* 0 - All messages are displayed.
* 1 - Hide messages by type `TEST_MESSAGE`. 
* 2 - Hide messages by types `TEST_MESSAGE`, `TEST_NAME`, `FAIL` and `SUCCESS`.
* 10 - Hide messages by type `HEADER`.
* 100 - Hide messages by type `REPORT`.
* 1000 - Hide all messages.


Rules of levels summation:
* Levels cannot add within ten. I.e. cannot add levels 1 and 2, 10 and 20 but can add levels 1 and
    10.
* Level 1000 doesn't add with anyone.

## Command line arguments description

* **--continue_after_assert (-a)** - Skip error assert tests mode activation.
* **--help (-h)** - Show help by command line arguments.
* **--silent_level (-s) \[level\]** - Different variants of silent mode activation.
* **--test (-t) \[test name\]** - Execution test with name "test name". There can be several of
    these keys on the command line (for every test name individual key i.e.: `-t test_a test_b`
    \- wrong, `-t test_a -t test_b` - right). In this case all specified tests will be launched.
* **--version (-v)** - Show version, copyright and other info about the test system.

## Result codes description

* 0 - All tests are passed successfully. There are no errors.
* 1 - At least one test failed.
* 100 - Invalid command line. The mistake details will be printed to the console.
* 101 - Unknown command line key. The mistake details will be printed to the console.
* 102 - Invalid key applying context. The mistake details will be printed to the console.
* 200 - Unknown error. If it works correctly, such an error should not appear.

## Future features

* Colored output and corresponding command line key.
* C++ system exception processing.
* Added tests list output and corresponding command line key for it.

## The library versioning rule

The version has three components:

* Major version - Global changes. Migrating to version 1 will be mean of everything planned
    implementation and correct work of the existing functionality. Also after migrating to version
    1 from this version cannot changed command line keys format, methods and macros signatures
    throughout the entire major version.
* Minor version - Significant changes related to adding new functionality and significant (before
    version 1.0.0) and insignificant (starting from version 1.0.0) changes existing functionality.
    Before version 1.0.0 minor version changes can to broke backwards compatible.
* Correction version - Editing any errors. Cannot to broke backwards compatible.
