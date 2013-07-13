# Test runner proof of concept
#
# Copyright (c) 2013  Vlad Riscutia

from ctypes import *
import interop

try:
    sample = interop.Module('../test/msvc/Debug/test.dll')
except:
    sample = interop.Module('../test/gcc/libtest.so')

sample.load()

results = {
    0: '.',
    1: 'F',
    2: 'E',
    3: 'I'
}

for group in sample.tests:
    for test in sample.tests[group]:
        print("running %s/%s" % (group, test))
        result = sample.run_test(group, test)
        print(results[result])

