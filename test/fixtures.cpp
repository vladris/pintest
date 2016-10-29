// Tests fixture setup/teardown
//
// Copyright (c) 2016 Vlad Riscutia
// MIT License
#include "../pintest.hpp"


TEST_GROUP(fixtures)
{
    int value = 0;

    // Fixture setup
    SETUP()
    {
        value = 1;
    }

    TEST(first)
    {
        // Setup should've run first
        test::assert::equals(1, value);
        value = 2;
    }

    TEST(second)
    {
        // Setup should've run first
        test::assert::equals(1, value);
        value = 2;
    }

    // Fixture teardown
    TEARDOWN()
    {
        // Test should've run before
        test::assert::equals(2, value);
    }
};