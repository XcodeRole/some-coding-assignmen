def Fibonacci(n):
    assert(n>=0)
    if n==0:
        return 0
    elif n==1:
        return 1
    elif n>1:
        return Fibonacci(n-1)+Fibonacci(n-2)
def Moosonacci(n):
    assert(n>=0)
    if n==0:
        return 0
    elif n==1:
        return 1
    elif n==2:
        return 2
    elif n>2:
        return Moosonacci(n-1)+Moosonacci(n-2)+Moosonacci(n-3)

def recsum(i, j):
    if i>=j :
        return 0
    else:
        return i+recsum(i+1, j)

def countoddrec(i, j):
    if i>=j:
        return 0
    elif i%2 == 1:
        return 1+countoddrec(i+1, j)
    else:
        return countoddrec(i+1, j)
        