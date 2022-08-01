from a8q4 import *
from treebuilding import *
import treenode as TN
import treebuilding as TB

tricky_tree = TN.treenode(0, TB.build_fibtree (5), TB.build_complete (10))

complete_case=build_complete(20)
uncomplete_case=tricky_tree

assert (True,20)==complete(complete_case)
assert (False,None)==complete(uncomplete_case)

print("pass")