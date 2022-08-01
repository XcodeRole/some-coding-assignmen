from a8q1 import *
from treenode import *
from treebuilding import *
"""
the case2 binary tree is like follow:
        2
      /   \
    7       5
  /   \
11      6
"""
case1=None
case2=build_lecture_example()

#############################################
#nodes_at_level
assert nodes_at_level(case1,1)==None
assert nodes_at_level(case2,1)==1
assert nodes_at_level(case2,3)==2
assert nodes_at_level(case2,4)==0

#############################################
#largest_leaf_value
assert largest_leaf_value(case1)==None
assert largest_leaf_value(case2)==11

#############################################
#closest
assert closest(case1,1)==None
assert closest(case2,1)==2

print("pass")


