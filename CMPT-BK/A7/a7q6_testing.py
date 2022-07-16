from a7q6 import *
assert recsumlisti (0, []) == 0, 'Empty list'
assert recsumlisti (0, [1, 3, 4]) == 8, 'start at index 0'
assert recsumlisti (1, [1, 3, 4]) == 7, 'start at index 1'
assert recsumlisti (2, [1, 3, 4]) == 4, 'start at index 2'
assert recsumlisti (3, [1, 3, 4]) == 0, 'start at index 3'

assert recmemberlisti (0, 0, []) == False , 'Empty list'
assert recmemberlisti (0, 3, [1, 3, 4]) == True , 'start at index 0'
assert recmemberlisti (1, 3, [1, 3, 4]) == True , 'start at index 1'
assert recmemberlisti (2, 3, [1, 3, 4]) == False , 'start at index 2'

assert reccountlisti (0, 0, []) == 0, 'Empty list'
assert reccountlisti (0, 3, [1, 3, 4, 3]) == 2, 'start at index 0'
assert reccountlisti (1, 3, [1, 3, 4, 3]) == 2, 'start at index 1'
assert reccountlisti (2, 3, [1, 3, 4, 3]) == 1, 'start at index 2'

print('tests pass')