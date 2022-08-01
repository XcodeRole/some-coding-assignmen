from a8q3 import *
from treenode import *
from treebuilding import *
from treetesting import *
####################################
#check_trees
case1=None
cmp_case1=None

case2=build_lecture_example()
cmp_case2=build_lecture_example()

case3=build_lecture_example()
cmp_case3=build_turtle()

assert check_trees(case1,cmp_case1)==True
assert check_trees(case1,case2)==False

assert check_trees(case2,cmp_case2)==True
assert check_trees(case3,cmp_case3)==False

############################
#replace
case1=build_lecture_example()
replace(case1,11,3)
cmp_case1=treenode(2,treenode(7,treenode(3),treenode(6)),treenode(5))

#use to string function given
assert to_string_for_testing(case1)==to_string_for_testing(cmp_case1)

#use check_trees
assert check_trees(case1,cmp_case1)

#######################################
#copy
case1=build_lecture_example()
cmp_case1=copy(case1)

case2=None
cmp_case2=copy(case2)

assert check_trees(case1,cmp_case1)
assert check_trees(case2,cmp_case2)


print("pass")
