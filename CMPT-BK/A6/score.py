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
verbose = True  # True turns on the verbose behaviour.
                # All errors and information is displayed on console
                # When set to False, only minimal information is displayed.

time_limit = 2  # In seconds, how many seconds a test case can use.
                # This time_limit allows the script to shut down a test case
                # if it runs longer than this.
                # Test cases in this script (below) should all complete in 
                # a few seconds, except if there is an infinite loop somewhere!
                # This limit is a little generous, because the thread library needs
                # some time to start up, and it might depend on the computer or OS.

failure_limit = 0   # How many test failures will cause the script to stop trying
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

def runemall(title, the_tests, score_table):
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
        score: a list [(n,s), ...] where n, s are integers
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
    print('{}'.format(title),end='')
    max_score = max(s for t,s in score_table)
    for (p, s) in score_table:
        if passed <= p:
            print(': {2}/{3}\nTests passed: {0}/{1}'.format(passed, count, s, max_score))
            break
    print('--------------------------------------------')

    if failure_limit > 0 and failures == failure_limit:
        print("-*-*-Halted after {} failures.  Partial score reported above!-*-*-".format(failures))
        print('Setting failure_limit to 0 will run all the tests to complete the scoring.\n')

    if verbose:
        print('To silence almost all console output from this script:')
        print('  - try setting verbose = False')
        print('If the number of errors reported is too high for your preference:')
        print('  - try setting failure_limit to a small positive integer')
        print('  - if you are just starting, try failure_limit = 1')
        print('  - the script will stop testing after this many failed test cases')
    elif not verbose and failures > 0:
        print('(to see more output, set verbose = True)')


####################################################################
# A load of tests follows
####################################################################

# one string used to report any error
# format:  "<test item>: <reason>; found '<result>', expected '<expected>'"
assert_report = "{}: {}; found '{}', expected '{}'"


# basic tests for new LList object

def test_001():
    test_item = "LList()"
    reason = "new LList object _size should be 0"
    a_llist = L.LList()
    result = a_llist._size
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_002():
    test_item = "LList()"
    reason = "new LList object _head should be None"
    a_llist = L.LList()
    result = a_llist._head
    expected = None
    assert result is expected, assert_report.format(test_item, reason, result, expected)


def test_003():
    test_item = "LList()"
    reason = "new LList object _tail should be None"
    a_llist = L.LList()
    result = a_llist._tail
    expected = None
    assert result is expected, assert_report.format(test_item, reason, result, expected)


def test_004():
    test_item = "LList() + is_empty()"
    reason = "new LList object should be empty"
    # create a object
    the_llist = L.LList()

    # check if is_empty() works
    result = the_llist.is_empty()
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_005():
    test_item = "LList() + size()"
    reason = "new LList object should have size 0"
    # create a object
    the_llist = L.LList()

    # check if is_empty() works
    result = the_llist.size()
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_006():
    test_item = "LList() + is_empty()"
    reason = "singleton LList should not be empty"

    # create a node chain and list by hand
    the_node = L.node('arbitrary')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    # check if is_empty() works
    result = the_llist.is_empty()
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_007():
    test_item = "LList() + size()"
    reason = "singleton LList should have size 1"
    the_node = L.node('arbitrary')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    result = the_llist.size()
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.add_to_front()
###############################################################################################

def test_008():
    test_item = "add_to_front()"
    reason = "size after insertion on empty LList should be 1"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_009():
    test_item = "add_to_front()"
    reason = "head after insertion on empty LList should not be None"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist._head
    assert result is not None, assert_report.format(test_item, reason, result, "a non-None reference")


def test_010():
    test_item = "add_to_front()"
    reason = "tail after insertion on empty LList should not be None"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist._tail
    assert result is not None, assert_report.format(test_item, reason, result, "a non-None reference")


def test_011():
    test_item = "add_to_front()"
    reason = "head, tail after insertion on empty LList should be the same"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = (the_llist._head == the_llist._tail)
    expected = True
    assert result is expected, assert_report.format(test_item, reason, result, "a non-None reference")


def test_012():
    test_item = "add_to_front()"
    reason = "data at head after insertion on empty LList should be the value inserted"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist._head.data
    assert result is target, assert_report.format(test_item, reason, result, target)


def test_013():
    test_item = "add_to_front()"
    reason = "data at tail after insertion on empty LList should be the value inserted"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist._tail.data
    assert result is target, assert_report.format(test_item, reason, result, target)


def test_014():
    test_item = "add_to_front()"
    reason = "next reference after insertion on empty LList shold be None"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist._head.next
    assert result is None, '{}: {}; {}'.format(test_item, reason, "chain should end at one node, but next is not None!")


def test_015():
    test_item = "add_to_front()"
    reason = "after insertion on empty LList, should not be empty"
    the_llist = L.LList()
    target = 'one'
    the_llist.add_to_front(target)
    result = the_llist.is_empty()
    expected = False
    assert result is expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.add_to_front():
###############################################################################################

def test_016():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, size attribute should be 2"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = the_llist._size
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_017():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, the _head should change"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = (the_llist._head is not the_node)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_018():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, _tail should not change"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = (the_llist._tail is the_node)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_019():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, _head and _tail should be different"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = (the_llist._head is not the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_020():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, data in _head should be the value inserted"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_021():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, next for in _head should not be None"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = (the_llist._head.next is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_022():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, next for in _head should be the the initial node"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = (the_llist._head.next is the_node)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_023():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, data in _tail should be the the initial data"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = the_llist._tail.data
    expected = initial_data
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_024():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, should not be empty"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = the_llist.is_empty()
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_025():
    test_item = "add_to_front()"
    reason = "after insertion on LList with one node, size should be 2"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'two'
    the_llist.add_to_front(target)
    result = the_llist.size()
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.add_to_back()
###############################################################################################

def test_026():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, _head should not be None"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = (the_llist._head is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_027():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, _tail should not be None"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = (the_llist._tail is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_028():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, _size should be 1"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_029():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, _head, _tail should be the same"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = (the_llist._head is the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_030():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, data at _head should be the value inserted"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_031():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, data at _tail should be the value inserted"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = the_llist._tail.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_032():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, the next for _head should be None"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = the_llist._head.next
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_033():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, is_empty() should return False"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = the_llist.is_empty()
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_034():
    test_item = "add_to_back()"
    reason = "after insertion on empty LList, size() should return 1"
    the_llist = L.LList()
    target = 'three'
    the_llist.add_to_back(target)
    result = the_llist.size()
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_035():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, _size should be 2"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = the_llist._size
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_036():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, _head should be the inserted node"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = (the_llist._head is the_node)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_037():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, _tail should not be None"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = (the_llist._tail is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_038():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, _tail should not be the initial node"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = (the_llist._tail is not the_node)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_039():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, _head should not be _tail"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = (the_llist._head is not the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_040():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, data at _tail should be the inserted value"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = the_llist._tail.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_041():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, data at _head should be the initial value"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = the_llist._head.data
    expected = initial_data
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_042():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, next for _head should not be None"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = (the_llist._head.next is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_043():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, next for _head should be _tail"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = (the_llist._head.next is the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_044():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, is_empty() should return False"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = the_llist.is_empty()
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_045():
    test_item = "add_to_back()"
    reason = "after insertion on LList with one node, size() should return 2"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'four'
    the_llist.add_to_back(target)
    result = the_llist.size()
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.value_is_in()
###############################################################################################

def test_046():
    test_item = "value_is_in()"
    reason = "on empty LList, should not find any target"
    the_llist = L.LList()
    target = 5
    result = the_llist.value_is_in(target)
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_047():
    test_item = "value_is_in()"
    reason = "on LList with one node, target not present, should not find the target"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'six'
    result = the_llist.value_is_in(target)
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_048():
    test_item = "value_is_in()"
    reason = "on LList with one node, target present, should find the target"
    target = '7'
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    result = the_llist.value_is_in(target)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_049():
    test_item = "value_is_in()"
    reason = "on LList with two nodes, target not present, should not find the target"
    target = '7'
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    result = the_llist.value_is_in(target)
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_050():
    test_item = "value_is_in()"
    reason = "on LList with two nodes, target in tail, should find the target"
    target = '7'
    the_tail = L.node(target)
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    result = the_llist.value_is_in(target)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_051():
    test_item = "value_is_in()"
    reason = "on LList with two nodes, target in head, should find the target"
    target = '7'
    the_tail = L.node('not the target')
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    result = the_llist.value_is_in(target)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.get_index_of_value()
###############################################################################################

def test_052():
    test_item = "get_index_of_value()"
    reason = "on empty LList, should not find any target (flag should be False)"
    the_llist = L.LList()
    target = 9
    flag, idx = the_llist.get_index_of_value(target)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_053():
    test_item = "get_index_of_value()"
    reason = "on empty LList, should not find any target (index should be None)"
    the_llist = L.LList()
    target = 9
    flag, idx = the_llist.get_index_of_value(target)
    result = idx
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_054():
    test_item = "get_index_of_value()"
    reason = "on singleton LList, target not present, should not find the target (flag should be False)"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'six'
    flag, idx = the_llist.get_index_of_value(target)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_055():
    test_item = "get_index_of_value()"
    reason = "on singleton LList, target not present, should not find the target (index should be None)"
    initial_data = 'not the target'
    the_node = L.node(initial_data)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    target = 'six'
    flag, idx = the_llist.get_index_of_value(target)
    result = idx
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_056():
    test_item = "get_index_of_value()"
    reason = "on singleton LList, target present, should find the target (flag should be True)"
    target = '10'
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, idx = the_llist.get_index_of_value(target)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_057():
    test_item = "get_index_of_value()"
    reason = "on singleton LList, target present, should find the target (index should be 0)"
    target = '10'
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, idx = the_llist.get_index_of_value(target)
    result = idx
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_058():
    test_item = "get_index_of_value()"
    reason = "on LList with 2 nodes, target not present, should not find the target (flag should be False)"
    target = '10'
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, idx = the_llist.get_index_of_value(target)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_059():
    test_item = "get_index_of_value()"
    reason = "on LList with 2 nodes, target not present, should not find the target (index should be None)"
    target = '10'
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, idx = the_llist.get_index_of_value(target)
    result = idx
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_060():
    test_item = "get_index_of_value()"
    reason = "on LList with 2 nodes, target in tail, should find the target (flag should be True)"
    target = '10'
    the_tail = L.node(target)
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, idx = the_llist.get_index_of_value(target)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_061():
    test_item = "get_index_of_value()"
    reason = "on LList with 2 nodes, target in tail, should find the target (index should be 1)"
    target = '10'
    the_tail = L.node(target)
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, idx = the_llist.get_index_of_value(target)
    result = idx
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_062():
    test_item = "get_index_of_value()"
    reason = "on LList with 2 nodes, target in head, should find the target (flag should be True)"
    target = '10'
    the_tail = L.node('not the target')
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, idx = the_llist.get_index_of_value(target)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_063():
    test_item = "get_index_of_value()"
    reason = "on LList with 2 nodes, target in head, should find the target (index should be 0)"
    target = '10'
    the_tail = L.node('not the target')
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, idx = the_llist.get_index_of_value(target)
    result = idx
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.retrieve_data_at_index()
###############################################################################################

def test_064():
    test_item = "retrieve_data_at_index()"
    reason = "on empty LList, should not return a value (flag should be False)"
    the_llist = L.LList()
    idx = 0
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_065():
    test_item = "retrieve_data_at_index()"
    reason = "on empty LList, should not return a value (value should be None)"
    the_llist = L.LList()
    idx = 0
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = val
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_066():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with one node, valid index (0), should return a value (flag should be True)"
    target = 12
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_067():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with one node, valid index (0), should return a value"
    target = 12
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_068():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with two nodes, valid index (0), should return a value (flag should be True)"
    the_tail = L.node(16)
    the_head = L.node(18, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 0
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_069():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with two nodes, valid index (0), should return a value"
    target = 18
    the_tail = L.node(16)
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 0
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_070():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with two nodes, valid index (1), should return a value (flag should be True)"
    the_tail = L.node(16)
    the_head = L.node(18, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 1
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_071():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with two nodes, valid index (1), should return a value"
    target = 16
    the_tail = L.node(target)
    the_head = L.node(18, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 1
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_072():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with two nodes, invalid index (2), should not return a value (flag should be False)"
    the_tail = L.node(16)
    the_head = L.node(18, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 2
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_073():
    test_item = "retrieve_data_at_index()"
    reason = "on LList with two nodes, invalid index (2), should not return a value (value should be None)"
    the_tail = L.node(16)
    the_head = L.node(18, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 2
    flag, val = the_llist.retrieve_data_at_index(idx)
    result = val
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.set_data_at_index()
###############################################################################################

def test_074():
    test_item = "set_data_at_index()"
    reason = "on empty LList, invalid index (0), should not succeed (flag should be False)"
    the_llist = L.LList()
    idx = 0
    target = 23
    flag = the_llist.set_data_at_index(idx, target)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_075():
    test_item = "set_data_at_index()"
    reason = "on LList with one node, valid index (0), should succeed (flag should be True)"
    target = 23
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    idx = 0
    flag = the_llist.set_data_at_index(idx, target)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_076():
    test_item = "set_data_at_index()"
    reason = "on LList with one node, valid index (0), should succeed (data should be set)"
    target = 23
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    idx = 0
    flag = the_llist.set_data_at_index(idx, target)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_077():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, valid index (0), should succeed (flag should be True)"
    target = 23
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 0
    result = the_llist.set_data_at_index(idx, target)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_078():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, valid index (0), should succeed (data should be set)"
    target = 23
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 0
    flag = the_llist.set_data_at_index(idx, target)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_079():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, valid index (1), should succeed (flag should be True)"
    target = 23
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 1
    flag = the_llist.set_data_at_index(idx, target)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_080():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, valid index (0), should succeed (data should be set)"
    target = 23
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 1
    flag = the_llist.set_data_at_index(idx, target)
    result = the_llist._tail.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_081():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, invalid index (2), should not succeed (flag should be False)"
    target = 23
    the_tail = L.node('not the target')
    the_head = L.node('not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 2
    flag = the_llist.set_data_at_index(idx, target)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_082():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, invalid index (2), should not succeed (data at head should not change)"
    target = 23
    data1 = 'not here'
    data2 = 'not there'
    the_tail = L.node(data1)
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 2
    flag = the_llist.set_data_at_index(idx, target)
    result = the_llist._head.data
    expected = data2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_083():
    test_item = "set_data_at_index()"
    reason = "on LList with two nodes, invalid index (2), should not succeed (data at tail should not change)"
    target = 23
    data1 = 'not here'
    data2 = 'not there'
    the_tail = L.node(data1)
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    idx = 2
    flag = the_llist.set_data_at_index(idx, target)
    result = the_llist._tail.data
    expected = data1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.remove_from_front()
###############################################################################################

def test_084():
    test_item = "remove_from_front()"
    reason = "on empty LList, should not have effect (flag should be False)"
    the_llist = L.LList()
    flag, val = the_llist.remove_from_front()
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_085():
    test_item = "remove_from_front()"
    reason = "on empty LList, should not have effect (value should be None)"
    the_llist = L.LList()
    flag, val = the_llist.remove_from_front()
    result = val
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_086():
    test_item = "remove_from_front()"
    reason = "on LList with one node, removal should work (flag should be True)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_front()
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_087():
    test_item = "remove_from_front()"
    reason = "on LList with one node, removal should work (value should be known value)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_front()
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_088():
    test_item = "remove_from_front()"
    reason = "on LList with one node, removal should work (_size should change)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_front()
    result = the_llist._size
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_089():
    test_item = "remove_from_front()"
    reason = "on LList with one node, removal should work (_head should be None)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_front()
    result = the_llist._head
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_090():
    test_item = "remove_from_front()"
    reason = "on LList with one node, removal should work (_tail should be None)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_front()
    result = the_llist._tail
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_091():
    test_item = "remove_from_front()"
    reason = "on LList with two nodes, removal should work (flag should be True)"
    the_tail = L.node(29)
    target = 33
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_front()
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_092():
    test_item = "remove_from_front()"
    reason = "on LList with two nodes, removal should work (value should be known)"
    the_tail = L.node(29)
    target = 33
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_front()
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_093():
    test_item = "remove_from_front()"
    reason = "on LList with two nodes, removal should work (_size should change)"
    the_tail = L.node(29)
    target = 33
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_front()
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_094():
    test_item = "remove_from_front()"
    reason = "on LList with two nodes, removal should work (_head and _tail should be same)"
    the_tail = L.node(29)
    target = 33
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_front()
    result = (the_llist._head is the_tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)

def test_095():
    test_item = "remove_from_front()"
    reason = "on LList with two nodes, removal should work (_tail should not change)"
    the_tail = L.node(29)
    target = 33
    the_head = L.node(target, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_front()
    result = (the_llist._tail is the_tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# UNIT TESTING - LList.remove_from_back()
###############################################################################################

def test_096():
    test_item = "remove_from_back()"
    reason = "on empty LList, should not have effect (flag should be False)"
    the_llist = L.LList()
    flag, val = the_llist.remove_from_back()
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_097():
    test_item = "remove_from_back()"
    reason = "on empty LList, should not have effect (value should be None)"
    the_llist = L.LList()
    flag, val = the_llist.remove_from_back()
    result = val
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_098():
    test_item = "remove_from_back()"
    reason = "on LList with one node, removal should work (flag should be True)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_back()
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_099():
    test_item = "remove_from_back()"
    reason = "on LList with one node, removal should work (value should be known value)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_back()
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_100():
    test_item = "remove_from_back()"
    reason = "on LList with one node, removal should work (_size should change)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_back()
    result = the_llist._size
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_101():
    test_item = "remove_from_back()"
    reason = "on LList with one node, removal should work (_head should be None)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_back()
    result = the_llist._head
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_102():
    test_item = "remove_from_back()"
    reason = "on LList with one node, removal should work (_tail should be None)"
    target = 25
    the_node = L.node(target)
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node

    flag, val = the_llist.remove_from_back()
    result = the_llist._tail
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_103():
    test_item = "remove_from_back()"
    reason = "on LList with two nodes, removal should work (flag should be True)"
    target = 29
    the_tail = L.node(target)
    the_head = L.node(33, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_back()
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_104():
    test_item = "remove_from_back()"
    reason = "on LList with two nodes, removal should work (value should be known)"
    target = 29
    the_tail = L.node(target)
    the_head = L.node(33, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_back()
    result = val
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_105():
    test_item = "remove_from_back()"
    reason = "on LList with two nodes, removal should work (_size should change)"
    target = 29
    the_tail = L.node(target)
    the_head = L.node(33, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_back()
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_106():
    test_item = "remove_from_back()"
    reason = "on LList with two nodes, removal should work (_head should not change)"
    the_tail = L.node(29)
    the_head = L.node(33, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_back()
    result = (the_llist._head is the_head)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_107():
    test_item = "remove_from_back()"
    reason = "on LList with two nodes, removal should work (_head and _tail should be same)"
    the_tail = L.node(29)
    the_head = L.node(33, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail

    flag, val = the_llist.remove_from_back()
    result = (the_llist._tail is the_head)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)



###############################################################################################
# UNIT TESTING - LList.insert_value_at_index()
###############################################################################################


def test_108():
    test_item = "insert_value_at_index()"
    reason = "on empty LList, insertion at valid index (flag should be True)"
    the_llist = L.LList()
    idx = 0
    target = 'one'
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_109():
    test_item = "insert_value_at_index()"
    reason = "on empty LList, insertion at valid index (_size should change)"
    the_llist = L.LList()
    idx = 0
    target = 'one'
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_110():
    test_item = "insert_value_at_index()"
    reason = "on empty LList, insertion at valid index (_head and _tail should be same)"
    the_llist = L.LList()
    idx = 0
    target = 'one'
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head is the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_111():
    test_item = "insert_value_at_index()"
    reason = "on empty LList, insertion at valid index (value accessible at _head)"
    the_llist = L.LList()
    idx = 0
    target = 'one'
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_112():
    test_item = "insert_value_at_index()"
    reason = "on empty LList, insertion at valid index (value accessible at _tail)"
    the_llist = L.LList()
    idx = 0
    target = 'one'
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_113():
    test_item = "insert_value_at_index()"
    reason = "on empty LList, insertion at valid index (next for node should be None)"
    the_llist = L.LList()
    idx = 0
    target = 'one'
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.next
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_114():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 0 (flag should be True)"
    target = 'two'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_115():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 0 (new data at head)"
    target = 'two'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_116():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 0 (next from _head should not be None)"
    target = 'two'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head.next is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_117():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 0 (_size should increase)"
    target = 'two'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_118():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 0 (_head _tail should be different)"
    target = 'two'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head is not the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_119():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 0 (next from _tail should be None)"
    target = 'two'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.next
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_120():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (flag should be True)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_121():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (old data should not be at the head)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head.data != target)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_122():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (new data should  be at the head)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_123():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (next for head should not be None)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head.next is not None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_124():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (_size should change)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_125():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (_head _tail should be different)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head is not the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_126():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at valid index 1 (next of _tail should be None)"
    target = 'three'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.next
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_127():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at invalid index 2 (flag should be False)"
    target = 'four'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_128():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at invalid index 2 (_size should not change)"
    target = 'four'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_129():
    test_item = "insert_value_at_index()"
    reason = "on LList with one node, insertion at invalid index 2 (_head _tail should be the same)"
    target = 'four'
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    the_llist._tail = the_node
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head is the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_130():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 0 (flag should be the True)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'five'
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_131():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 0 (new data should be at head)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'five'
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_132():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 0 (_size should increase)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'five'
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 3
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_133():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 0 (next of _tail should be None)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'five'
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.next
    expected = None
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_134():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 0 (next of _head should be previous head)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'five'
    idx = 0
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head.next is the_head)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_135():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 1 (flag should be True)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'six'
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_136():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 1 (data at _head should not change)"
    the_tail = L.node('not the target')
    data2 = 'still not the target'
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'six'
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.data
    expected = data2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_137():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 1 (new data should be at next of _head)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'six'
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.next.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_138():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 1 (_size should increase)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'six'
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 3
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_139():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 1 (next of _tail should be None)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'six'
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._tail.next is None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_140():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 1 (_head should be previous head)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'five'
    idx = 1
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head is the_head)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_141():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 2 (flag should be True)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_142():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 2 (data at head should not change)"
    the_tail = L.node('not the target')
    data2 = 'still not the target'
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.data
    expected = data2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_143():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 2 (data at 2nd node should not change)"
    data1 = 'not the target'
    the_tail = L.node(data1)
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.next.data
    expected = data1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_144():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 2 (new data should be at _tail)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.data
    expected = target
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_145():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 2 (_size should increase)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 3
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_146():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at valid index 2 (next of _tail should be None)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 2
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._tail.next is None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_147():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (flag should be False)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_148():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (_size should not change)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._size
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_149():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (_head should not change)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head is the_head)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)

def test_150():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (_tail should not change)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._tail is the_tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_151():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (next of head should be tail)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = (the_llist._head.next is the_tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_152():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (data at head should not change)"
    the_tail = L.node('not the target')
    data2 = 123
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._head.data
    expected = data2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_153():
    test_item = "insert_value_at_index()"
    reason = "on LList with two nodes, insertion at invalid index 3 (data at tail should not change)"
    data1 = 456
    the_tail = L.node(data1)
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    target = 'nine'
    idx = 3
    flag = the_llist.insert_value_at_index(target, idx)
    result = the_llist._tail.data
    expected = data1
    assert result == expected, assert_report.format(test_item, reason, result, expected)



###############################################################################################
# UNIT TESTING - LList.delete_item_at_index()
###############################################################################################

def test_154():
    test_item = "delete_item_at_index()"
    reason = "on empty LList, removal (flag should be False)"
    the_llist = L.LList()
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_155():
    test_item = "delete_item_at_index()"
    reason = "on empty LList, removal (_size should not change)"
    the_llist = L.LList()
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._size
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_156():
    test_item = "delete_item_at_index()"
    reason = "on empty LList, removal (_head should be None)"
    the_llist = L.LList()
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._head is None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)

def test_157():
    test_item = "delete_item_at_index()"
    reason = "on empty LList, removal (_tail should be None)"
    the_llist = L.LList()
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._tail is None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_158():
    test_item = "delete_item_at_index()"
    the_node = L.node('not the target')
    reason = "on LList with one node, removal at valid index 0 (flag should be True)"
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_159():
    test_item = "delete_item_at_index()"
    reason = "on LList with one node, removal at valid index 0 (_head should be None)"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._head is None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_160():
    test_item = "delete_item_at_index()"
    reason = "on LList with one node, removal at valid index 0 (_size should decrease)"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._size
    expected = 0
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_161():
    test_item = "delete_item_at_index()"
    reason = "on LList with one node, removal at valid index 0 (_tail should be None)"
    the_node = L.node('not the target')
    the_llist = L.LList()
    the_llist._size = 1
    the_llist._head = the_node
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._tail is None)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_162():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 0 (flag should be True)"
    the_tail = L.node('ten')
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_163():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 0 (data at _head should be known)"
    data1 = 'ten'
    the_tail = L.node(data1)
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._head.data
    expected = data1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_164():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 0 (_size should decrease)"
    the_tail = L.node('ten')
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_165():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 0 (_head _tail should be identical)"
    the_tail = L.node('ten')
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 0
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._head is the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_166():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 1 (flag should be true)"
    the_tail = L.node('ten')
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 1
    flag = the_llist.delete_item_at_index(idx)
    result = flag
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_167():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 1 (data at head should be known)"
    the_tail = L.node('ten')
    data2 = 'twelve'
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 1
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._head.data
    expected = data2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_168():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 1 (_size should decrease)"
    the_tail = L.node('ten')
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 1
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._size
    expected = 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_169():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at valid index 1 (_head _tail should be identical)"
    the_tail = L.node('ten')
    the_head = L.node('twelve', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 1
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._head is the_llist._tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_170():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (flag should be False)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = flag
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_171():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (_size should not change)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._size
    expected = 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_172():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (_head should not change)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._head is the_head)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)

def test_173():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (_tail should not change)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._tail is the_tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_174():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (next of head should be tail)"
    the_tail = L.node('not the target')
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = (the_llist._head.next is the_tail)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_175():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (data at head should not change)"
    the_tail = L.node('not the target')
    data2 = 123
    the_head = L.node(data2, the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._head.data
    expected = data2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_176():
    test_item = "delete_item_at_index()"
    reason = "on LList with two nodes, removal at invalid index 3 (data at tail should not change)"
    data1 = 456
    the_tail = L.node(data1)
    the_head = L.node('still not the target', the_tail)
    the_llist = L.LList()
    the_llist._size = 2
    the_llist._head = the_head
    the_llist._tail = the_tail
    idx = 3
    flag = the_llist.delete_item_at_index(idx)
    result = the_llist._tail.data
    expected = data1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# INTEGRATION TESTING
###############################################################################################

###############################################################################################
# check if all the operations work after a bunch of data is added using add_to_back()
#

def test_177():
    test_item = "multiple add_to_back() followed by is_empty()"
    reason = "LList should not be empty"

    # an integration test tests how operations work together
    # first set up a LList with a bunch of nodes in the node chain
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)

    # now check if a single operation works properly
    result = the_llist.is_empty()
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_178():
    test_item = "multiple add_to_back() followed by size()"
    reason = "LList should have known size"
    # common set up
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)

    # check one aspect
    result = the_llist.size()
    expected = len(stuff)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_179():
    test_item = "multiple add_to_back() followed by value_is_in()"
    reason = "should be able to find the first value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    result = the_llist.value_is_in(stuff[idx])
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_180():
    test_item = "multiple add_to_back() followed by value_is_in()"
    reason = "should be able to find the 2nd value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 1
    result = the_llist.value_is_in(stuff[idx])
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_181():
    test_item = "multiple add_to_back() followed by value_is_in()"
    reason = "should be able to find the last value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    result = the_llist.value_is_in(stuff[idx])
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_182():
    test_item = "multiple add_to_back() followed by value_is_in()"
    reason = "should not be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    result = the_llist.value_is_in("not in the LList")
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_183():
    test_item = "multiple add_to_back() followed by get_index_of_value()"
    reason = "should be able to find the 1st value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_184():
    test_item = "multiple add_to_back() followed by get_index_of_value()"
    reason = "should be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 3
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_185():
    test_item = "multiple add_to_back() followed by get_index_of_value()"
    reason = "should be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_186():
    test_item = "multiple add_to_back() followed by get_index_of_value()"
    reason = "should not be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    result = the_llist.get_index_of_value("this value is not in the LList")
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_187():
    test_item = "multiple add_to_back() followed by retrieve_data_at_index()"
    reason = "should be able to retrieve the last value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    result = the_llist.retrieve_data_at_index(idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_188():
    test_item = "multiple add_to_back() followed by retrieve_data_at_index()"
    reason = "should be able to retrieve the first value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    result = the_llist.retrieve_data_at_index(idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_189():
    test_item = "multiple add_to_back() followed by retrieve_data_at_index()"
    reason = "should be able to retrieve a middle value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 2
    result = the_llist.retrieve_data_at_index(idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_190():
    test_item = "multiple add_to_back() followed by retrieve_data_at_index()"
    reason = "should not be able to retrieve for bad index"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) + 2
    result = the_llist.retrieve_data_at_index(idx)
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# check if all the operations work after a bunch of data is added using add_to_front()
#

def test_191():
    test_item = "multiple add_to_front() followed by is_empty()"
    reason = "LList should not be empty"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    result = the_llist.is_empty()
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_192():
    test_item = "multiple add_to_front() followed by size()"
    reason = "LList should have known size"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    result = the_llist.size()
    expected = len(stuff)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_193():
    test_item = "multiple add_to_front() followed by value_is_in()"
    reason = "should be able to find the first value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = 0
    result = the_llist.value_is_in(stuff[idx])
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_194():
    test_item = "multiple add_to_front() followed by value_is_in()"
    reason = "should be able to find the 2nd value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = 1
    result = the_llist.value_is_in(stuff[idx])
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_195():
    test_item = "multiple add_to_front() followed by value_is_in()"
    reason = "should be able to find the last value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = len(stuff) - 1
    result = the_llist.value_is_in(stuff[idx])
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_196():
    test_item = "multiple add_to_front() followed by value_is_in()"
    reason = "should not be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    result = the_llist.value_is_in("not in the LList")
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_197():
    test_item = "multiple add_to_front() followed by get_index_of_value()"
    reason = "should be able to find the 1st value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = 0
    # the position of the item in the LList is different because add_to_front pushes data backwards
    rev_idx = len(stuff) - 1 - idx
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, rev_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_198():
    test_item = "multiple add_to_front() followed by get_index_of_value()"
    reason = "should be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = 3
    # the position of the item in the LList is different because add_to_front pushes data backwards
    rev_idx = len(stuff) - 1 - idx
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, rev_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_199():
    test_item = "multiple add_to_front() followed by get_index_of_value()"
    reason = "should be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = len(stuff) - 1
    # the position of the item in the LList is different because add_to_front pushes data backwards
    rev_idx = len(stuff) - 1 - idx
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, rev_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_200():
    test_item = "multiple add_to_back() followed by get_index_of_value()"
    reason = "should not be able to find the value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    result = the_llist.get_index_of_value("this value is not in the LList")
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_201():
    test_item = "multiple add_to_front() followed by retrieve_data_at_index()"
    reason = "should be able to retrieve the last value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = len(stuff) - 1
    rev_idx = len(stuff) - 1 - idx
    result = the_llist.retrieve_data_at_index(rev_idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_202():
    test_item = "multiple add_to_front() followed by retrieve_data_at_index()"
    reason = "should be able to retrieve the first value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = 0
    rev_idx = len(stuff) - 1 - idx
    result = the_llist.retrieve_data_at_index(rev_idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_203():
    test_item = "multiple add_to_front() followed by retrieve_data_at_index()"
    reason = "should be able to retrieve a middle value"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = 2
    rev_idx = len(stuff) - 1 - idx
    result = the_llist.retrieve_data_at_index(rev_idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_204():
    test_item = "multiple add_to_front() followed by retrieve_data_at_index()"
    reason = "should not be able to retrieve for bad index"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    idx = len(stuff) + 2
    result = the_llist.retrieve_data_at_index(idx)
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# check what happens if you change the data in the Llist using set_data_at_index()
# from here down, we assume that both add_to_back() and add_to_front() work correctly.
# we're testing other operations.  We're using add_to_back() because it's a little easier
# to calculate indices than if we used add_to_front()

def test_205():
    test_item = "set_data_at_index()"
    reason = "at index 0, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "this is here now"
    result = the_llist.set_data_at_index(idx, new_data)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_206():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "should not find old data at index 0"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_207():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "should  find new data at index 0"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.get_index_of_value(new_data)
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_208():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "should find old data at index 1 after change at index 0"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = 0
    check_idx = 1
    new_data = "this is here now"
    the_llist.set_data_at_index(change_idx, new_data)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_209():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "should find old data at end after change at index 0"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = 0
    check_idx = len(stuff) - 1
    new_data = "this is here now"
    the_llist.set_data_at_index(change_idx, new_data)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_210():
    test_item = "set_data_at_index() then size()"
    reason = "change to front data, size should not change"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.size()
    expected = len(stuff)
    assert result == expected, assert_report.format(test_item, reason, result, expected)



def test_211():
    test_item = "set_data_at_index()"
    reason = "change at last item, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    new_data = "this is here now"
    result = the_llist.set_data_at_index(idx, new_data)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_212():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at last item, should not find old data there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_213():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at last item, should find new data there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.get_index_of_value(new_data)
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_214():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at last item, should find old data at index 0"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = len(stuff) - 1
    check_idx = 0
    new_data = "this is here now"
    the_llist.set_data_at_index(change_idx, new_data)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_215():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at last item, should find old data just in front of last item"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = len(stuff) - 1
    check_idx = len(stuff) - 2
    new_data = "this is here now"
    the_llist.set_data_at_index(change_idx, new_data)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_216():
    test_item = "set_data_at_index() then size()"
    reason = "change to back data, size should not change"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) - 1
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.size()
    expected = len(stuff)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_217():
    test_item = "set_data_at_index()"
    reason = "change at middle item, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) // 2
    new_data = "this is here now"
    result = the_llist.set_data_at_index(idx, new_data)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_218():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at middle item, should not find old data there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) // 2
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_219():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at middle item, should find new data there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) // 2
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.get_index_of_value(new_data)
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_220():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at middle item, should find old data just before it"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = len(stuff) // 2
    check_idx = change_idx - 1
    new_data = "this is here now"
    the_llist.set_data_at_index(change_idx, new_data)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_221():
    test_item = "set_data_at_index() then get_index_of_value()"
    reason = "change at middle item, should find old data just after it"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = len(stuff) // 2
    check_idx = change_idx + 1
    new_data = "this is here now"
    the_llist.set_data_at_index(change_idx, new_data)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_222():
    test_item = "set_data_at_index() then size()"
    reason = "change to middle data, size should not change"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = len(stuff) // 2
    new_data = "this is here now"
    the_llist.set_data_at_index(idx, new_data)
    result = the_llist.size()
    expected = len(stuff)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# check what happens as a bunch of insert_value_at() operations are used on a LList

def test_223():
    test_item = "insert_value_at_index()"
    reason = "insert new value at index 0, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index 0"
    result = the_llist.insert_value_at_index(new_data, idx)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_224():
    test_item = "insert_value_at_index()"
    reason = "insert new value at index 0, new value should be first"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    result = the_llist.get_index_of_value(new_data)
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_225():
    test_item = "insert_value_at_index()"
    reason = "insert new value at index 0, old first value should be second"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    change_idx = 0
    check_idx = 0
    new_data = "a new datum added at index {}".format(change_idx)
    the_llist.insert_value_at_index(new_data, change_idx)
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx+1)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_226():
    test_item = "insert_value_at_index()"
    reason = "insert new value at index 0, size should increase"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    result = the_llist.size()
    expected = len(stuff) + 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_227():
    test_item = "insert_value_at_index()"
    reason = "insert new values first and last, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    result = the_llist.insert_value_at_index(new_data2, idx2)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_228():
    test_item = "insert_value_at_index()"
    reason = "insert new values first and last, should find new data last"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    result = the_llist.get_index_of_value(new_data2)
    expected = (True, idx2)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_229():
    test_item = "insert_value_at_index()"
    reason = "insert new values first and last, should find old data shifted by one"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    check_idx = 6
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx+1)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_230():
    test_item = "insert_value_at_index()"
    reason = "insert new values first and last, size should increase"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    result = the_llist.size()
    expected = len(stuff) + 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_231():
    test_item = "insert_value_at_index()"
    reason = "insert new values first, last, and middle; flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    idx3 = the_llist.size() // 2
    new_data3 = "a new datum added at index {}".format(idx3)
    result = the_llist.insert_value_at_index(new_data3, idx3)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_232():
    test_item = "insert_value_at_index()"
    reason = "insert new values first, last, and middle; should find new data at middle"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    idx3 = the_llist.size() // 2
    new_data3 = "a new datum added at index {}".format(idx3)
    the_llist.insert_value_at_index(new_data3, idx3)
    result = the_llist.get_index_of_value(new_data3)
    expected = (True, idx3)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_233():
    test_item = "insert_value_at_index()"
    reason = "insert new values first, last, and middle; should find old data shifted"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    idx3 = the_llist.size() // 2
    new_data3 = "a new datum added at index {}".format(idx3)
    the_llist.insert_value_at_index(new_data3, idx3)
    check_idx = len(stuff) - 1
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx + 2)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_234():
    test_item = "insert_value_at_index()"
    reason = "insert new values first, last, and middle; should find old data shifted"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    new_data = "a new datum added at index {}".format(idx)
    the_llist.insert_value_at_index(new_data, idx)
    idx2 = the_llist.size()
    new_data2 = "a new datum added at index {}".format(idx2)
    the_llist.insert_value_at_index(new_data2, idx2)
    idx3 = the_llist.size() // 2
    new_data3 = "a new datum added at index {}".format(idx3)
    the_llist.insert_value_at_index(new_data3, idx3)
    result = the_llist.size()
    expected = len(stuff) + 3
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# check what happens when you start deleting values from the LList

def test_235():
    test_item = "delete_item_at_index()"
    reason = "removing value at index 0, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    result = the_llist.delete_item_at_index(idx)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_236():
    test_item = "delete_item_at_index(), get_item_at_index()"
    reason = "removing value at index 0, value should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_237():
    test_item = "delete_item_at_index(), value_is_in()"
    reason = "removing value at index 0, value should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    result = the_llist.value_is_in(stuff[idx])
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_238():
    test_item = "delete_item_at_index()"
    reason = "removing value at index 0, size should change"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    result = the_llist.size()
    expected = len(stuff) - 1
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_239():
    test_item = "delete_item_at_index()"
    reason = "removing value at index 0, old values move forward"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    result = the_llist.get_index_of_value(stuff[idx + 1])
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_240():
    test_item = "delete_item_at_index()"
    reason = "removing value at start and end, flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    result = the_llist.delete_item_at_index(idx2)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_241():
    test_item = "delete_item_at_index(), get_index_of_value()"
    reason = "removing value at start and end, old data at end should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    result = the_llist.get_index_of_value(stuff[idx2 + 1])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_242():
    test_item = "delete_item_at_index(), value_is_in()"
    reason = "removing value at start and end, old data at end should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    result = the_llist.value_is_in(stuff[idx2 + 1])
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_243():
    test_item = "delete_item_at_index(), value_is_in()"
    reason = "removing value at start and end, size should decrease"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    result = the_llist.size()
    expected = len(stuff) - 2
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_244():
    test_item = "delete_item_at_index(), get_index_of_value()"
    reason = "removing value at start and end, old data should still be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    check_idx = len(stuff) - 2
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx - 1)
    assert result == expected, assert_report.format(test_item, reason, result, expected)



def test_245():
    test_item = "delete_item_at_index()"
    reason = "removing value at start, end, middle; flag should be True"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    idx3 = the_llist.size() // 2
    result = the_llist.delete_item_at_index(idx3)
    expected = True
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_246():
    test_item = "delete_item_at_index(), get_index_of_value()"
    reason = "removing value at start, end, middle; removed middle value should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    idx3 = the_llist.size() // 2
    the_llist.delete_item_at_index(idx3)
    check_idx = len(stuff) // 2
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_247():
    test_item = "delete_item_at_index(), value_is_in()"
    reason = "removing value at start, end, middle; removed middle value should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    idx3 = the_llist.size() // 2
    the_llist.delete_item_at_index(idx3)
    result = the_llist.value_is_in("TURN-AROUND")
    expected = False
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_248():
    test_item = "delete_item_at_index(), size()"
    reason = "removing value at start, end, middle; size should decrease"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    idx3 = the_llist.size() // 2
    the_llist.delete_item_at_index(idx3)
    result = the_llist.size()
    expected = len(stuff) - 3
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_249():
    test_item = "delete_item_at_index(), size()"
    reason = "removing value at start, end, middle; other data should still be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    idx = 0
    the_llist.delete_item_at_index(idx)
    idx2 = len(stuff) - 2
    the_llist.delete_item_at_index(idx2)
    idx3 = the_llist.size() // 2
    the_llist.delete_item_at_index(idx3)
    check_idx = len(stuff) - 3
    result = the_llist.get_index_of_value(stuff[check_idx])
    expected = (True, check_idx - 2)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# check what happens when you add and remove a bunch from the back

def test_250():
    test_item = "add_to_back(), remove_from_back()"
    reason = "size should be known"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_back()
    result = the_llist.size()
    expected= len(stuff) - to_remove
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_251():
    test_item = "add_to_back(), remove_from_back(), get_index_of_value()"
    reason = "removed values should not be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_back()
    idx = the_llist.size()
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_252():
    test_item = "add_to_back(), remove_from_back(), get_index_of_value()"
    reason = "remaining values should be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_back()
    idx = the_llist.size() - 1
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, idx)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_253():
    test_item = "add_to_back(), remove_from_back(), retrieve_data_at_index()"
    reason = "remaining values should be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_back(word)
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_back()
    idx = the_llist.size() - 1
    result = the_llist.retrieve_data_at_index(idx)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


###############################################################################################
# check what happens when you add and remove a bunch from the front

def test_254():
    test_item = "add_to_front(), remove_from_front(), size()"
    reason = "size should be known"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_front()
    result = the_llist.size()
    expected = len(stuff) - to_remove
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_255():
    test_item = "add_to_front(), remove_from_front(), get_index_of_value()"
    reason = "removed data should be gone"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)  # like a stack
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_front()
    idx = len(stuff) - to_remove
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (False, None)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_256():
    test_item = "add_to_front(), remove_from_front(), get_index_of_value()"
    reason = "remaining data should be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)  # like a stack
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_front()
    idx = len(stuff) - to_remove - 1 # the first item in the LList calculated from the end of stuff
    result = the_llist.get_index_of_value(stuff[idx])
    expected = (True, 0)
    assert result == expected, assert_report.format(test_item, reason, result, expected)


def test_257():
    test_item = "add_to_front(), remove_from_front(), retrieve_data_at_index()"
    reason = "remaining data should be there"
    the_llist = L.LList()
    stuff = 'HEY STOP SIGN THANK YOU TURN AROUND DOING HORSE SHOE TURTLE'.split()
    for word in stuff:
        the_llist.add_to_front(word)
    to_remove = 4
    for i in range(to_remove):
        the_llist.remove_from_front()
    idx = len(stuff) - to_remove - 1 # the first item in the LList calculated from the end of stuff
    result = the_llist.retrieve_data_at_index(0)
    expected = (True, stuff[idx])
    assert result == expected, assert_report.format(test_item, reason, result, expected)


# a list of references to the functions defined above 
# you can comment out any tests you don't want to run
llist_tests = [
    # unit tests begin here
    test_001, test_002, test_003, test_004, test_005, test_006, test_007,
    test_008, test_009, test_010, test_011, test_012, test_013, test_014,
    test_015, test_016, test_017, test_018, test_019, test_020, test_021,
    test_022, test_023, test_024, test_025, test_026, test_027, test_028,
    test_029, test_030, test_031, test_032, test_033, test_034, test_035,
    test_036, test_037, test_038, test_039, test_040, test_041, test_042,
    test_043, test_044, test_045, test_046, test_047, test_048, test_049,
    test_050, test_051, test_052, test_053, test_054, test_055, test_056,
    test_057, test_058, test_059, test_060, test_061, test_062, test_063,
    test_064, test_065, test_066, test_067, test_068, test_069, test_070,
    test_071, test_072, test_073, test_074, test_075, test_076, test_077,
    test_078, test_079, test_080, test_081, test_082, test_083, test_084,
    test_085, test_086, test_087, test_088, test_089, test_090, test_091,
    test_092, test_093, test_094, test_095, test_096, test_097, test_098,
    test_099, test_100, test_101, test_102, test_103, test_104, test_105,
    test_106, test_107, test_108, test_109, test_110, test_111, test_112,
    test_113, test_114, test_115, test_116, test_117, test_118, test_119,
    test_120, test_121, test_122, test_123, test_124, test_125, test_126,
    test_127, test_128, test_129, test_130, test_131, test_132, test_133,
    test_134, test_135, test_136, test_137, test_138, test_139, test_140,
    test_141, test_142, test_143, test_144, test_145, test_146, test_147,
    test_148, test_149, test_150, test_151, test_152, test_153, test_154,
    test_155, test_156, test_157, test_158, test_159, test_160, test_161,
    test_162, test_163, test_164, test_165, test_166, test_167, test_168,
    test_169, test_170, test_171, test_172, test_173, test_174, test_175,
    test_176, 
    
    # integration tests begin here
    
    test_177, test_178, test_179, test_180, test_181, test_182,
    test_183, test_184, test_185, test_186, test_187, test_188, test_189,
    test_190, test_191, test_192, test_193, test_194, test_195, test_196,
    test_197, test_198, test_199, test_200, test_201, test_202, test_203,
    test_204, test_205, test_206, test_207, test_208, test_209, test_210,
    test_211, test_212, test_213, test_214, test_215, test_216, test_217,
    test_218, test_219, test_220, test_221, test_222, test_223, test_224,
    test_225, test_226, test_227, test_228, test_229, test_230, test_231,
    test_232, test_233, test_234, test_235, test_236, test_237, test_238,
    test_239, test_240, test_241, test_242, test_243, test_244, test_245,
    test_246, test_247, test_248, test_249, test_250, test_251, test_252,
    test_253, test_254, test_255, test_256, test_257,
    ]

# a list (n, s), where n is number of tests passed; s is score 
# e.g., if n = 157, then s = 15
# e.g., if n = 227, then s = 20

# The starter file gets 42 test correct without adding any code!
# Your code has to be pretty solid to get top marks!
table = [(42, 0), (77, 5), (102, 10), (152, 15),
        (202, 20), (242, 24), (252, 27), (257, 30)]

try:
    # if the file is named correctly, import will work
    import LList as L
    runemall("LList score", llist_tests, table)

except ImportError as e:
    # but if it is missing, report the problem, with zero score
    max_score = max(v for (t, v) in table)
    report_import_error("LList score", "LList", max_score)

