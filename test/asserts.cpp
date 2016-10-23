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
        test_assert::equals(4, 2 + 2);
    }

    // ASSERT_EQUALS that fails
    TEST(assert_equals_fail)
    {
        test_assert::equals(5, 2 + 2);
    }

    // ASSERT_NOTEQUALS that succeeds
    TEST(assert_notequals_pass)
    {
        test_assert::not_equals(5, 2 + 2);
    }

    // ASSERT_NOTEQUALS that fails
    TEST(assert_notequals_fail)
    {
        test_assert::not_equals(4, 2 + 2);
    }

    // ASSERT_ISTRUE that succeeds
    TEST(assert_istrue_pass)
    {
        test_assert::is_true(2 + 2 == 4);
    }

    // ASSERT_ISTRUE that fails
    TEST(assert_istrue_fail)
    {
        test_assert::is_true(2 + 2 == 5);
    }

    // ASSERT_ISFALSE that succeeds
    TEST(assert_isfalse_pass)
    {
        test_assert::is_false(2 + 2 == 5);
    }

    // ASSERT_ISFALSE that fails
    TEST(assert_isfalse_fail)
    {
        test_assert::is_false(2 + 2 == 4);
    }

    // ASSERT_ISNULL that succeeds
    TEST(assert_isnull_pass)
    {
        test_assert::is_null(nullptr);
    }

    // ASSERT_ISNULL that fails
    TEST(assert_isnull_fail)
    {
        test_assert::is_null((void *)0xdeadbeef);
    }
};
