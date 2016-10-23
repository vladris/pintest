// Tests fixture setup/teardown
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License
#include "../test.hpp"


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
        test_assert::equals(1, value);
        value = 2;
    }

    TEST(second)
    {
        // Setup should've run first
        test_assert::equals(1, value);
        value = 2;
    }

    // Fixture teardown
    TEARDOWN()
    {
        // Test should've run before
        test_assert::equals(2, value);
    }
};