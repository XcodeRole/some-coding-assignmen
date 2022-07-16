from a7q8 import *
from node import node

case1=None
case2=node(1)
case3=node(1,node(2))

case4=node(1,node(2))

assert to_string(case1)==''
assert to_string(case2)=='[ 1 | / ]'
assert to_string(case3)=='[ 1 | * -]-->[ 2 | / ]'

assert check_chains(case1,case2)==False
assert check_chains(case3,case4)==True

case5=copy(case3)
assert check_chains(case3,case5)==True

case6=node(1,node(3))
assert check_chains(case6,replace(case3,2,3))==True

print("tests pass")

