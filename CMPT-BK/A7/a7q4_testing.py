from a7q4 import * 
assert recsum(1, 0) == 0, 'Empty series'
assert recsum(0, 1) == 0, 'Series length 1'
assert recsum(0, 2) == 1, 'Series length 2'
assert recsum(0, 5) == sum(range(0, 5)), 'Series length 5'
assert recsum(5, 10) == sum(range(5, 10)), 'Series length 5, starting at 5'


assert countoddrec (1, 0) == 0, 'Empty sequence '
assert countoddrec (0, 1) == 0, 'Sequence length 1'
assert countoddrec (0, 2) == 1, 'Sequence length 2'
assert countoddrec (0, 5) == 2, 'Sequence length 4'
assert countoddrec (5, 10) == 3, 'Sequence length 5, starting at 5'

print("test pass")