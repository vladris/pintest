# Pin Test

*The lightest unit test framework to poke holes in your C++ logic*

Pin Test is a featherweight unit testing framework for C++. It is easy to integrate with both existing native code and existing automation. On the native side, Pin Test is implemented as a single header, `pintest.hpp`. The framework comes with a Python3 test runner which supports automatic test discovery. Other runners can be easily implemented, as the native interface consists of just 2 `extern "C"` functions.

Tested with Clang 3.5, GCC 4.8, MSVC 14.0. Test runner requires Python3.

## Getting Started

Get `pintest.hpp`:

    curl https://raw.githubusercontent.com/vladris/pintest/master/pintest.hpp > pintest.hpp

Create a unit test:

```c++
#include "pintest.hpp"

TEST_GROUP(example_tests)
{
   TEST(test_addition)
   {
      test::assert::equals(42, 40 + 2);
   }
        
   TEST(test_subtraction)
   {
      test::assert::not_equals(1, 42 - 40);
   }
};
```
    
Build all unit tests as a shared library (.DLL on Windows, .so on Linux).

Get the test runner using pip:

    sudo pip3 install pintest 

From a parent directory of your build target, run:

    pintest
    
That's it. The test runner will find the library and run all available tests.

## pintest.hpp

### Macros and test registration

Unlike most C++ unit test frameworks, Pin Test is not macro-heavy. The only 4 macros #defined are `TEST_GROUP`, `TEST`, `TEST_SETUP` and `TEST_TEARDOWN`. `TEST_GROUP` declares a test class and registers a static instance of it with the framework. `TEST_GROUP(foo)` declares `foo` as `struct foo`. This declaration can be succeeded with inheritance etc., the only requirement being a default constructor. `TEST` declares a method which returns void and doesn't take any arguments, and registers it with the framework. `TEST_SETUP` and `TEST_TEARDOWN` register the special methods `void __setup()` and `void __teardown()` with the framework - these are called before and after each test in the group.     

### Asserts

In Pin Test, asserts are functions which throw `test::details::assert_failed_exception` on failure. This exception should be handled internally by the framework, so client code doesn't need to worry about it. The following asserts are provided:

```c++
void fail(const std::string& message = "");
    
template <typename T, typename U>
void equals(const T& expected, const U& actual, const std::string& message = "");

template <typename T, typename U>
void not_equals(const T& expected, const U& actual, const std::string& message = "");

template <typename T>
void is_true(const T& actual, const std::string& message = "");

template <typename T>
void is_false(const T& actual, const std::string& message = "");

template <typename T>
void is_null(const T& actual, const std::string& message = "");

template <typename T>
void is_not_null(const T& actual, const std::string& message = "");

template <typename T, typename Callable>
void throws(Callable c, const std::string& message = ""); 
```

# Interface

The following two functions are exported from the shared library by the framework:

```c++
extern "C" const char *list_tests();

extern "C" int run_test(const char *group, const char *name);
```

These are C exports to make it easier to integrate with other languages/frameworks used to drive automation. `list_tests` returns a comma-separated list of all tests in this shared library (a test is represented as `group::name`). `run_test` takes a group and a name, runs the specified test (including setup and teardown if available), and returns the result as an int. Results are:

    0 // Success => test passed
    1 // Failed => assertion failed, assertion message is printed to stderr
    2 // Exception => test (or setup/teardown) threw an unhandled exception
    3 // Invalid => test not found

## Test Runner

### Autodiscovery

The Python3 test runner implements test autodiscovery by crawling a directory and its subdirectories and checking for every shared library whether it exports the `list_tests` and `run_test` functions. In list mode, it will simply print all test modules found and all tests functions registered within them:

    pintest -l some/directory
    
Or, to scan the current directory, don't provide a path after `-l`:

    pintest -l
    
To only list the tests within a specific module, use the `-m` flag:

    pintest -l -m some/shared_module.so
    
### Running Tests

To run all tests under the given directory, call:

    pintest -r some/directory
    
To run all tests under the current directory, call:

    pintest -r
    
or call without any flags:

    pintest
    
To run all tests within a specified module, use the `-m` flag:

    pintest -r -m some/shared_module.so
    
To run a particular test within a module, use both `-m` and `-t` flags:

    pintest -r -m some/shared_module.so -t test_group::test_name

### Output

When running tests, `pintest` will print the name of the module and one of the following characters for each test: `.FEICM`. Characters are interpreted as follows:

    . => test passed
    F => test failed
    E => test threw an unhandled exception
    I => test is invalid
    
The above map directly to the native test results returned by `run_test`. Additionally, we have:

    C => exception was thrown inside the Python test runner itself
    M => invalid module provided via -m argument

Additional output from C++ code is not suppressed, notably assert failures print messages to stderr.

---

Copyright (c) 2016 Vlad Riscutia

[MIT License] (http://opensource.org/licenses/mit)
