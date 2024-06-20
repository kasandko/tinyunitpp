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

This macro allows to test two values ​​for equality. It has the following arguments:
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

This macro allows to test two values ​​for not equality. It has the following arguments:
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

This macro allows to check getting boolean value for truth. It has the following arguments:
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

This macro allows to check getting boolean value for untruth. It has the following arguments:
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

This macro allows to display any messages or string to console. It has the following arguments:
* `MSG` - Output string or string variable. A type of variable can be implicitly casting to
    `std::string`.

Example:

```cpp
std::string msg = "Message 1";

TUPP_MESSAGE(msg);
TUPP_MESSAGE("Message 2");
```