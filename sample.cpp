// test.hpp proof of concept
//
// Copyright (c) 2013  Vlad Riscutia
#include "test.hpp"

// Defines a test group
TEST_GROUP(group1)
{
	// Fixture setup
	TEST_SETUP()
	{
		printf("setup\n");
	}

	// A test
	TEST(foo)
	{
		printf("tests foo!\n");
	}

	// Another test
	TEST(bar)
	{
		printf("tests bar!\n");
	}
}

// Defines another test group
TEST_GROUP(group2)
{
	// A test
	TEST(baz)
	{
		printf("tests baz!\n");
	}

	// Fixture teardown
	TEST_TEARDOWN()
	{
		printf("teardown\n");
	}
}

// Run everything with Tests::getInstance().run()
