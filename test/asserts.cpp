// Exercises asserts
//
// Copyright (c) 2013 Vlad Riscutia
// MIT License
#include "../test.hpp"


TEST_GROUP(asserts)
{
    // equals succeeds
    TEST(assert_equals_pass)
    {
        test_assert::equals(4, 2 + 2);
    }

    // equals fails
    TEST(assert_equals_fail)
    {
        test_assert::equals(5, 2 + 2);
    }

    // not_equals succeeds
    TEST(assert_notequals_pass)
    {
        test_assert::not_equals(5, 2 + 2);
    }

    // not_equals fails
    TEST(assert_notequals_fail)
    {
        test_assert::not_equals(4, 2 + 2);
    }

    // is_true succeeds
    TEST(assert_istrue_pass)
    {
        test_assert::is_true(2 + 2 == 4);
    }

    // is_true fails
    TEST(assert_istrue_fail)
    {
        test_assert::is_true(2 + 2 == 5);
    }

    // is_false succeeds
    TEST(assert_isfalse_pass)
    {
        test_assert::is_false(2 + 2 == 5);
    }

    // is_false fails
    TEST(assert_isfalse_fail)
    {
        test_assert::is_false(2 + 2 == 4);
    }

    // is_null succeeds
    TEST(assert_isnull_pass)
    {
        test_assert::is_null(nullptr);
    }

    // is_null fails
    TEST(assert_isnull_fail)
    {
        test_assert::is_null((void *)0xdeadbeef);
    }

    // throws succeeds
    TEST(assert_throws_pass)
    {
        test_assert::throws<int>([](){ throw 0; });
    }

    // throws fails (nothing thrown)
    TEST(assert_throws_fails_no_exception)
    {
        test_assert::throws<int>([]() { });
    }

    // throws fails (different exception)
    TEST(assert_throws_fails_different_exception)
    {
        test_assert::throws<std::string>([]() { throw 0; });
    }

    // fail
    TEST(assert_fail)
    {
        test_assert::fail();
    }
};
