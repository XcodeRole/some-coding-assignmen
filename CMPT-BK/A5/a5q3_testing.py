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
#   Assignment 5 Question 3 test script

import a5q1 as a5q1
import a5q3 as a5q3
import node as N


####################################################################################################
#### UNIT TEST CASE: sumnc() ####
test_item = "sumnc()"

data_in = None
expected = 0
reason = 'Empty node chain'

result = a5q3.sumnc(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))



#### UNIT TEST CASE: sumnc() ####
data_in = N.node(1)
expected = 1
reason = 'node chain with one node'

result = a5q3.sumnc(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_chain() ####
data_in = N.node(1, N.node(-2))
expected = -1
reason = 'node chain with two nodes'

result = a5q3.sumnc(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_chain() ####
data_in = N.node(1, N.node(-2, N.node(3)))
expected = 2
reason = 'node chain with three nodes'

result = a5q3.sumnc(data_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


####################################################################################################
#### UNIT TEST CASE: count_in() ####
test_item = "count_in()"

chain_in = None
value_in = None
expected = 0
reason = 'Empty node chain'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_in() ####
chain_in = N.node(1)
value_in = 0
expected = 0
reason = 'node chain with one node, value not present'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_in() ####
chain_in = N.node(7)
value_in = 7
expected = 1
reason = 'node chain with one node, value present'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))



#### UNIT TEST CASE: count_in() ####
chain_in = N.node('one', N.node('two'))
value_in = 7
expected = 0
reason = 'node chain with two nodes, value not present'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_in() ####
chain_in = N.node('one', N.node('two'))
value_in = 'two'
expected = 1
reason = 'node chain with two nodes, value present last'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_in() ####
chain_in = N.node('one', N.node('two'))
value_in = 'one'
expected = 1
reason = 'node chain with two nodes, value present first'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))



#### UNIT TEST CASE: count_in() ####
chain_in = N.node(True, N.node(True))
value_in = True
expected = 2
reason = 'node chain with two nodes, value present twice'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_in() ####
chain_in = N.node(1, N.node('two', N.node(1, N.node('three', N.node(1)))))
value_in = 1
expected = 3
reason = 'node chain with three nodes, value present multiple times'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


#### UNIT TEST CASE: count_in() ####
chain_in = N.node(1, N.node('two', N.node(3, N.node('four'))))
value_in = 555
expected = 0
reason = 'node chain with four nodes, value not present'

result = a5q3.count_in(chain_in, value_in)
if result != expected:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result, expected, reason))


# ####################################################################################################
# #### UNIT TEST CASE: replace_in() ####
test_item = "replace_in()"

chain_in = None
target_in = 1
repl_in = 0
expected_str = "EMPTY"
reason = 'empty node chain'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))


#### UNIT TEST CASE: replace_in() ####
chain_in = N.node(1)
target_in = 1
repl_in = 0
expected_str = "[ 0 | / ]"
reason = 'node chain with one node, target replaced'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))

#### UNIT TEST CASE: replace_in() ####
chain_in = N.node(1)
target_in = 0
repl_in = 1
expected_str = "[ 1 | / ]"
reason = 'node chain with one node, target not present'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))


#### UNIT TEST CASE: replace_in() ####
chain_in = N.node(1, N.node(2))
target_in = 0
repl_in = 1
expected_str = "[ 1 | *-]-->[ 2 | / ]"
reason = 'node chain with two nodes, target not present'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))


#### UNIT TEST CASE: replace_in() ####
chain_in = N.node(1, N.node(2))
target_in = 1
repl_in = 10
expected_str = "[ 10 | *-]-->[ 2 | / ]"
reason = 'node chain with two nodes, target present first'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))


#### UNIT TEST CASE: replace_in() ####
chain_in = N.node(1, N.node(2))
target_in = 2
repl_in = 10
expected_str = "[ 1 | *-]-->[ 10 | / ]"
reason = 'node chain with two nodes, target present last'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))

#### UNIT TEST CASE: replace_in() ####
chain_in = N.node(1, N.node(2, N.node(3, N.node(1, N.node(4)))))
target_in = 1
repl_in = 10
expected_str = "[ 10 | *-]-->[ 2 | *-]-->[ 3 | *-]-->[ 10 | *-]-->[ 4 | / ]"
reason = 'node chain with multiple nodes, target present multiple times'

a5q3.replace_in(chain_in, target_in, repl_in)
result_str = a5q1.to_string(chain_in)
if result_str != expected_str:
    print('Test failed: {}: got "{}" expected "{}" -- {}'.format(test_item, result_str, expected_str, reason))



####################################################################################################
print('*** testing complete ***')
