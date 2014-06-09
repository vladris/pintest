// test.hpp
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License

#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>

namespace test
{

    // ****************************************************************************
    // Test results
    // ****************************************************************************
    enum Result
    {
        Success = 0, // Test succeeded
        Failed,      // Assertion failed
        Exception,   // Test threw an exception
        Invalid      // Invalid test
    };

    // ****************************************************************************
    // AssertFailedException
    // ****************************************************************************
    class AssertFailedException : public std::exception
    {
    };

    // ****************************************************************************
    // Test executor
    // ****************************************************************************
    class Executor
    {
    public:
        // Get singleton
        static Executor& get_instance()
        {
            static Executor instance;

            return instance;
        }

        void add_group(const std::string &name)
        {
			tests[name] = current_tests;
			current_tests = NamedTest();
        }

        void add_test(const std::string &name, std::function<void()> test)
        {
			current_tests[name] = test;
        }
        
		template <typename T> const char *get_with_previous(T &collection, const char* previous)
		{
            // If no previous specified, return name of first item in collection
			if (!previous)
			{
				return collection.begin()->first.c_str();
			}

            // Otherwise find item following previous
			auto it = collection.find(previous);
			it++;

			if (it == collection.end())
			{
				return nullptr;
			}

			return it->first.c_str();
		}

		const char* get_group(const char *previous)
		{
			return get_with_previous(tests, previous);
		}

		const char* get_test(const char *group, const char *previous)
		{
			return get_with_previous(tests[group], previous);
        }

		Result run_test(const std::string &group, const std::string &name)
		{
			return run_fixture(group, name);
		}

    private:
        // No public constructors
		Executor() { }
		Executor(Executor const&) = delete;
		void operator=(Executor const&) = delete;

		Result run_function(const std::string &group, const std::string &function)
		{
            // Find function in fixture
			auto f = tests[group].find(function);

            // Invalid if not found
			if (f == tests[group].end())
			{
				return Result::Invalid;
			}

			try
			{
                // Execute
				f->second();
			}
			catch (const AssertFailedException&)
			{
                // Assertion failure
				return Result::Failed;
			}
			catch (...)
			{
                // Unhandled exception
				return Result::Exception;
			}

            // Great success
			return Result::Success;
		}

		Result run_fixture(const std::string &group, const std::string &test)
		{
            // Don't run if exception in test setup
			if (run_function(group, "setup") == Result::Exception)
			{
				return Result::Exception;
			}

			auto result = run_function(group, test);

            // Result becomes exception if exception in teardown
			if (run_function(group, "teardown") == Result::Exception)
			{
				return Result::Exception;
			}

			return result;
		}

		typedef std::map<std::string, std::function<void()>> NamedTest;
		NamedTest current_tests;
		std::map<std::string, NamedTest> tests;
    };

    // ****************************************************************************
    // Internals
    // ****************************************************************************
    namespace internal
    {
        // Helper for instantiating and registering a test group
        template <typename T> struct InstanceHelper
        {
            std::unique_ptr<T> instance;

            InstanceHelper(const char *name)
            {
                // Instantiate test group
				instance = std::make_unique<T>();

                // Register name-type pair
                Executor::get_instance().add_group(name);
            }
        };

        // Helper for registering a test method
        struct RegisterTest
        {
            RegisterTest(const char *name, std::function<void()> test)
            {
                // Register test and associate with group
                Executor::get_instance().add_test(name, test);
            }
        };

        // Implementation of AssertFailedException
        class AssertFailedExceptionImpl : public AssertFailedException
        {
        public:
            AssertFailedExceptionImpl(const char* message, const char* filename, int lineno)
            {
                std::cerr << "Assert failed at " << filename << ":" << lineno << std::endl <<
                    message << std::endl;
            }
        };
    } // namespace internal
} // namespace test

#ifdef _MSC_VER
    // inline allows implementation in header, __declspec(noinline) prevents actual inlining
    #define WEAK    inline __declspec(noinline) __declspec(dllexport)
#else
    #define WEAK    __attribute__((weak))
#endif // _MSC_VER

// Gets the test groups
extern "C" WEAK const char *get_group(const char *previous)
{
	try
	{
        return test::Executor::get_instance().get_group(previous);
	}
	catch (std::out_of_range&)
	{
		return nullptr;
	}
}

// Gets the tests
extern "C" WEAK const char *get_test(const char *group, const char *previous)
{
	try
	{
		return test::Executor::get_instance().get_test(group, previous);
	}
	catch (std::out_of_range&)
	{
		return nullptr;
	}
}

// Runs the given test from the given group
extern "C" WEAK int run_test(const char *group, const char *test)
{
	return test::Executor::get_instance().run_test(group, test);
}

// ****************************************************************************
// Assert macros
// ****************************************************************************

// An easy way to skip defining these in favor of custom asserts
#ifndef CUSTOM_TEST_ASSERTS

#define ASSERT_EQUALS(expected, actual)             if ((expected) != (actual)) { throw test::internal::AssertFailedExceptionImpl("ASSERT_EQUALS: " # actual " doesn't equal " # expected, __FILE__, __LINE__); }
#define ASSERT_NOTEQUALS(expected, actual)          if ((expected) == (actual)) { throw test::internal::AssertFailedExceptionImpl("ASSERT_NOTEQUALS: " # actual " equals " # expected, __FILE__, __LINE__);  }

#define ASSERT_ISTRUE(actual)                       if (!(actual)) { throw test::internal::AssertFailedExceptionImpl("ASSERT_ISTRUE: " # actual " is false", __FILE__, __LINE__); }
#define ASSERT_ISFALSE(actual)                      if (actual) { throw test::internal::AssertFailedExceptionImpl("ASSERT_ISFALSE: " # actual " is true", __FILE__, __LINE__); }

#define ASSERT_ISNULL(actual)                       if ((actual) != nullptr) { throw test::internal::AssertFailedExceptionImpl("ASSERT_ISNULL: " # actual " is not NULL", __FILE__, __LINE__); }
#define ASSERT_NOTNULL(actual)                      if ((actual) == nullptr) { throw test::internal::AssertFailedExceptionImpl("ASSERT_NOTNULL: " # actual " is NULL", __FILE__, __LINE__); }

#define ASSERT_THROWS(exception, code)              try \
                                                    { \
                                                        code; \
                                                        throw test::internal::AssertFailedExceptionImpl("ASSERT_THROWS: " # code " doesn't throw an exception", __FILE__, __LINE__); \
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

// ****************************************************************************
// Test macros
// ****************************************************************************

// Test groups are classes
#define TEST_GROUP(name)    struct name; \
                            \
                            test::internal::InstanceHelper<name> _ ## name ## _ ## instance(# name); \
                            \
                            struct name

// Test (automatically registers itself)
#define TEST(name)	        test::internal::RegisterTest _ ## name { # name, [&]() { this->name(); } }; \
                            void name()

// Setup and teardown
#define TEST_SETUP()	    TEST(setup)

#define TEST_TEARDOWN()     TEST(teardown)
