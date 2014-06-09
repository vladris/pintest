// Exercises asserts
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License
#include "../test.hpp"


TEST_GROUP(asserts)
{
    // ASSERT_EQUALS that succeeds
    TEST(assert_equals_pass)
    {
        ASSERT_EQUALS(4, 2 + 2);
    }

    // ASSERT_EQUALS that fails
    TEST(assert_equals_fail)
    {
        ASSERT_EQUALS(5, 2 + 2);
    }

    // ASSERT_NOTEQUALS that succeeds
    TEST(assert_notequals_pass)
    {
        ASSERT_NOTEQUALS(5, 2 + 2);
    }

    // ASSERT_NOTEQUALS that fails
    TEST(assert_notequals_fail)
    {
        ASSERT_NOTEQUALS(4, 2 + 2);
    }

    // ASSERT_ISTRUE that succeeds
    TEST(assert_istrue_pass)
    {
        ASSERT_ISTRUE(2 + 2 == 4);
    }

    // ASSERT_ISTRUE that fails
    TEST(assert_istrue_fail)
    {
        ASSERT_ISTRUE(2 + 2 == 5);
    }

    // ASSERT_ISFALSE that succeeds
    TEST(assert_isfalse_pass)
    {
        ASSERT_ISFALSE(2 + 2 == 5);
    }

    // ASSERT_ISFALSE that fails
    TEST(assert_isfalse_fail)
    {
        ASSERT_ISFALSE(2 + 2 == 4);
    }

    // ASSERT_ISNULL that succeeds
    TEST(assert_isnull_pass)
    {
        ASSERT_ISNULL(nullptr);
    }

    // ASSERT_ISNULL that fails
    TEST(assert_isnull_fail)
    {
        ASSERT_ISNULL((void *)0xdeadbeef);
    }
};
