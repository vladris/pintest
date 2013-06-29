# Test runner proof of concept
#
# Copyright (c) 2013  Vlad Riscutia

from ctypes import *

sample = cdll.LoadLibrary('../sample/Debug/sample.dll')

# run_all = getattr(sample, '?run_all@Executor@test@@SAXXZ')

get_group = getattr(sample, '?get_group@Interop@test@@CAPBDPBD@Z')
get_test = getattr(sample, '?get_test@Interop@test@@CAPBDPBD0@Z')
run_test = getattr(sample, '?run_test@Interop@test@@CAXPBD0@Z') 

tests = dict()

group = get_group(None)
while group:
    tests[c_char_p(group).value] = []
    test = get_test(group, None)
    while test:
        tests[c_char_p(group).value].append(c_char_p(test).value)
        test = get_test(group, test)
    group = get_group(group)

for group in tests:
    for test in tests[group]:
        print("running %s/%s" % (group, test))
        run_test(c_char_p(group), c_char_p(test))

