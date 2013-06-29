// test.hpp
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License

#ifndef __TEST_HPP
#define __TEST_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

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
    // Internals
    // ****************************************************************************
    namespace internal
    {
        // Runable object
        class Runable
        {
        public:
            virtual void run() { }

            // Get null object
            static Runable& null_runable()
            {
                static Runable instance;

                return instance;
            }
        };

        // Named object
        class Named
        {
        public:
            // Get the name of the object
            std::string& get_name()
            {
                return name;
            }

        protected:
            Named(const std::string &name)
            {
                this->name = name;
            }

        private:
            // Name
            std::string name;
        };

        // A test is a named runnable
        class Test : public Runable, public Named
        {
        public:
            Test(const std::string &name)
                : Named(name)
            {
            }
        };

        // Named collection
        template <typename T> class Collection : public Named
        {
        public:
            Collection(const std::string &name)
                : Named(name)
            {
            }

            // Register an item
            void register_test(T *test)
            {
                names.push_back(test->get_name());
                collection[test->get_name()] = test;
            }

            // Run the named item
            virtual Result run(const std::string &name)
            {
                return Result::Invalid;
            }

            // Return the name of the item following previous
            const char *get_names(const char *previous)
            {
                auto it = names.begin();

                // Return first if no previous
                if (previous == nullptr)
                {
                    return it->c_str();
                }

                while (it->compare(previous) != 0)
                {
                    it++;

                    if (it == names.end())
                    {
                        return nullptr;
                    }
                }

                it++;

                // Return null if at end of collection, otherwise return the name
                if (it == names.end())
                {
                    return nullptr;
                }
                else
                {
                    return it->c_str();
                }
            }

            std::map<std::string, T*> collection;

        protected:
            std::vector<std::string> names;
        };
    } // namespace internal

    class AssertFailedException : public std::exception
    {
    public:
        AssertFailedException(const char* message, const char* filename, int lineno)
        {
            this->message = message;
            this->filename = filename;
            this->lineno = lineno;
        }

    private:
        const char* message;
        const char* filename;
        int lineno;
    };

    // ****************************************************************************
    // Test executor
    // ****************************************************************************
    class Executor : public internal::Collection<internal::Collection<internal::Test>>
    {
    public:
        // Get singleton
        static Executor& get_instance()
        {
            static Executor instance;

            return instance;
        }

        Result run_test(const std::string &group, const std::string &test)
        {
            try
            {
                return collection[group]->run(test);
            }
            catch (std::out_of_range)
            {
                return Result::Invalid;
            }
        }

    private:
        // No public constructors
        Executor() : Collection("") { }
        Executor(Executor const&) : Collection("") { }
        void operator=(Executor const&);
    };

    // ****************************************************************************
    // Internals
    // ****************************************************************************
    namespace internal
    {
        // A group of tests
        class TestGroup : public Collection<Test>
        {
        public:
            // The group registers itself during construction
            TestGroup(const std::string &name)
                : Collection(name)
            {
                Executor::get_instance().register_test(this);

                this->setup = &Runable::null_runable();
                this->teardown = &Runable::null_runable();
            }

            // Setup and teardown
            Runable *setup;
            Runable *teardown;

            // Run a test from the collection
            Result run(const std::string &name) override
            {
                return run_fixture(collection[name]);
            }

        private:
            Result run_fixture(Test* test)
            {
                try
                {
                    setup->run();
                    test->run();
                    teardown->run();
                }
                catch (...)
                {
                    return Result::Exception;
                }

                return Result::Success;
            }
        };
    } // namespace internal
} // namespace test

#ifdef _MSC_VER
    // inline allows implementation in header, __declspec(noinline) prevents actual inlining
    #define WEAK    inline __declspec(noinline) __declspec(dllexport)
#else
    #define WEAK    __attribute__((weak))
#endif // _WIN32

// Gets the test groups
extern "C" WEAK const char *get_group(const char *previous)
{
    return test::Executor::get_instance().get_names(previous);
}

// Gets the tests
extern "C" WEAK const char *get_test(const char *group, const char *previous)
{
    try
    {
        return test::Executor::get_instance().collection.at(group)->get_names(previous);
    }
    catch (std::out_of_range)
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

#define THROW_ASSERT(message)                       throw test::AssertFailedException(message, __FILE__, __LINENO__)

#define ASSERT_EQUALS(expected, actual)             if ((expected) != (actual)) { THROW_ASSERT("ASSERT_EQUALS failed: " # actual " doesn't equal " # expected); }
#define ASSERT_NOTEQUALS(expected, actual)          if ((expected) == (actual)) { THROW_ASSERT("ASSERT_NOTEQUALS failed: " # actual " equals " # expected, __FILE__, __LINENO__);  }

#define ASSERT_ISTRUE(actual)                       if (!(actual)) { THROW_ASSERT("ASSERT_ISTRUE failed: " # actual " is false", __FILE__, __LINENO__); }
#define ASSERT_ISFALSE(actual)                      if ((actual) { THROW_ASSERT("ASSERT_ISFALSE failed: " # actual " is true", __FILE__, __LINENO__); }

#define ASSERT_ISNULL(actual)                       if ((actual) != nullptr) { THROW_ASSERT("ASSERT_ISNULL failed: " # actual " is not NULL", __FILE__, __LINENO__); }
#define ASSERT_NOTNULL(actual)                      if ((actual) == nullptr) { THROW_ASSERT("ASSERT_NOTNULL failed: " # actual " is NULL", __FILE__, __LINENO__); }

#define ASSERT_THROWS(exception, code)              try \
                                                    { \
                                                        code; \
                                                        THROW_ASSERT("ASSERT_THROWS failed: " # code " doesn't throw an exception", __FILE__, __LINENO__); \
                                                    } \
                                                    catch(exception) \
                                                    { \
                                                    } \
                                                    catch (...) \
                                                    { \
                                                        throw test::AssertFailedException("ASSERT_THROWS failed: " # code " doesn't throw " # exception, __FILE__, __LINENO__); \
                                                    } \

#define ASSERT_FAIL()                               throw test::AssertFailedException("ASSERT_FAIL", __FILE__, __LINENO__);

#endif // CUSTOM_TEST_ASSERTS

// ****************************************************************************
// Test macros
// ****************************************************************************

// Test groups are mapped to namespaces
#define TEST_GROUP(name)    namespace name { static test::internal::TestGroup _testGroup(#name); } namespace name

// Setup and teardown
#define TEST_SETUP()	    struct Setup : public test::internal::Runable \
                            { \
                                Setup() { _testGroup.setup = this; } \
                                void run() override; \
                            } _setup; \
                            void Setup::run()

#define TEST_TEARDOWN()     struct Teardown : public test::internal::Runable \
                            { \
                                Teardown() { _testGroup.teardown = this; } \
                                void run() override; \
                            } _teardown; \
                            void Teardown::run()

// Test (automatically instantiates itself and registers itself to test group)
#define TEST(name)	        struct name : public test::internal::Test \
                            { \
                                name() : Test(# name) { _testGroup.register_test(this); } \
                                void run() override; \
                            } _ ## name; \
                            void name::run()

#endif // __TEST_HPP
