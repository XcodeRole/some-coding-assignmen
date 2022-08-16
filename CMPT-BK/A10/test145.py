# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this
# file to a public or private website, or providing this file to a person
# not registered in CMPT 145, constitutes Academic Misconduct, according
# to the University of Saskatchewan Policy on Academic Misconduct.
#
# Synopsis:
#     scoring script for A6
#     student version
#
#     Run this script to see how many of your functions work!
#
# This script contains a lot of code that CMPT 145 students do not need
# understand.  It uses threading (CMPT 270), exceptions (CMPT 270),
# decorators (CMPT 370).  

####################################################################
# Script control for console output 
####################################################################

# Use the following variables to control the console output from this script
verbose = True # True turns on the verbose behaviour.
                # All errors and information is displayed on console
                # When set to False, only minimal information is displayed.

time_limit = 2  # In seconds, how many seconds a test case can use.
                # This time_limit allows the script to shut down a test case
                # if it runs longer than this.
                # Test cases in this script (below) should all complete in 
                # a few seconds, except if there is an infinite loop somewhere!
                # This limit is a little generous, because the thread library needs
                # some time to start up, and it might depend on the computer or OS.

failure_limit = 1   # How many test failures will cause the script to stop trying
                    # If this value is not positive, then the script will try all tests,
                    # no matter how many failures you have.
                    # If this value is positive, the script will stop trying after 
                    # encountering this many test failures or other problems.


####################################################################
# Code for running, and scoring, tests
# A bit complicated, and not examinable in CMPT 145
# Skip down to the test cases!
####################################################################

# scoring specific imports

import sys as sys
import threading
import _thread as thread
import traceback


# solution to the problem of infinite loops
# If a function has an infinite loop, then testing it might never complete!
# Solution:
# Run the test in a separate thread; halt the thread after a pre-defined
# amount of time has passed.  Assume that if the test exceeded the time limit,
# that a problem was encountered, usually an infinite loop.
# Source:
#     https://stackoverflow.com/questions/492519/timeout-on-a-function-call
# This is not an ideal solution since it uses _thread, and not a higher level
# module, e.g., threading.  I have no time to do it properly, so I am going
# with what we have.

def quit_function(fn_name):
    """
    Purpose:
        A function to halt the test thread, and report an error to the console.
    Pre-conditions:
        fn_name: the name of the function that was started in the test thread
    Return:
        (none)
    """
    print('{} took too long'.format(fn_name), file=sys.stdout)
    sys.stdout.flush()       # force the console output to display before interrupting
    thread.interrupt_main()  # raises KeyboardInterrupt


def exit_after(s):
    """
    Purpose:
        Decorator to exit process if function
        takes longer than s seconds
    Pre-conditions:
        s: positive number of seconds
    Return:
        A function that will halt after s seconds
    """
    def outer(fn):
        # returns a decorated function
        def inner(*args, **kwargs):
            # runs a function using the threading library
            timer = threading.Timer(s, quit_function, args=[fn.__name__])
            timer.start()
            try:
                result = fn(*args, **kwargs)
            finally:
                timer.cancel()
            return result
        return inner
    return outer


def report_import_error(title, missing, score):
    """
    Purpose:
        Report an error due to missing import file.
    Pre-conditions:
        title: a title for the set of tests
        missing: the name of the missing file
        score: the maximum score for the tests what would have been done
    Post-conditions:
        Reports missing file to the console, with a zero score.
    """
    print('*******************************************')
    print('{}'.format(title), end='')
    print(': {2}/{3}\nTests passed: {0}/{1}'.format(0, '??', 0, score))
    print("No file {}".format(missing))
    print('*******************************************')


####################################################################
# code to aid general scoring
####################################################################

    
def runemall(title, max_score, score_table, the_tests):
    """
    Purpose:
        Run all the tests, and count the ones that succeed.
        "run 'em all" not "rune mall"
        Reports:
            test case failures
            run time errors in the test cases
            interruptions for long run times
        Summarizes:
            The number of test cases passed
            A score, according to the score list
    Pre-conditions:
        title: a title for the set of tests
        the_tests: a list of test function objects
        score_table: a list of (t, s) tuples, 
                     where t is a number of tests passed, 
                           s is the corresponding score
        max_score: the number of points the question is worth
    Post-conditions:
        Reports the score for the tests to the console
        Reports error(s) and advice on the console.
    """
    count = 0
    passed = 0
    failures = 0

    for t in the_tests:
        try:
            count += 1
            # try calling the function t
            gettrace = getattr(sys, 'gettrace', None)
            if gettrace is None or not gettrace():
                # only set the limit if we're not debugging!
                runner = exit_after(time_limit)
                runnable = runner(t)
                runnable()
            else:
                # we're debugging, so don't set up for interrupt!
                t()
            passed += 1
        except AssertionError as e:
            # the test case failed (probably)
            failures += 1
            if verbose:
                print("***Test failure*** in test case:", t.__name__)
                print("---the test case failed")
                print("---your function did not do the right thing (incorrect return value or improper post-condition)")
                print(e)
            if failure_limit > 0 and failures >= failure_limit:
                break
        except KeyboardInterrupt as e:
            # the function call t() was interrupted (probably)
            failures += 1
            if verbose:
                print("***Had to terminate a function used in test case:", t.__name__)
                print("------took too long to finish")
                print("------Probable cause: one of your functions called in this test has an infinite loop")
            if failure_limit > 0 and failures >= failure_limit:
                break
        except Exception as e:
            # some exception not normally associated with a test case was raised
            failures += 1
            if verbose:
                print("***Runtime error (Exception) in test case:", t.__name__)
                print("---the test case did not complete")
                print("---Probable cause: one of your functions called in this test crashed before it could return an answer")
                print("---Possible alternative: one of your functions called in this test returned a value of the wrong type")
                print(traceback.format_exc())

            if failure_limit > 0 and failures >= failure_limit:
                break
        if verbose:
            print('Progress: {} tests passed, out of {} attempted.'.format(passed, count))

    print('----------------- Summary ------------------')
    print('{}'.format(title), end='')
    for (p, s) in score_table:
        if passed <= p:
            print(': {2}/{3}\nTests passed: {0}/{1}'.format(passed, count, s, max_score))
            break

    if failure_limit > 0 and failures == failure_limit:
        print("-*-*-Halted after {} failures.  Partial score reported above!-*-*-".format(failures))
        print('Setting failure_limit to 0 will run all the tests to complete the scoring.\n')

    if verbose:
        print('--------------------------------------------')
        print('To silence almost all console output from this script:')
        print('  - try setting verbose = False')
        print('If the number of errors reported is too high for your preference:')
        print('  - try setting failure_limit to a small positive integer')
        print('  - if you are just starting, try failure_limit = 1')
        print('  - the script will stop testing after this many failed test cases')
    elif not verbose and failures > 0:
        print('(to see more output, set verbose = True)')
