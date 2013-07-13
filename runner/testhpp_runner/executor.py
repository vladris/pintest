'''
    executor
    ~~~~~~~~

    Entry point for executing a single test

    :copyright: Copyright (c) 2013 Vlad Riscutia
    :license: MIT License
'''
import argparse
import codes
import interop
from subprocess import Popen, PIPE



def spawn_exec(module, test):
    '''
    Spawn a new process to execute the test
    '''
    proc = Popen(['runtest-exec', '-m', module, '-t', test], stdout=PIPE, stderr=PIPE)
    stdout, stderr = proc.communicate()
    return proc.returncode, stdout, stderr



def run_single_test(module, test):
    '''
    Runs a single test
    '''
    try:
        m = interop.Module(module)
    except:
        return codes.INVALID_MODULE
    group, test = test.split('/') 
    return m.run_test(group, test)



def main(argv=None):
    '''
    Entry point
    '''
    try:
        return parse(argv)
    except:
        return codes.RUNNER_EXCEPTION



def parse(argv):
    '''
    Parses command line
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--module', type=str, required=True,
        help='module containing test')
    parser.add_argument('-t', '--test', type=str, required=True,
        help='group/test to run (eg: -t foo/bar)')

    command = parser.parse_args(argv)
    return run_single_test(command.module, command.test)
