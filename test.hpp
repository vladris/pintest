// test.hpp proof of concept
//
// Copyright (c) 2013  Vlad Riscutia

#include <map>
#include <string>

// Testable
class Testable 
{ 
public:
    Testable(const std::string &name)
    {
        this->name = name;
    }

    std::string name;

    virtual void run() = 0; 
};

// Null test
class NullTest : public Testable
{
public:
	static NullTest& getInstance()
	{
		static NullTest instance;

		return instance;
	}
   
    void run() override { }

private:
	// No public constructors
    NullTest() : Testable("") { }
    NullTest(NullTest const&) : Testable("") { }
	void operator=(NullTest const&);
};

// Testable collection
class TestableCollection : public Testable
{
public:
    TestableCollection(const std::string &name)
        : Testable(name)
    {
        this->setup = &NullTest::getInstance();
        this->teardown = &NullTest::getInstance();
    }

    Testable *setup;
    Testable *teardown;

    void add(Testable *runable)
    {
        collection[runable->name] = runable;
    }

    void run() override
    {
        for (auto runable : collection)
        {
            setup->run();
            runable.second->run();
            teardown->run();
        }
    }

private:
    std::map<std::string, Testable*> collection;
};

// Test register
class Tests : public TestableCollection
{
public:
	// This is a singleton
	static Tests& getInstance()
	{
		static Tests instance;

		return instance;
	}

    static __declspec(dllexport) void run_all()
    {
        getInstance().run();
    }

private:
	// No public constructors
    Tests() : TestableCollection("") { }
    Tests(Tests const&) : TestableCollection("") { }
	void operator=(Tests const&);
};

// Test group
class TestGroup : public TestableCollection
{
public:
    TestGroup(const std::string &name)
        : TestableCollection(name)
    {
        Tests::getInstance().add(this);
    }
};


// Test groups are mapped to namespaces
#define TEST_GROUP(name)		namespace name { static TestGroup testGroup(#name); } namespace name

// Setup and teardown
#define TEST_SETUP()	struct Setup : Testable \
                        { \
                            Setup() : Testable("setup") { testGroup.setup = this; } \
                            void run() override; \
                        } setup; \
                        void Setup::run()

#define TEST_TEARDOWN() struct Teardown : Testable \
                        { \
                            Teardown() : Testable("teardown") { testGroup.teardown = this; } \
                            void run() override; \
                        } teardown; \
                        void Teardown::run()

// Test (automatically instantiates itself and adds itself to test group)
#define TEST(name)	struct name : public Testable \
					{ \
                        name() : Testable(# name) { testGroup.add(this); } \
						void run() override; \
					} _ ## name; \
					void name::run()
