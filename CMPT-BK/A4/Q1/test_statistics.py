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
#   Assignment 4: ADTs and  Objects

# This script is a starter file for testing the Statistics ADT

import Statistics as S


def close_enough(a, b, tolerance):
    """
    Purpose:
        Check if 2 floating point values are close enough to 
        be considered equal.  See the Addendum in the assignment!

        This doesn't need to be a function, but it may help
        understanding the issues if the details are encapsulated
        ihere.

    Pre-Conditions:
        :param a: a floating point value
        :param b: a floating point value
        :param tolerance: a small positive floating point value
    Post-Conditions:
        none
    Return:
        :return True if the difference between a and b is small
    """
    return abs(a - b) < tolerance


#####################################################################
# test the constructor Statistics()
# We can't test the object's identity, and the attributes are private
# The only thing we can do is check to see if the initial values are correct
# This is indirect testing, and since we're using 2 methods in the test, 
# it's a very limited form of integration testing.


test_item = 'Statistics()'

############  test case ############
expected = 0
reason = "Initial count value"

# call the operation
stats = S.Statistics()
result = stats.count()

if result != expected:
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))


############  test case ############
expected = 0
reason = "Initial average value"

# call the operation
stats = S.Statistics()
result = stats.mean()

if result != expected:
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))




#####################################################################
# test add()  
# We can't test add() directly, because it returns None.
# We can't look into the object at the private attributes either.
# So check add() + count() to see if we get sensible numbers.
# these are integration tests

############  test case ############
test_item = 'add() followed by count()'
data_in = 0
expected = 1
reason = "Check count after one zero added"

# call the operation
stats = S.Statistics()
stats.add(data_in)
result = stats.count()

if result != expected:
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))


############  test case ############
test_item = 'add() followed by count()'
data_in = [0, 0, 0, 0, 0]
expected = 5
reason = "Check count after 5 values added"

# call the operation
stats = S.Statistics()
for v in data_in:
    stats.add(v)
result = stats.count()

if result != expected:
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))






#####################################################################
# test mean()


############  test case ############
test_item = 'add() followed by mean()'
data_in = 0
expected = 0
reason = "Check average after one zero added"

# call the operation
stats = S.Statistics()
stats.add(data_in)
result = stats.mean()

# We should almost never test floating point values for equality.
# So use the close_enough() function defined above.

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))



############  test case ############
test_item = 'add() followed by mean()'
data_in = [0, 0, 0, 0, 0]
expected = 0.0
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
for v in data_in:
    stats.add(v)
result = stats.mean()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))





print('*** Test script completed ***')
