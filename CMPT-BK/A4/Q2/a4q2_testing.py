import a4q2 as S


def close_enough(a, b, tolerance):
    """
    Purpose:
        Check if 2 floating point values are close enough to 
        be considered equal.  See the Addendum in the assignment!

        This doesn't need to be a function, but it may help
        understanding the issues if the details are encapsulated
        ihere.

    Pre-Conditions:
        :param a: a floating point value
        :param b: a floating point value
        :param tolerance: a small positive floating point value
    Post-Conditions:
        none
    Return:
        :return True if the difference between a and b is small
    """
    return abs(a - b) < tolerance

#####################################################################
# test the constructor Statistics()
# We can't test the object's identity, and the attributes are private
# The only thing we can do is check to see if the initial values are correct
# This is indirect testing, and since we're using 2 methods in the test, 
# it's a very limited form of integration testing.


"""
Since two new functions are added, I use the expected and result lists to test
And in a2q4_testing.py(this file) I only save the new and modified parts as well as uitl funtion
"""


test_item = 'Statistics()'

############  test case ############
expected = [0,0,None,None]
reason = "Initial count value"

# call the operation
stats = S.Statistics()
result=[]
result.append(stats.count())
result.append(stats.mean())
result.append(stats.minimum())
result.append(stats.minimum())

if result != expected:
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))


#####################################################################
# test maximum()

############  test case ############
test_item = 'add() followed by maximum()'
data_in = [2.3, 1.5, 2.4, 3.8, 6.7]
expected = 6.7
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
for v in data_in:
    stats.add(v)
result = stats.maximum()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))

############  test case ############
test_item = 'add() followed by maximum()'
data_in = 1
expected = 1
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
stats.add(data_in)
result = stats.maximum()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))

############  test case ############
test_item = 'add() followed by maximum()'
data_in = [0,0,0,0,0]
expected = 0
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
for v in data_in:
    stats.add(v)
result = stats.maximum()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))

#####################################################################
# test minimum()

############  test case ############
test_item = 'add() followed by minimum()'
data_in = [2.3, 1.5, 2.4, 3.8, 6.7]
expected = 1.5
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
for v in data_in:
    stats.add(v)
result = stats.minimum()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))

############  test case ############
test_item = 'add() followed by minimum()'
data_in = 1
expected = 1
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
stats.add(data_in)
result = stats.minimum()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))

############  test case ############
test_item = 'add() followed by maximum()'
data_in = [1.2,1.2,1.2,1.2,1.2]
expected = 1.2
reason = "Check average after 5 zeroes added"

# call the operation
stats = S.Statistics()
for v in data_in:
    stats.add(v)
result = stats.maximum()

if not close_enough(expected, result, 0.0001):
    print('Error in {}: expected {} but obtained {} -- {}'.format(test_item, expected, result, reason))


print('*** Test script completed ***')
