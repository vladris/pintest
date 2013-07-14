'''
    test_native
    ~~~~~~~~~~~

    Test native code by making sure native tests have expected results

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
from testhpp_runner import discovery, executor, interop
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
