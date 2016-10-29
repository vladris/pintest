'''
    interop
    ~~~~~~~

    Interop with native test modules

    :copyright: Copyright (c) 2016 Vlad Riscutia
    :license: MIT License
'''
from ctypes import cdll, c_char_p, c_int


class Module:
    '''
    Wrapper over a native test module
    '''
    def __init__(self, filename):
        '''
        Creates a new Module instance. This function raises an error if
        the module is not a valid test module (doesn't export the expected
        functions)
        '''
        self.__module = cdll.LoadLibrary(filename)

        self.__list_tests = self.__module.list_tests
        self.__list_tests.restype = c_char_p

        self.__run_test = self.__module.run_test

        self.filename = filename
        self.tests = []


    def load(self):
        '''
        Loads the tests from the module into a dictionary
        '''
        self.tests = self.__list_tests().decode('ascii').split(',')


    def run_test(self, group, test):
        '''
        Runs the given test from the given group
        '''
        group = group.encode('ascii')
        test = test.encode('ascii')

        return c_int(self.__run_test(c_char_p(group), c_char_p(test))).value
