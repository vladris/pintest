'''
    test_native
    ~~~~~~~~~~~

    Test native code by making sure native tests have expected results

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
from testhpp_runner import discovery, executor, interop, codes
from unittest import TestCase
from os import path as path


class NativeTests(TestCase):
    def test_discovery(self):
        '''
        Tests test discovery
        '''
        modules = discovery.discover('.')        

        # should discover one module 
        self.assertEquals(1, len(modules))

        module = modules[0]

        # make sure the right module was found 
        filename = path.basename(module.filename).lower()
        self.assertTrue(filename in ['libtest.so', 'test.dll'])

    def test_asserts(self):
        module = discovery.discover('.')[0]
        
        tests = {
            'assert_equals_pass': codes.SUCCESS,
            'assert_equals_fail': codes.FAILED,
            'assert_notequals_pass': codes.SUCCESS,
            'assert_notequals_fail': codes.FAILED,
            'assert_istrue_pass': codes.SUCCESS,
            'assert_istrue_fail': codes.FAILED,
            'assert_isfalse_pass': codes.SUCCESS,
            'assert_isfalse_fail': codes.FAILED,
            'assert_isnull_pass': codes.SUCCESS,
            'assert_isnull_fail': codes.FAILED,
        }

        for test in tests:
            self.assertEqual(tests[test], module.run_test('asserts', test))