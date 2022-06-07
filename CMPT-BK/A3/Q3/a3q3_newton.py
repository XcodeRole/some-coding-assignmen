from a3q3 import newton

def test_newton():
    case1=0.25
    case2=0.75
    case3=1.7
    case4=153

    case5=0
    case6=1

    assert newton(case1)>case1
    assert newton(case2)>case2
    assert newton(case3)<case3
    assert newton(case4)<case4

    assert newton(case5)!=0
    assert newton(case6)==1

    print("newton test succeed!")

if __name__=='__main__':
    test_newton()