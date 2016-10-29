// Exercises asserts
//
// Copyright (c) 2016 Vlad Riscutia
// MIT License
#include "../pintest.hpp"


TEST_GROUP(asserts)
{
    // equals succeeds
    TEST(assert_equals_pass)
    {
        test::assert::equals(4, 2 + 2);
    }

    // equals fails
    TEST(assert_equals_fail)
    {
        test::assert::equals(5, 2 + 2);
    }

    // not_equals succeeds
    TEST(assert_not_equals_pass)
    {
        test::assert::not_equals(5, 2 + 2);
    }

    // not_equals fails
    TEST(assert_not_equals_fail)
    {
        test::assert::not_equals(4, 2 + 2);
    }

    // is_true succeeds
    TEST(assert_is_true_pass)
    {
        test::assert::is_true(2 + 2 == 4);
    }

    // is_true fails
    TEST(assert_is_true_fail)
    {
        test::assert::is_true(2 + 2 == 5);
    }

    // is_false succeeds
    TEST(assert_is_false_pass)
    {
        test::assert::is_false(2 + 2 == 5);
    }

    // is_false fails
    TEST(assert_is_false_fail)
    {
        test::assert::is_false(2 + 2 == 4);
    }

    // is_null succeeds
    TEST(assert_is_null_pass)
    {
        test::assert::is_null(nullptr);
    }

    // is_null fails
    TEST(assert_is_null_fail)
    {
        test::assert::is_null((void *)0xdead);
    }

    // is_not_null succeeds
    TEST(assert_is_not_null_pass)
    {
        test::assert::is_not_null((void *)0xdead);
    }

    // is_not_null fails
    TEST(assert_is_not_null_fail)
    {
        test::assert::is_not_null(nullptr);
    }

    // throws succeeds
    TEST(assert_throws_pass)
    {
        test::assert::throws<int>([](){ throw 0; });
    }

    // throws fails (nothing thrown)
    TEST(assert_throws_fails_no_exception)
    {
        test::assert::throws<int>([]() { });
    }

    // throws fails (different exception)
    TEST(assert_throws_fails_different_exception)
    {
        test::assert::throws<std::string>([]() { throw 0; });
    }

    // fail
    TEST(assert_fail)
    {
        test::assert::fail();
    }
};
