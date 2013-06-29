// test.hpp
//
// Copyright (c) 2013  Vlad Riscutia, MIT License

#ifndef __TEST_HPP
#define __TEST_HPP 

#include <map>
#include <string>
#include <vector>

namespace test 
{

    // ****************************************************************************
    // Internals
    // ****************************************************************************
    namespace internal
    {
        // Runable object
        class Runable
        {
        public:
            virtual void run() { };

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
            virtual void run(const std::string &name) { }

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

    private:
        // No public constructors
        Executor() : Collection("") { }
        Executor(Executor const&) : Collection("") { }
        void operator=(Executor const&);
    };

    // ****************************************************************************
    // Interop exports
    // ****************************************************************************
    class Interop
    {
        // Gets the test groups
        static __declspec(dllexport) const char *get_group(const char *previous)
        {
            return Executor::get_instance().get_names(previous);
        }

        // Gets the tests
        static __declspec(dllexport) const char *get_test(const char *group, const char *previous)
        {
            try
            {
                return Executor::get_instance().collection.at(group)->get_names(previous);
            }
            catch (std::out_of_range)
            {
                return nullptr;
            }
        }

        // Runs the given test from the given group
        static __declspec(dllexport) void run_test(const char *group, const char *test)
        {
            try
            {
                Executor::get_instance().collection[group]->run(test);
            }
            catch (std::out_of_range)
            {
                // TODO: when run_test returns a test resul
            }
        }
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
            void run(const std::string &name) override
            {
                run_fixture(collection[name]);
            }

        private:
            void run_fixture(Test* test)
            {
                setup->run();
                test->run();
                teardown->run();
            }
        };
    } // namespace internal
} // namespace test

// ****************************************************************************
// Macros
// ****************************************************************************

// Test groups are mapped to namespaces
#define TEST_GROUP(name)    namespace name { static test::internal::TestGroup testGroup(#name); } namespace name

// Setup and teardown
#define TEST_SETUP()	    struct Setup : public test::internal::Runable \
                            { \
                                Setup() { testGroup.setup = this; } \
                                void run() override; \
                            } setup; \
                            void Setup::run()

#define TEST_TEARDOWN()     struct Teardown : public test::internal::Runable \
                            { \
                                Teardown() { testGroup.teardown = this; } \
                                void run() override; \
                            } teardown; \
                            void Teardown::run()

// Test (automatically instantiates itself and registers itself to test group)
#define TEST(name)	        struct name : public test::internal::Test \
                            { \
                                name() : Test(# name) { testGroup.register_test(this); } \
                                void run() override; \
                            } _ ## name; \
                            void name::run()

#endif // __TEST_HPP