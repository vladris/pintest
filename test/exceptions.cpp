// Tests throwing exceptions
//
// Copyright (c) 2016 Vlad Riscutia
// MIT License
#include "../pintest.hpp"


TEST_GROUP(exceptions)
{
	// A test that throws an arbitrary exception
	TEST(throws_int)
	{
        throw -1;
	}
};