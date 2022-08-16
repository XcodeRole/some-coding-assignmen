# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.


import test145 as Test

import a10q1 as BSTP

def str_form(obj, result, expected, reason):
    """
    Purpose:
        Produce a string for information about a failed test case.
        Abbreviates test case assertions somewhat.
    Preconditions:
        :param obj:  a string describing the objective of the test (a function or method name)
        :param result: a value: the actual result of using the objective 
        :param expected: a value: the intended or expected result of using the objective
        :param reason: a string describing the purpose of the test
    Return:
        :return: a string
    """
    return 'Test fault for {}.  Returned <{}>, expected <{}>.  Reason: "{}"'.format(obj, result, expected, reason)


#########  empty tree tests ##################3
def test_1():
    test_objective = 'member_prim()'
    reason = 'member on empty BSTPtree, check return'
    atnode = None
    data = 'one'
    result = BSTP.member_prim(atnode, data)
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_2():
    test_objective = 'delete_prim()'
    reason = 'delete on empty BSTPtree, check return flag'
    atnode = None
    data = 'one'
    result, t = BSTP.delete_prim(atnode, data)
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_3():
    test_objective = 'delete_prim()'
    reason = 'delete on empty BSTPtree, check return tree'
    atnode = None
    data = 'one'
    f, result = BSTP.delete_prim(atnode, data)
    expected = None
    assert result == expected, str_form(test_objective, result, expected, reason)


#################  empty tree insertion tests #####################    
def test_4():
    test_objective = 'insert_prim()'
    reason = 'insert on empty BSTPtree, check flag'
    atnode = None
    data = 'one'
    result, newtree = BSTP.insert_prim(atnode, data)
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_5():
    test_objective = 'insert_prim()'
    reason = 'insert on empty BSTPtree, check tree reference'
    atnode = None
    data = 'one'
    flag, result = BSTP.insert_prim(atnode, data)
    assert result is not None, str_form(test_objective, None, 'not None', reason)


def test_6():
    test_objective = 'insert_prim()'
    reason = 'insert on empty BSTPtree, check tree data'
    atnode = None
    data = 'one'
    flag, atree = BSTP.insert_prim(atnode, data)
    result = atree.data
    expected = data
    assert result == expected, str_form(test_objective, result, expected, reason)


########  singleton tree membership tests  ############
def test_7():
    test_objective = 'member_prim()'
    reason = 'member on singleton BSTPtree, value at root, check return'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    result = BSTP.member_prim(atnode, data)
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


########  singleton tree membership tests  ############
def test_8():
    test_objective = 'member_prim()'
    reason = 'member on singleton BSTPtree, value at root, check left subtree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    b = BSTP.member_prim(atnode, data)
    result = atnode.left
    expected = None
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_9():
    test_objective = 'member_prim()'
    reason = 'member on singleton BSTPtree, value at root, check right subtree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    b = BSTP.member_prim(atnode, data)
    result = atnode.right
    expected = None
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_10():
    test_objective = 'member_prim()'
    reason = 'member on singleton BSTPtree, value not in tree, check return'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    result = BSTP.member_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_11():
    test_objective = 'member_prim()'
    reason = 'member on singleton BSTPtree, value not in tree, check left subtree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    b = BSTP.member_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    result = atnode.left
    expected = None
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_12():
    test_objective = 'member_prim()'
    reason = 'member on singleton BSTPtree, value not in tree, check right subtree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    b = BSTP.member_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    result = atnode.right
    expected = None
    assert result == expected, str_form(test_objective, result, expected, reason)


#### singleton tree deletion tests ############
def test_13():
    test_objective = 'delete_prim()'
    reason = 'delete on singleton BSTPtree, value at root, check flag'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    result, retval = BSTP.delete_prim(atnode, data)
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_14():
    test_objective = 'delete_prim()'
    reason = 'delete on singleton BSTPtree, value at root, check resulting tree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    flag, result = BSTP.delete_prim(atnode, data)
    expected = None
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_15():
    test_objective = 'delete_prim()'
    reason = 'delete on singleton BSTPtree, value not in tree, check flag'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    result, retval = BSTP.delete_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_16():
    test_objective = 'delete_prim()'
    reason = 'delete on singleton BSTPtree, value not in tree, check resulting tree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    flag, result = BSTP.delete_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    expected = atnode
    assert result == expected, str_form(test_objective, 'some tree', 'not the expected tree', reason)


def test_17():
    test_objective = 'delete_prim()'
    reason = 'delete on singleton BSTPtree, value not in tree, check left subtree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    flag, newtree = BSTP.delete_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    result = newtree.left
    expected = None
    assert result == expected, str_form(test_objective, 'something', None, reason)


def test_18():
    test_objective = 'delete_prim()'
    reason = 'delete on singleton BSTPtree, value not in tree, check right subtree'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    flag, newtree = BSTP.delete_prim(atnode, 'kjsdhkjsdhgsdfkjghdfkgjhsdkjhfsdg')
    result = newtree.right
    expected = None
    assert result == expected, str_form(test_objective, 'something', None, reason)


################  singleton tree (one leaf) insertion tests  #################
def test_19():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat value, check flag'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    result, newtree = BSTP.insert_prim(atnode, data)
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_20():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat value, check reference'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    flag, newtree = BSTP.insert_prim(atnode, data)
    
    assert atnode is newtree, str_form(test_objective, 'a different tree', 'original tree', reason)


def test_21():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat value, check data'
    atnode = None
    data = 'one'
    flag, atnode = BSTP.insert_prim(atnode, data)

    flag, newtree = BSTP.insert_prim(atnode, data)
    result = newtree.data
    expected = data
    
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_22():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert left, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    data = 3
    result, newtree = BSTP.insert_prim(atnode, data)

    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_23():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert left, check tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    data = 3
    flag, result = BSTP.insert_prim(atnode, data)

    expected = atnode

    assert result == expected, str_form(test_objective, 'different reference', 'reference to original tree', reason)


def test_24():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert left, check root data'

    atnode = None
    rdata = 5
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    data = 3
    flag, result = BSTP.insert_prim(atnode, data)

    result = atnode.data
    expected = rdata
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_25():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert left, check left tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    data = 3
    flag, atnode = BSTP.insert_prim(atnode, data)

    result = atnode.left

    assert result is not None, str_form(test_objective, None, 'something non None', reason)


def test_26():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert left, check left data'
    atnode = None

    data = '5'
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = '3'
    flag, atnode = BSTP.insert_prim(atnode, ldata)

    result = atnode.left.data
    expected = ldata
    
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_27():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert left, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = 3
    flag, atnode = BSTP.insert_prim(atnode, ldata)

    result = atnode.right

    assert result is None, str_form(test_objective, 'something non None', None, reason)


def test_28():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat left value, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = 3
    flag, atnode = BSTP.insert_prim(atnode, ldata)

    result, newtree = BSTP.insert_prim(atnode, ldata)
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_29():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat left value, check root reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = 3
    flag, atnode = BSTP.insert_prim(atnode, ldata)
    
    expected = atnode
    
    flag, result = BSTP.insert_prim(atnode, ldata)
    assert result is expected, str_form(test_objective, None, 'not None', reason)


def test_30():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat left value, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = 3
    flag, atnode = BSTP.insert_prim(atnode, ldata)

    result, newtree = BSTP.insert_prim(atnode, ldata)
    expected = False
    assert result is expected, str_form(test_objective, result, expected, reason)


def test_31():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat left value, check left reference'
    atnode = None

    data = '5'
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = '3'
    flag, atnode = BSTP.insert_prim(atnode, ldata)

    expected = atnode.left
    
    flag, newtree = BSTP.insert_prim(atnode, ldata)
    result = newtree.left

    assert result is expected, str_form(test_objective, result, expected, reason)


def test_32():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat left value, check left data'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    ldata = 3
    flag, atnode = BSTP.insert_prim(atnode, ldata)

    expected = ldata
    
    flag, newtree = BSTP.insert_prim(atnode, ldata)
    result = newtree.left.data

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_33():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert right, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    data = 8
    result, newtree = BSTP.insert_prim(atnode, data)

    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_34():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert right, check tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    data = 8
    flag, result = BSTP.insert_prim(atnode, data)

    expected = atnode

    assert result == expected, str_form(test_objective, 'different reference', 'reference to original tree', reason)


def test_35():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert right, check root data'

    atnode = None
    rdata = 5
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    data = 8
    flag, result = BSTP.insert_prim(atnode, data)

    result = atnode.data
    expected = rdata
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_36():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert right, check right tree'
    atnode = None

    data = '5'
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = '8'
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    result = atnode.right

    assert result is not None, str_form(test_objective, None, 'something non None', reason)


def test_37():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert right, check right data'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = 8
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    result = atnode.right.data
    expected = rdata
    
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_38():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, insert right, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = 8
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    result = atnode.left

    assert result is None, str_form(test_objective, 'something non None', None, reason)


def test_39():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat right value, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = 8
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    result, newtree = BSTP.insert_prim(atnode, rdata)
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_40():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat right value, check root reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = 8
    flag, atnode = BSTP.insert_prim(atnode, rdata)
    
    expected = atnode
    
    flag, result = BSTP.insert_prim(atnode, rdata)
    assert result is expected, str_form(test_objective, None, 'not None', reason)


def test_41():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat right value, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = 8
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    result, newtree = BSTP.insert_prim(atnode, rdata)
    expected = False
    assert result is expected, str_form(test_objective, result, expected, reason)


def test_42():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat right value, check left reference'
    atnode = None

    data = '5'
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = '8'
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    expected = atnode.right
    
    flag, newtree = BSTP.insert_prim(atnode, rdata)
    result = newtree.right

    assert result is expected, str_form(test_objective, result, expected, reason)


def test_43():
    test_objective = 'insert_prim()'
    reason = 'insert on singleton BSTPtree, repeat right value, check right data'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    rdata = 8
    flag, atnode = BSTP.insert_prim(atnode, rdata)

    expected = rdata
    
    flag, newtree = BSTP.insert_prim(atnode, rdata)
    result = newtree.right.data

    assert result == expected, str_form(test_objective, result, expected, reason)


#################  member on tree with left child only 
def test_44():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value in root, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_45():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value in root, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = atnode.left

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.left
    
    assert result is expected, str_form(test_objective, 'original left tnode', 'not that', reason)


def test_46():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value in root, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.right
    
    assert result is expected, str_form(test_objective, 'not None', expected, reason)


def test_47():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value in left child, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_48():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value in left child, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = atnode.left

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.left
    
    assert result is expected, str_form(test_objective, 'original left tnode', 'not that', reason)


def test_49():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value in left child, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.right
    
    assert result is expected, str_form(test_objective, None, 'not that', reason)


def test_50():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value not in tree, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, 0)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_51():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value not in tree, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, 4)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)

def test_52():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value not in tree, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, 8)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)


#################  member on tree with right child only 
def test_53():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value in root, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_54():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value in root, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = atnode.right

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.right
    
    assert result is expected, str_form(test_objective, 'original left tnode', 'not that', reason)


def test_55():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value in root, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.left
    
    assert result is expected, str_form(test_objective, 'not None', expected, reason)


def test_56():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value in right child, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_57():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value in right child, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = atnode.right

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.right
    
    assert result is expected, str_form(test_objective, 'original right tnode', 'not that', reason)


def test_58():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value in right child, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    b = BSTP.member_prim(atnode, sdata)
    result = atnode.left
    
    assert result is expected, str_form(test_objective, None, 'not that', reason)


def test_59():
    test_objective = 'member_prim()'
    reason = 'member on tree with right child, value not in tree, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, 0)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_60():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value not in tree, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, 6)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_61():
    test_objective = 'member_prim()'
    reason = 'member on tree with left child, value not in tree, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result = BSTP.member_prim(atnode, 7)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)


#################  delete on tree with left child only 
def test_62():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in root, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result, newtree = BSTP.delete_prim(atnode, data)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_63():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in root, check tree reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    flag, result = BSTP.delete_prim(atnode, data)
    
    assert result is not None, str_form(test_objective, None, "a treenode", reason)


def test_64():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in root, check tree data'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    flag, newtree = BSTP.delete_prim(atnode, data)
    result = newtree.data
    expected = 3
        
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_65():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in root, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    b, atnode = BSTP.delete_prim(atnode, sdata)
    result = atnode.right
    expected = None
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_66():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in root, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    b, atnode = BSTP.delete_prim(atnode, sdata)
    result = atnode.left
    expected = None
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_67():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in left child, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result, newtree = BSTP.delete_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_68():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in left child, check returned tree reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = atnode

    flag, result = BSTP.delete_prim(atnode, sdata)
    
    assert result is expected, str_form(test_objective, 'original tnode', 'not that', reason)


def test_69():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in left child, check returned tree data'
    atnode = None

    data = '5'
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = '3'
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = data

    flag, newtree = BSTP.delete_prim(atnode, sdata)
    result = newtree.data 
    
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_70():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in left child, check left tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    flag, newtree = BSTP.delete_prim(atnode, sdata)
    result = newtree.left
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_71():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left child, value in left child, check right tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 3
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    flag, newtree = BSTP.delete_prim(atnode, sdata)
    result = newtree.right
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


#################  delete on tree with right child only 
def test_72():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in root, check return'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result, newtree = BSTP.delete_prim(atnode, data)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_73():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in root, check tree reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    flag, result = BSTP.delete_prim(atnode, data)
    
    assert result is not None, str_form(test_objective, None, "a treenode", reason)


def test_74():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in root, check tree data'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    flag, newtree = BSTP.delete_prim(atnode, data)
    result = newtree.data
    expected = 8
        
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_75():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in root, check left reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    b, atnode = BSTP.delete_prim(atnode, sdata)
    result = atnode.left
    expected = None
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_76():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in root, check right reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    b, atnode = BSTP.delete_prim(atnode, sdata)
    result = atnode.right
    expected = None
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_77():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in right child, check flag'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)

    result, newtree = BSTP.delete_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_78():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in right child, check returned tree reference'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = atnode

    flag, result = BSTP.delete_prim(atnode, sdata)
    
    assert result is expected, str_form(test_objective, 'original tnode', 'not that', reason)


def test_79():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in right child, check returned tree data'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = data

    flag, newtree = BSTP.delete_prim(atnode, sdata)
    result = newtree.data 
    
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_80():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in right child, check right tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    flag, newtree = BSTP.delete_prim(atnode, sdata)
    result = newtree.right
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_81():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with right child, value in left child, check left tree'
    atnode = None

    data = 5
    flag, atnode = BSTP.insert_prim(atnode, data)

    sdata = 8
    flag, atnode = BSTP.insert_prim(atnode, sdata)
    expected = None

    flag, newtree = BSTP.delete_prim(atnode, sdata)
    result = newtree.left
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


#################  delete on tree with both left and right children 
def test_82():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right child, value in root, check return'
    atnode = None

    datas = [5, 3, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    result, newtree = BSTP.delete_prim(atnode, datas[0])
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_83():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right child, value in root, check tree reference'
    atnode = None

    datas = ['5', '3', '8']
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    expected = atnode.left
    
    b, result = BSTP.delete_prim(atnode, datas[0])
        
    assert result is expected, str_form(test_objective, "not the left treenode", "the original left treenode", reason)


def test_84():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right child, value in root, check tree data'
    atnode = None

    datas = [5, 3, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    flag, newtree = BSTP.delete_prim(atnode, datas[0])
    result = newtree.data
    expected = 3
        
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_85():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right child, value in root, check left reference'
    atnode = None

    datas = [5, 3, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    b, atnode = BSTP.delete_prim(atnode, datas[0])
    result = atnode.left
    expected = None
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_86():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right child, value in root, check right reference'
    atnode = None

    datas = [5, 3, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)
    expected = atnode.right

    b, atnode = BSTP.delete_prim(atnode, datas[0])
    result = atnode.right
    
    assert result is expected, str_form(test_objective, 'not the original right tnode', 'the original right tnode', reason)


#################  delete on tree with big left and right subtrees 
def test_87():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right subtrees, value in root, check return'
    atnode = None

    datas = [5, 1, 2, 3, 4, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    result, newtree = BSTP.delete_prim(atnode, datas[0])
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_88():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right subtrees, value in root, check tree reference'
    atnode = None

    datas = [5, 1, 2, 3, 4, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    expected = atnode.left
    
    b, result = BSTP.delete_prim(atnode, datas[0])
        
    assert result is expected, str_form(test_objective, "not the left treenode", "the original left treenode", reason)


def test_89():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right subtrees, value in root, check tree data'
    atnode = None

    datas = [5, 1, 2, 3, 4, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    flag, newtree = BSTP.delete_prim(atnode, datas[0])
    result = newtree.data
    expected = 1
        
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_90():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right subtrees, value in root, check left reference'
    atnode = None

    datas = [5, 1, 2, 3, 4, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)

    b, atnode = BSTP.delete_prim(atnode, datas[0])
    result = atnode.left
    expected = None
    
    assert result is expected, str_form(test_objective, 'something', expected, reason)


def test_91():
    test_objective = 'delete_prim()'
    reason = 'delete on tree with left and right subtrees, value in root, check former right subtree'
    atnode = None

    datas = [5, 1, 2, 3, 4, 8]
    for d in datas:
        flag, atnode = BSTP.insert_prim(atnode, d)
    expected = atnode.right

    b, atnode = BSTP.delete_prim(atnode, datas[0])

    result = atnode
    for i in range(4):
        assert result is not None, str_form(test_objective, 'None', 'a tnode', 'chaining down the test branch')
        result = result.right
    
    assert result is expected, str_form(test_objective, 'not the original right tnode', 'the original right tnode', reason)


#######################################################
def test_92():
    test_objective = 'member_prim()'
    reason = 'voluminous tree, value in tree to left'
    atnode = None

    datas = [10, 5, 15, 3, 7, 12, 17, 4, 6, 16, 25, 20]

    for k in datas:
        flag, atnode = BSTP.insert_prim(atnode, k)

    sdata = 3
    result = BSTP.member_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_93():
    test_objective = 'member_prim()'
    reason = 'voluminous tree, value in tree to right'
    atnode = None

    datas = [10, 5, 15, 3, 7, 12, 17, 4, 6, 16, 25, 20]

    for k in datas:
        flag, atnode = BSTP.insert_prim(atnode, k)

    sdata = 16
    result = BSTP.member_prim(atnode, sdata)
    expected = True

    assert result == expected, str_form(test_objective, result, expected, reason)


def test_94():
    test_objective = 'member_prim()'
    reason = 'voluminous tree, value not in tree'
    atnode = None

    datas = [10, 5, 15, 3, 7, 12, 17, 4, 6, 16, 25, 20]

    for k in datas:
        flag, atnode = BSTP.insert_prim(atnode, k)

    sdata = 3333333333
    result = BSTP.member_prim(atnode, sdata)
    expected = False

    assert result == expected, str_form(test_objective, result, expected, reason)



def test_95():
    test_objective = 'insert_prim() + member_prim()'
    reason = 'voluminous tree, all datas, check all values'
    atnode = None

    datas = ['10', '5', '15', '3', '7', '12', '17', '4', '6', '16', '25', '20']

    for k in datas:
        flag, atnode = BSTP.insert_prim(atnode, k)
        assert flag, str_form(test_objective, False, True, reason + ': sequential insertion into tree failed')

    for v in datas:
        retflag = BSTP.member_prim(atnode, v)
        expected = True
        assert retflag == expected, str_form(test_objective, retflag, expected,
                                             reason + ' failed to find data ' + str(k))


def test_96():
    test_objective = 'insert_prim() + member_prim()'
    reason = 'voluminous tree, all datas, check values not in tree'
    atnode = None

    datas = [10, 5, 15, 3, 7, 12, 17, 4, 6, 16, 25, 20]

    for k in datas:
        flag, atnode = BSTP.insert_prim(atnode, k)
        assert flag, str_form(test_objective, False, True, reason + ': sequential insertion into tree failed')

    for v in [8, 19, 0, 44]:
        retflag = BSTP.member_prim(atnode, v)
        expected = False
        assert retflag == expected, str_form(test_objective, retflag, expected,
                                             reason + ': unexpected success for data' + str(k))


def test_97():
    test_objective = 'insert_prim() + delete_prim()'
    reason = 'voluminous tree, all datas, delete each single value'

    datas = [5, 3, 1, 8, 10, 7, 2]

    for k in datas:
        atnode = None

        # create a tree from scratch with all the datas
        for k2 in datas:
            flag, atnode = BSTP.insert_prim(atnode, k2)
            assert flag, str_form(test_objective, False, True, reason + ': sequential insertion into tree failed')

        # remove just the one data k
        retflag, retval = BSTP.delete_prim(atnode, k)
        expected = True
        assert retflag == expected, str_form(test_objective, retflag, expected,
                                             reason + ': failed to find data ' + str(k))

        # check that the others are all still there
        for v in datas:
            if v != k:
                retflag = BSTP.member_prim(atnode, v)
                expected = True
                assert retflag == expected, str_form(test_objective, retflag, expected,
                                                     reason + ': failed to find data ' + str(k))


def test_98():
    test_objective = 'insert_prim() + delete_prim()'
    reason = 'voluminous tree, all datas, check values not in tree'
    atnode = None

    datas = ['5', '3', '1', '8', '10', '7', '2']

    for k in datas:
        flag, atnode = BSTP.insert_prim(atnode, k)
        assert flag, str_form(test_objective, False, True, reason + ': sequential insertion into tree failed')

    for v in ['6', '19', '0', '44']:
        retflag, retval = BSTP.delete_prim(atnode, v)
        expected = False
        assert retflag == expected, str_form(test_objective, retflag, expected,
                                             reason + ': unexpected deletion for data' + str(k))


def test_99():
    test_objective = 'insert_prim() + delete_prim()'
    reason = 'voluminous tree, all datas, delete values sequentially'

    datas = [5, 3, 1, 8, 10, 7, 2]

    # create a tree from scratch with all the datas
    atnode = None
    for k2 in datas:
        flag, atnode = BSTP.insert_prim(atnode, k2)
        assert flag, str_form(test_objective, False, True, reason + ': sequential insertion into tree failed')

    deleted = []
    for k in datas:
        # remove just the one data k
        retflag, retval = BSTP.delete_prim(atnode, k)
        expected = True
        assert retflag == expected, str_form(test_objective, retflag, expected,
                                             reason + ': failed to find data ' + str(k))
        deleted.append(k)

        # check that the others are all still there
        for v in datas:
            if v not in deleted:
                retflag = BSTP.member_prim(retval, v)
                expected = True
                assert retflag == expected, str_form(test_objective, retflag, expected,
                                                     reason + ': failed to find data ' + str(k))

def test_100():
    test_objective = 'Powers of ten are cool, I guess.  '
    reason = "Come on, 99 tests?  Had to come up with one more."
    expected = 100
    result = 100
    assert result == expected, str_form(test_objective, result, expected, reason)


all_of_em = [
            test_1, test_2, test_3, test_4, test_5, test_6, test_7,
            test_8, test_9, test_10, test_11, test_12, test_13,
            test_14, test_15, test_16, test_17, test_18, test_19,
            test_20, test_21, test_22, test_23, test_24, test_25,
            test_26, test_27, test_28, test_29, test_30, test_31,
            test_32, test_33, test_34, test_35, test_36, test_37,
            test_38, test_39, test_40, test_41, test_42, test_43,
            test_44, test_45, test_46, test_47, test_48, test_49,
            test_50, test_51, test_52, test_53, test_54, test_55,
            test_56, test_57, test_58, test_59, test_60, test_61,
            test_62, test_63, test_64, test_65, test_66, test_67,
            test_68, test_69, test_70, test_71, test_72, test_73,
            test_74, test_75, test_76, test_77, test_78, test_79,
            test_80, test_81, test_82, test_83, test_84, test_85,
            test_86, test_87, test_88, test_89, test_90, test_91,
            test_92, test_93, test_94, test_95, test_96, test_97,
            test_98, test_99, test_100
            ]

score_table = [(5,0), (45,5), (69,10), (79,12), (89,15), (98,18), (100,20)]
Test.runemall("A10Q1", 20, score_table, all_of_em)
    