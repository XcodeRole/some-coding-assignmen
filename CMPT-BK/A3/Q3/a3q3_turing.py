from a3q3 import turing

def test_turing():
    case1=[1,2,3]
    case2=[1,2,3,1]
    case3=[1,1,3]
    case4=[1,5,6,7,1,5]
    case5=[]

    assert turing(case1)==[1,2,3]
    assert turing(case2)==[1,2,3]
    assert turing(case3)==[1,3]
    assert turing(case4)==[1,5,6,7]
    assert turing(case5)==[]

    print("test turing succeed!")

if __name__=='__main__':
    test_turing()