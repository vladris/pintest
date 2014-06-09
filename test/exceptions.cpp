// Tests throwing exceptions
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License
#include "../test.hpp"


TEST_GROUP(exceptions)
{
	// A test that throws an arbitrary exception
	TEST(throws_int)
	{
        throw -1;
	}
};