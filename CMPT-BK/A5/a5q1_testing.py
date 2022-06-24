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
#    Assignment 5 Question 1 test script

import a5q1 as a5q1
import node as N

#### UNIT TEST CASES
test_item = 'to_string()'
data_in = None
expected = 'EMPTY'
reason = 'Empty node chain'

result = a5q1.to_string(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


data_in = N.node(1)
expected = '[ 1 | / ]'
reason = 'node chain with one node'

result = a5q1.to_string(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


data_in = N.node(1, N.node('two'))
expected = '[ 1 | *-]-->[ two | / ]'
reason = 'node chain with two nodes'

result = a5q1.to_string(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


data_in = N.node(1, N.node('two', N.node(3)))
expected = '[ 1 | *-]-->[ two | *-]-->[ 3 | / ]'
reason = 'node chain with three nodes'

result = a5q1.to_string(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

print('*** testing complete ***')
