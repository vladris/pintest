# Test runner proof of concept
#
# Copyright (c) 2013  Vlad Riscutia

import codes
from ctypes import *
import discovery
import interop
import executor
import sys

modules = discovery.discover('../..')

for module in modules:
    module.load()
    for group in module.tests:
        for test in module.tests[group]:
            r, stdo, stde = executor.spawn_exec(module.filename, group + "/" + test)
            sys.stdout.write(codes.RESULTS[r])

