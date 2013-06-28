// test.hpp proof of concept
//
// Copyright (c) 2013  Vlad Riscutia

#include <stdio.h>
#include <vector>
#include <memory>

// Runable
struct Runable { virtual void run() = 0; };

// Base test class
struct BaseTest : Runable
{
	virtual void setup() = 0;
	virtual void test() = 0;
	virtual void teardown() = 0;

	void run() override
	{
		setup();
		test();
		teardown();
	}
};

// Test register
class Tests : Runable
{
public:
	// This is a singleton
	static Tests& getInstance()
	{
		static Tests instance;

		return instance;
	}

	// Add a test group
	void add(Runable *reg)
	{
		groups.push_back(reg);
	}

	// Run all tests
	void run() override
	{
		for (auto group : groups)
		{
			group->run();
		}
	}

    static __declspec(dllexport) void run_all()
    {
        getInstance().run();
    }

private:
	// No public constructors
	Tests() { };
	Tests(Tests const&) { };
	void operator=(Tests const&);

	// Test groups
	std::vector<Runable*> groups;
};

// Represents a group of tests
class TestGroup : Runable
{
public:
	TestGroup()
	{
		// Adds itself to the test register
		Tests::getInstance().add(this);
	}

	// Add a test
	void add(BaseTest *test)
	{
		tests.push_back(test);
	}

	// Run all tests in group
	void run()
	{
		for (auto test : tests)
		{
			test->run();
		}
	}
private:
	std::vector<BaseTest*> tests;
};

// Test groups are mapped to namespaces
#define TEST_GROUP(name)		namespace name { static TestGroup testGroup; } namespace name

// Setup and teardown
#define TEST_SETUP()	struct Setup { static void run(); }; void Setup::run()
#define TEST_TEARDOWN() struct Teardown { static void run(); }; void Teardown::run()

// Test (automatically instantiates itself and adds itself to test group)
#define TEST(name)	struct name : public BaseTest \
					{ \
					    name() { testGroup.add(this); } \
						void setup() { Setup::run(); } \
						void teardown() { Teardown::run(); } \
						void test(); \
					} _ ## name; \
					void name::test()

// Setup and teardown to be used if the test group doesn't define its own
struct Setup { static void run() {} };
struct Teardown { static void run() {} };
