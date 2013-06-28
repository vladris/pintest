# Test runner proof of concept
#
# Copyright (c) 2013  Vlad Riscutia

from ctypes import *

sample = cdll.LoadLibrary('../sample/Debug/sample.dll')

# run_all = getattr(sample, '?run_all@Executor@test@@SAXXZ')

get_group = getattr(sample, '?get_group@Executor@test@@SAPBDPBD@Z')
get_test = getattr(sample, '?get_test@Executor@test@@SAPBDPBD0@Z')

tests = dict()

group = get_group(None)
while group:
    tests[c_char_p(group).value] = []
    test = get_test(group, None)
    while test:
        tests[c_char_p(group).value].append(c_char_p(test).value)
        test = get_test(group, test)
    group = get_group(group)


print(tests)

