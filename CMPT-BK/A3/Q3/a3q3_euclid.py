from a3q3 import euclid

def test_euclid():
    case1=(27,6)
    case2=(6,27)

    case3=(27,27)
    case4=(0,0)

    assert euclid(*case1)==3
    assert euclid(*case2)==3
    assert euclid(*case3)==27
    assert euclid(*case4)==0

    print("test euclid succeed!")


if __name__=='__main__':
    test_euclid()