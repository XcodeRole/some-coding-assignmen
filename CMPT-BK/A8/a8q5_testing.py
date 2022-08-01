from a8q5 import *
from treebuilding import *

case1=build_lecture_example()
assert (True,[2,7])==path_to(case1,6)
assert (False,None)==path_to(case1,3)

print("pass")
