from copy import deepcopy

def get_square():
    matStr=input("Please enter 9 numbers:")
    mat=[]
    row=[]
    str2list=matStr.split()
    for num in str2list:
        row.append(int(num))
        if len(row)==3:
            #use deepcopy
            mat.append(deepcopy(row))
            #The row is emptied on each iteration
            row.clear()
    return mat


def check_range(square):
    seen = [False]*9
    for row in square:
        for cal in row:
            seen[cal-1]=True
    ret=True
    for elem in seen:
        ret=ret and elem
    return ret

def check_rows(square):
    for row in square:
        sum=0
        for col in row:
            sum+=col
        if sum != 15:
            return False
    return True

def check_columns(square):
    # Put the sum of each column into a list
    col_sum=[0]*3
    for row in square:
        for i in range(len(row)):
            col_sum[i]+=row[i]
    for elem in col_sum:
        if elem != 15:
            return False
    return True


def check_diagonals(square):
    dim_r=len(square)
    dim_c=len(square[0])
    sum1=0
    sum2=0
    for i in range(dim_r):
        for j in range(dim_c):
            #The indices of the diagonal elements satisfy the following equation
            if i==j :
                sum1+=square[i][j]
            if i+j==dim_c-1:
                sum2+=square[i][j]
    if sum1==sum2==15:
        return True
    return False


def check_square(square):
    return check_range(square) and check_rows(square) and check_columns(square) and check_diagonals(square)


if __name__=='__main__':
    square=get_square()
    print(square)
    if check_square(square):
        print("YES")
    else:
        print("NO")