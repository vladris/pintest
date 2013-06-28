# Test runner proof of concept
#
# Copyright (c) 2013  Vlad Riscutia

from ctypes import *

sample = cdll.LoadLibrary("../sample/Debug/sample.dll")

run_all = getattr(sample, "?run_all@Tests@@SAXXZ")

run_all()
