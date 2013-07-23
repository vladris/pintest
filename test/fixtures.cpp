// Tests fixture setup/teardown
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License
#include "../test.hpp"


TEST_GROUP(fixtures)
{
    int value = 0;

    // Fixture setup
    TEST_SETUP()
    {
        value = 1;
    }

    TEST(first)
    {
        // Setup should've run first
        ASSERT_EQUALS(1, value);
        value = 2;
    }

    TEST(second)
    {
        // Setup should've run first
        ASSERT_EQUALS(1, value);
        value = 2;
    }

    // Fixture teardown
    TEST_TEARDOWN()
    {
        // Test should've run before
        ASSERT_EQUALS(2, value);
    }
}