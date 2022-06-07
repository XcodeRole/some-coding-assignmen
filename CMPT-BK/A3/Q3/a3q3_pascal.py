
from a3q3 import pascal

def test_pascal():
    case1="triangle1.txt"
    case2="triangle2.txt"
    case3="triangle3.txt"

    triangle1=[[1],[1 ,1],[1 ,2, 1],[1 ,3, 3, 1],[1 ,4, 6, 4 ,1],[1, 5, 10, 10 ,5 ,1],[1 ,6 ,15 ,20 ,15 ,6 ,1]]
    assert pascal(case1)==(7,triangle1)

    triangle2=[
        [1],
        [1,2],
        [1,2,3]
    ]
    assert pascal(case2)==(3,triangle2)

    triangle3=[
        [3],
        [2,3]
    ]

    assert pascal(case3)==(2,triangle3)


    #Test that the elements of the triangle are also lists.
    for alist in triangle1:
        assert type(alist) is list


    print("test pascal succeed!")

if __name__=='__main__':
    test_pascal()
    