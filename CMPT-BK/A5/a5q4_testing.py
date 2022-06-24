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
#   Assignment 5 Question 4 test script

import a5q1 as a5q1
import a5q2 as a5q2
import a5q4 as a5q4
import node as N



####################################################################################################
#### UNIT TEST CASE: copync() ####
test_item = "copync()"

data_in = None
expected = 'same chain'
reason = 'Empty node chain'

result = a5q2.check_chains(data_in, a5q4.copync(data_in))
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


data_in = N.node(1)
expected = 'same values'
reason = 'Singleton node chain'

result = a5q2.check_chains(data_in, a5q4.copync(data_in))
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

data_in = N.node(1, N.node(2, N.node(3)))
expected = 'same values'
reason = 'Longer node chain'

temp = a5q4.copync(data_in)
result = a5q2.check_chains(data_in, temp)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


####################################################################################################
#### UNIT TEST CASE: copync() ####
test_item = "double_up()"

data_in = None
expected = None
reason = 'Empty node chain'

a5q4.double_up(data_in)
result_str = a5q1.to_string(data_in)
expected_str = a5q1.to_string(expected)
if expected_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))


data_in = N.node('a')
expected = N.node('a', N.node('a'))
reason = 'singleton node chain'

a5q4.double_up(data_in)
result_str = a5q1.to_string(data_in)
expected_str = a5q1.to_string(expected)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))


data_in = N.node('a', N.node('b', N.node('see')))
expected = N.node('a', N.node('a', N.node('b', N.node('b', N.node('see', N.node('see'))))))
reason = 'longer node chain'

a5q4.double_up(data_in)
result_str = a5q1.to_string(data_in)
expected_str = a5q1.to_string(expected)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))




####################################################################################################
print('*** testing complete ***')



