# Test runner proof of concept
#
# Copyright (c) 2013  Vlad Riscutia

from ctypes import *
import interop
import discovery

modules = discovery.discover('..')

results = {
    0: '.',
    1: 'F',
    2: 'E',
    3: 'I'
}

for module in modules:
    module.load()
    for group in module.tests:
        for test in module.tests[group]:
            print("running %s/%s" % (group, test))
            result = module.run_test(group, test)
            print(results[result])

