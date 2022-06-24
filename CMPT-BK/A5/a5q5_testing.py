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
#    Assignment 5 Question 5 test script

import a5q1 as a5q1
import a5q5 as a5q5
import node as N

####################################################################################################

#### UNIT TEST CASE: split_chain() ####
achain = None
expected1 = 'EMPTY'
expected2 = 'EMPTY'
reason = 'Empty node chain'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)



#### UNIT TEST CASE: split_chain() ####
achain = N.node(1)
expected1 = 'EMPTY'
expected2 = '[ 1 | / ]'
reason = 'node chain with one node'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)



#### UNIT TEST CASE: split_chain() ####
achain = N.node(1, N.node('two'))
expected1 = '[ 1 | / ]'
expected2 = '[ two | / ]'
reason = 'node chain with two nodes'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)



#### UNIT TEST CASE: split_chain() ####
achain = N.node(1, N.node('two', N.node(3)))
expected1 = '[ 1 | / ]'
expected2 = '[ two | *-]-->[ 3 | / ]'
reason = 'node chain with three nodes'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)



#### UNIT TEST CASE: split_chain() ####
achain = N.node(1, N.node('two', N.node(3,N.node('four'))))
expected1 = '[ 1 | *-]-->[ two | / ]'
expected2 = '[ 3 | *-]-->[ four | / ]'
reason = 'node chain with four nodes'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)



#### UNIT TEST CASE: split_chain() ####
achain = N.node(1, N.node('two', N.node(3,N.node('four', N.node(5)))))
expected1 = '[ 1 | *-]-->[ two | / ]'
expected2 = '[ 3 | *-]-->[ four | *-]-->[ 5 | / ]'
reason = 'node chain with five nodes'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)



#### UNIT TEST CASE: split_chain() ####
achain = N.node(1, N.node('two', N.node(3,N.node('four', N.node(5, N.node('six'))))))
expected1 = '[ 1 | *-]-->[ two | *-]-->[ 3 | / ]'
expected2 = '[ four | *-]-->[ 5 | *-]-->[ six | / ]'
reason = 'node chain with six nodes'

result1, result2 = a5q5.split_chain(achain)
str_result1 = a5q1.to_string(result1)
str_result2 = a5q1.to_string(result2)
if str_result1 != expected1:
    print('Test failed: split_chain(): got first', str_result1, 'expected', str(expected1), '--'+reason)
if str_result2 != expected2:
    print('Test failed: split_chain(): got second', str_result2, 'expected', str(expected2), '--'+reason)


####################################################################################################

#### UNIT TEST CASE: merge() ####
achain = None
anotherchain = None
expected = 'EMPTY'
reason = 'two empty node chains'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(2)
anotherchain = None
expected = '[ 2 | / ]'
reason = 'one empty node chain (second parameter) and input node chain with only one node (first parameter)'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = None
anotherchain = N.node(2)
expected = '[ 2 | / ]'
reason = 'one empty node chain (first parameter) and input node chain with only one node (second parameter)'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(2, N.node(5, N.node(9)))
anotherchain = None
expected = '[ 2 | *-]-->[ 5 | *-]-->[ 9 | / ]'
reason = 'one empty node chain (second parameter) and input node chain with multiple nodes (first parameter)'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = None
anotherchain = N.node(2, N.node(5, N.node(9)))
expected = '[ 2 | *-]-->[ 5 | *-]-->[ 9 | / ]'
reason = 'one empty node chain (second parameter) and input node chain with multiple nodes (second parameter)'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(2)
anotherchain = N.node(3)
expected = '[ 2 | *-]-->[ 3 | / ]'
reason = 'two node chains with one node each, the value in the second node chain is larger then the value in the first node chain'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(10)
anotherchain = N.node(3)
expected = '[ 3 | *-]-->[ 10 | / ]'
reason = 'two node chains with one node each, the value in the first node chain is larger then the value in the second node chain'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(1, N.node(1, N.node(9)))
anotherchain = N.node(2, N.node(7, N.node(15)))
expected = '[ 1 | *-]-->[ 1 | *-]-->[ 2 | *-]-->[ 7 | *-]-->[ 9 | *-]-->[ 15 | / ]'
reason = 'two node chains with multiple nodes, all positive values'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(-10, N.node(-10, N.node(-9)))
anotherchain = N.node(-8, N.node(-7, N.node(-2)))
expected = '[ -10 | *-]-->[ -10 | *-]-->[ -9 | *-]-->[ -8 | *-]-->[ -7 | *-]-->[ -2 | / ]'
reason = 'two node chains with multiple nodes, all negative values; will take all values from nc1 first then append nc2'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(-10, N.node(-10, N.node(-9)))
anotherchain = N.node(-18, N.node(-17, N.node(-12)))
expected = '[ -18 | *-]-->[ -17 | *-]-->[ -12 | *-]-->[ -10 | *-]-->[ -10 | *-]-->[ -9 | / ]'
reason = 'two node chains with multiple nodes, all negative values; will take all values from nc2 first then append nc1'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(10, N.node(20, N.node(30)))
anotherchain = N.node(10, N.node(20, N.node(30)))
expected = '[ 10 | *-]-->[ 10 | *-]-->[ 20 | *-]-->[ 20 | *-]-->[ 30 | *-]-->[ 30 | / ]'
reason = 'two identical chains, multiple nodes'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge() ####
achain = N.node(-10, N.node(20, N.node(30)))
anotherchain = N.node(-18, N.node(-10, N.node(0)))
expected = '[ -18 | *-]-->[ -10 | *-]-->[ -10 | *-]-->[ 0 | *-]-->[ 20 | *-]-->[ 30 | / ]'
reason = 'node chains with mixed positive and negative integers'

result = a5q5.merge(achain, anotherchain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge(): got', str_result, 'expected', str(expected), '--'+reason)


####################################################################################################

#### UNIT TEST CASE: merge_sort() ####
achain = None
expected = 'EMPTY'
reason = 'empty node chain'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(10)
expected = '[ 10 | / ]'
reason = 'node chain with one node (positive)'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(-10)
expected = '[ -10 | / ]'
reason = 'node chain with one node (negative)'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(10, N.node(9, N.node(12, N.node(7, N.node(11, N.node(8))))))
expected = '[ 7 | *-]-->[ 8 | *-]-->[ 9 | *-]-->[ 10 | *-]-->[ 11 | *-]-->[ 12 | / ]'
reason = 'node chain multiple values, all positive'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(-10, N.node(-9, N.node(-12, N.node(-7, N.node(-11, N.node(-8))))))
expected = '[ -12 | *-]-->[ -11 | *-]-->[ -10 | *-]-->[ -9 | *-]-->[ -8 | *-]-->[ -7 | / ]'
reason = 'node chain multiple values, all negative'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(1, N.node(2, N.node(3, N.node(4, N.node(5, N.node(6))))))
expected = '[ 1 | *-]-->[ 2 | *-]-->[ 3 | *-]-->[ 4 | *-]-->[ 5 | *-]-->[ 6 | / ]'
reason = 'node chain multiple values, already in ascending order'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(6, N.node(5, N.node(4, N.node(3, N.node(2, N.node(1))))))
expected = '[ 1 | *-]-->[ 2 | *-]-->[ 3 | *-]-->[ 4 | *-]-->[ 5 | *-]-->[ 6 | / ]'
reason = 'node chain multiple values, already in descending order'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)



#### UNIT TEST CASE: merge_sort() ####
achain = N.node(-6, N.node(50, N.node(-17, N.node(-3, N.node(2, N.node(1))))))
expected = '[ -17 | *-]-->[ -6 | *-]-->[ -3 | *-]-->[ 1 | *-]-->[ 2 | *-]-->[ 50 | / ]'
reason = 'node chain multiple values, mixed positive and negative'

result = a5q5.merge_sort(achain)
str_result = a5q1.to_string(result)
if str_result != expected:
    print('Test failed: merge_sort(): got', str_result, 'expected', str(expected), '--'+reason)


####################################################################################################
print('*** testing complete ***')


