from a7q7 import *
from node import node

case1=None
case2=node(1)
case3=node(1,node(2))
assert sumnc_rec(case1)==0
assert sumnc_rec(case2)==1
assert sumnc_rec(case3)==3

assert membernc_rec(1,case1)==False
assert membernc_rec(1,case2)==True
assert membernc_rec(1,case3)==True

assert countnc_rec(1,case1)==0
assert countnc_rec(1,case2)==1
assert countnc_rec(1,case3)==1

