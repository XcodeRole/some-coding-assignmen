from treenode import *
from treebuilding import *
from a8q2 import *

case1=None
case2=build_lecture_example()

assert collect_inorder(case1)==[]
assert collect_preorder(case1)==[]
assert collect_postorder(case1)==[]


assert collect_inorder(case2)==[11,7,6,2,5]
assert collect_preorder(case2)==[2,7,11,6,5]
assert collect_postorder(case2)==[11,6,7,5,2]

print("pass")
