import sys as sys

if len(sys.argv)!=2:
    print("Please provide an integer parameter")
    exit()
example = int(sys.argv [1])
def factorial(x):
    """
    Calculate the product of numbers 1 to x.
    """
    total = 1
    for i in range(1,x+1):
        total *= i
    return total

print(factorial(example))