from a7q5 import *

assert recsumlist ([]) == 0, 'Empty list'
assert recsumlist ([1, 3, 4]) == 8, 'Non -empty list'

assert recmemberlist (0, []) == False , 'Empty list'
assert recmemberlist (3, [1, 3, 4]) == True , 'Non -empty list'
assert recmemberlist (5, [1, 3, 4]) == False , 'Non -empty list'

assert reccountlist (0, []) == 0, 'Empty list'
assert reccountlist (3, [1, 3, 4, 3]) == 2, 'Non -empty list'
assert reccountlist (2, [1, 3, 4, 3]) == 0, 'Non -empty list'


assert subst_str('l', 'x', 'Hello , world!') == 'Hexxo , worxd!'
assert subst_str('o', 'i', 'Hello , world!') == 'Helli , wirld!'
assert subst_str('z', 'q', 'Hello , world!') == 'Hello , world!'