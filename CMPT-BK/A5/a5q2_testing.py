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
#    Assignment 5 Question 2 test script

import a5q1 as a5q1
import a5q2 as a5q2
import node as N




####################################################################################################
#### UNIT TEST CASE: check_chains() ####
test_item = "check_chains()"

data_in1 = None
data_in2 = None
expected = 'same chain'
reason = 'Two empty node chains'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1)
data_in2 = None
expected = 'different starting at 0'
reason = 'One empty node chain'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

#### UNIT TEST CASE: check_chains() ####
data_in1 = None
data_in2 = N.node(1)
expected = 'different starting at 0'
reason = 'One empty node chain'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1)
data_in2 = N.node(1)
expected = 'same values'
reason = 'Simple node chains, same values'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1)
data_in2 = data_in1
expected = 'same chain'
reason = 'Simple node chain, two copies of the same reference'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1, N.node(2, N.node(3)))
data_in2 = data_in1
expected = 'same chain'
reason = 'longer node chain, two copies of the same reference'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1, N.node(2, N.node(3)))
data_in2 = N.node(1, N.node(2, N.node(3)))
expected = 'same values'
reason = 'longer node chain, two copies of the same chain'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1, N.node(2, N.node(3)))
data_in2 = N.node(1, N.node(2, N.node(1)))
expected = 'different starting at 2'
reason = 'longer node chain, similar, last value different'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1, N.node(2, N.node(3)))
data_in2 = N.node(1, N.node(2, N.node(3, N.node(4))))
expected = 'different starting at 3'
reason = 'longer node chain, similar, second chain longer'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))

#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1, N.node(2, N.node(3, N.node(4))))
data_in2 = N.node(1, N.node(2, N.node(3)))
expected = 'different starting at 3'
reason = 'longer node chain, similar, first chain longer'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: check_chains() ####
data_in1 = N.node(1, N.node(2, N.node(3, N.node(4))))
data_in2 = N.node(2, N.node(3, N.node(4)))
expected = 'different starting at 0'
reason = 'longer node chain, very different'

result = a5q2.check_chains(data_in1, data_in2)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))



####################################################################################################
print('*** testing complete ***')



