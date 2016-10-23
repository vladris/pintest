// test.hpp
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace test { namespace details {

struct assert_failed_exception
{
    const std::string message;
};

enum class result : uint8_t
{
    Success = 0, // Test succeeded
    Failed,      // Assertion failed
    Exception,   // Test threw an exception
    Invalid      // Invalid test
};

struct test_registry
{
    using test_func = std::function<void()>;

    static auto& use()
    {
        static test_registry instance;

        return instance;
    }

    void push_group(const std::string& name)
    {
        _group = name;
    }

    void add_test(const std::string& name, test_func func)
    {
        _tests.push_back({ _group, name, func });
    }

    auto list_tests()
    {
        std::string list;

        for (auto&& test : _tests)
            list += test.group + "::" + test.name + ",";

        return list.erase(list.size() - 1);
    }

    auto find_test(const std::string& group, const std::string& name)
    {
        return std::find_if(_tests.begin(), _tests.end(),
            [&](auto& test) { return test.group == group && test.name == name; });
    }

    auto end()
    {
        return _tests.end();
    }

private:
    struct test
    {
        const std::string group;
        const std::string name;
        test_func func;
    };

    std::string _group;
    std::vector<test> _tests;
};

struct test_executor
{
    static auto& use()
    {
        static test_executor instance;

        return instance;
    }

    auto run_test(const std::string& group, const std::string& name)
    {
        return run_fixture(group, name);
    }

private:
    auto run_function(const std::string& group, const std::string& name)
    {
        auto registry = test_registry::use();

        // Find function in fixture
        auto it = registry.find_test(group, name);

        // Invalid if not found
        if (it == registry.end())
            return result::Invalid;

        try
        {
            // Execute
            it->func();
        }
        catch (const assert_failed_exception&)
        {
            // Assertion failure
            return result::Failed;
        }
        catch (...)
        {
            // Unhandled exception
            return result::Exception;
        }

        // Great success
        return result::Success;
    }

    auto run_fixture(const std::string& group, const std::string& test) -> result
    {
        // Don't run if exception in test setup
        if (run_function(group, "__setup") == result::Exception)
            return result::Exception;

        auto result = run_function(group, test);

        // Result becomes exception if exception in teardown
        if (run_function(group, "__teardown") == result::Exception)
            return result::Exception;

        return result;
    }
};

// Helper for instantiating and registering a test group
template <typename T> struct instance_helper
{
    std::unique_ptr<T> instance;

    instance_helper(const std::string& name)
    {
        // Instantiate test group
        instance = std::make_unique<T>();

        // Register name-type pair
        test_registry::use().push_group(name);
    }
};

// Helper for registering a test method
struct register_test_helper
{
    register_test_helper(const std::string& name, test_registry::test_func func)
    {
        // Register test and associate with group
        test_registry::use().add_test(name, func);
    }
};

// Implementation of AssertFailedException
struct assert_failed_exception_impl : public assert_failed_exception
{
    assert_failed_exception_impl(const std::string& message, const std::string& filename, const int lineno)
    {
        std::cerr << "Assert failed at " << filename << ":" << lineno << std::endl <<
            message << std::endl;
    }
};

#ifdef _MSC_VER
    // inline allows implementation in header, __declspec(noinline) prevents actual inlining
    #define WEAK    inline __declspec(noinline) __declspec(dllexport)
#else
    #define WEAK    __attribute__((weak))
#endif // _MSC_VER

// List all tests
extern "C" WEAK const char *list_tests()
{
    return test_registry::use().list_tests().c_str();
}

// Runs the given test from the given group
extern "C" WEAK int run_test(const char *group, const char *name)
{
    return static_cast<int>(test_executor::use().run_test(group, name));
}

}} // namespace test::details

// An easy way to skip defining these in favor of custom asserts
#ifndef CUSTOM_TEST_ASSERTS

#define ASSERT_EQUALS(expected, actual)             if ((expected) != (actual)) { throw test::details::assert_failed_exception_impl("ASSERT_EQUALS: " # actual " doesn't equal " # expected, __FILE__, __LINE__); }
#define ASSERT_NOTEQUALS(expected, actual)          if ((expected) == (actual)) { throw test::details::assert_failed_exception_impl("ASSERT_NOTEQUALS: " # actual " equals " # expected, __FILE__, __LINE__);  }


#define ASSERT_ISTRUE(actual)                       if (!(actual)) { throw test::details::assert_failed_exception_impl("ASSERT_ISTRUE: " # actual " is false", __FILE__, __LINE__); }
#define ASSERT_ISFALSE(actual)                      if (actual) { throw test::details::assert_failed_exception_impl("ASSERT_ISFALSE: " # actual " is true", __FILE__, __LINE__); }

#define ASSERT_ISNULL(actual)                       if ((actual) != nullptr) { throw test::details::assert_failed_exception_impl("ASSERT_ISNULL: " # actual " is not NULL", __FILE__, __LINE__); }
#define ASSERT_NOTNULL(actual)                      if ((actual) == nullptr) { throw test::details::assert_failed_exception_impl("ASSERT_NOTNULL: " # actual " is NULL", __FILE__, __LINE__); }

#define ASSERT_THROWS(exception, code)              try \
                                                    { \
                                                        code; \
                                                        throw test::details::assert_failed_exception_impl("ASSERT_THROWS: " # code " doesn't throw an exception", __FILE__, __LINE__); \
                                                    } \
                                                    catch(const exception&) \
                                                    { \
                                                    } \
                                                    catch (...) \
                                                    { \
                                                        throw test::AssertFailedException("ASSERT_THROWS: " # code " doesn't throw " # exception, __FILE__, __LINE__); \
                                                    } \

#define ASSERT_FAIL()                               throw test::AssertFailedException("ASSERT_FAIL", __FILE__, __LINE__);

#endif // CUSTOM_TEST_ASSERTS

// Test groups are classes
#define TEST_GROUP(name)    struct name; \
                            \
                            test::details::instance_helper<name> _ ## name ## _ ## instance(# name); \
                            \
                            struct name

// Test (automatically registers itself)
#define TEST(name)          test::details::register_test_helper _ ## name { # name, [&]() { this->name(); } }; \
                            void name()

// Setup and teardown
#define TEST_SETUP()        TEST(__setup)

#define TEST_TEARDOWN()     TEST(__teardown)
