// test.hpp
//
// Copyright (c) 2013  Vlad Riscutia, MIT License

#ifndef __TEST_HPP
#define __TEST_HPP 

#include <map>
#include <string>

namespace test 
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

// Testable
class Testable : public Runable
{ 
public:
    // Get the name of the testable
    std::string& get_name()
    {
        return name;
    }

protected:
    // Protected constructor
    Testable(const std::string &name)
    {
        this->name = name;
    }

private:
    // Testable name
    std::string name;
};

// Testable collection
class TestableCollection : public Testable
{
public:
    TestableCollection(const std::string &name)
        : Testable(name)
    {
        this->setup = &Runable::null_runable();
        this->teardown = &Runable::null_runable();
    }

    // Setup and teardown
    Runable *setup;
    Runable *teardown;

    // Register a testable
    void register_testable(Testable *testable)
    {
        collection[testable->get_name()] = testable;
    }

    // Run all testables
    void run() override
    {
        for (auto testable : collection)
        {
            run_testable(testable.second);
        }
    }

    // Run a testable from the collection
    void run(const std::string &name)
    {
        run_testable(collection[name]);
    }

private:
    std::map<std::string, Testable*> collection;

    void run_testable(Testable* testable)
    {
        setup->run();
        testable->run();
        teardown->run();
    }
};

// Test executor
class Executor : public TestableCollection
{
public:
    // Get singleton
	static Executor& get_instance()
	{
		static Executor instance;

		return instance;
	}

    // Used by runner
    static __declspec(dllexport) void run_all()
    {
        get_instance().run();
    }

private:
	// No public constructors
    Executor() : TestableCollection("") { }
    Executor(Executor const&) : TestableCollection("") { }
	void operator=(Executor const&);
};

// A group of tests
class TestGroup : public TestableCollection
{
public:
    // The group registers itself during construction
    TestGroup(const std::string &name)
        : TestableCollection(name)
    {
        Executor::get_instance().register_testable(this);
    }
};

} // namespace test

// Test groups are mapped to namespaces
#define TEST_GROUP(name)    namespace name { static test::TestGroup testGroup(#name); } namespace name

// Setup and teardown
#define TEST_SETUP()	    struct Setup : public test::Runable \
                            { \
                                Setup() { testGroup.setup = this; } \
                                void run() override; \
                            } setup; \
                            void Setup::run()

#define TEST_TEARDOWN()     struct Teardown : public test::Runable \
                            { \
                                Teardown() { testGroup.teardown = this; } \
                                void run() override; \
                            } teardown; \
                            void Teardown::run()

// Test (automatically instantiates itself and registers itself to test group)
#define TEST(name)	        struct name : public test::Testable \
                            { \
                                name() : Testable(# name) { testGroup.register_testable(this); } \
                                void run() override; \
                            } _ ## name; \
                            void name::run()

#endif // __TEST_HPP