// More simple tests
//
// Copyright (c) 2013  Vlad Riscutia
#include "../test.hpp"

// Defines a test group
TEST_GROUP(group3)
{
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

