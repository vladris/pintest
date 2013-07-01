// Asserts
//
// Copyright (c) 2013  Vlad Riscutia
#include "../test.hpp"

TEST_GROUP(asserts)
{
    TEST(assert_equals_true)
    {
        ASSERT_EQUALS(4, 2 + 2);
    }

    TEST(assert_equals_false)
    {
        ASSERT_EQUALS(5, 2 + 2);
    }
}
