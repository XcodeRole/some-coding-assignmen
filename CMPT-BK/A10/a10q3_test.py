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
import a10q3 as T

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


def test_0():
    test_objective = 'Multiples of ten are cool, I guess.  '
    reason = "Come on, 19 tests?  Had to come up with one more."
    expected = 20
    result = 20
    assert result == expected, str_form(test_objective, result, expected, reason)
    
def test_1():
    test_objective = 'TBase is_empty()'
    reason = 'Empty TBase'
    tset = T.TBase()
    result = tset.is_empty()
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_2():
    test_objective = 'TBase size()'
    reason = 'Empty TBase'
    tset = T.TBase()
    result = tset.size()
    expected = 0
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_3():
    test_objective = 'TBase member()'
    reason = 'Empty TBase'
    tset = T.TBase()
    result = tset.member(1)
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_4():
    test_objective = 'TBase remove()'
    reason = 'Empty TBase'
    tset = T.TBase()
    result = tset.remove('a')
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_5():
    test_objective = 'TBase add()'
    reason = 'Empty TBase'
    tset = T.TBase()
    result = tset.add('a')
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_6():
    test_objective = 'TBase is_empty()'
    reason = 'Singleton TBase'
    tset = T.TBase()
    tset.add('a')
    result = tset.is_empty()
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_7():
    test_objective = 'TBase size()'
    reason = 'Singleton TBase'
    tset = T.TBase()
    tset.add('a')
    result = tset.size()
    expected = 1
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_8():
    test_objective = 'TBase member()'
    reason = 'Singleton TBase, checking non-element'
    tset = T.TBase()
    tset.add('a')
    result = tset.member('b')
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_9():
    test_objective = 'TBase member()'
    reason = 'Singleton TBase, checking element'
    tset = T.TBase()
    tset.add('a')
    result = tset.member('a')
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)

def test_10():
    test_objective = 'TBase remove()'
    reason = 'Singleton TBase, remove element'
    tset = T.TBase()
    tset.add('a')
    result = tset.remove('a')
    expected = True
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_11():
    test_objective = 'TBase remove() + size()'
    reason = 'Singleton TBase, remove element, check size'
    tset = T.TBase()
    tset.add('a')
    f = tset.remove('a')
    result = tset.size()
    expected = 0
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_12():
    test_objective = 'TBase remove() + member()'
    reason = 'Singleton TBase, remove element, check element'
    tset = T.TBase()
    tset.add('a')
    f = tset.remove('a')
    result = tset.member('a')
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_13():
    test_objective = 'TBase remove()'
    reason = 'Singleton TBase, remove non-element'
    tset = T.TBase()
    tset.add('a')
    result = tset.remove('b')
    expected = False
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_14():
    test_objective = 'TBase remove()'
    reason = 'Singleton TBase, remove non-element, check size'
    tset = T.TBase()
    tset.add('a')
    f = tset.remove('b')
    result = tset.size()
    expected = 1    
    assert result == expected, str_form(test_objective, result, expected, reason)


def test_15():
    test_objective = 'TBase add() + member()'
    reason = 'voluminous TBase, check member'
    tset = T.TBase()
    
    datas = ['10', '5', '15', '3', '7', '12', '17', '4', '6', '16', '25', '20']

    for k in datas:
        result = tset.add(k)
        expected = True
        assert result == expected, str_form(test_objective, result, expected, reason + ': sequential insertion failed for '+str(k))

    for v in datas:
        result = tset.member(v)
        expected = True
        assert result == expected, str_form(test_objective, retflag, expected,
                                             reason + ' failed to find data ' + str(k))

def test_16():
    test_objective = 'add() + member()'
    reason = 'voluminous tree, all datas, check values not in tree'
    tset = T.TBase()

    datas = [10, 5, 15, 3, 7, 12, 17, 4, 6, 16, 25, 20]

    for k in datas:
        result = tset.add(k)
        expected = True
        assert result == expected, str_form(test_objective, result, expected, reason + ': sequential insertion failed for '+str(k))

    for v in [8, 19, 0, 44]:
        result = tset.member(v)
        expected = False
        assert result == expected, str_form(test_objective, result, expected,
                                             reason + ': unexpected success for data' + str(k))


def test_17():
    test_objective = 'add() + remove()'
    reason = 'voluminous tree, all datas, delete each single value'

    datas = [5, 3, 1, 8, 10, 7, 2]

    for k in datas:
        # create from scratch with all the datas
        tset = T.TBase()

        for k2 in datas:
            result = tset.add(k2)
            expected = True
            assert result == expected, str_form(test_objective, result, expected, reason + ': sequential insertion failed for '+str(k))

        # remove just the one data k
        result = tset.remove(k)
        expected = True
        assert result == expected, str_form(test_objective, result, expected,
                                             reason + ': failed to find data ' + str(k))

        # check that the others are all still there
        for v in datas:
            if v != k:
                result = tset.member(v)
                expected = True
                assert result == expected, str_form(test_objective, result, expected,
                                                     reason + ': failed to find data ' + str(k))


def test_18():
    test_objective = 'add() + remove()'
    reason = 'voluminous tree, all datas, check values not in tree'
    tset = T.TBase()

    datas = ['5', '3', '1', '8', '10', '7', '2']

    for k in datas:
        result = tset.add(k)
        expected = True
        assert result == expected, str_form(test_objective, result, expected, reason + ': sequential insertion failed for '+str(k))

    for v in ['6', '19', '0', '44']:
        result = tset.remove(v)
        expected = False
        assert result == expected, str_form(test_objective, result, expected,
                                             reason + ': unexpected deletion for data' + str(k))


def test_19():
    test_objective = 'add() + remove()'
    reason = 'voluminous tree, all datas, delete values sequentially'
    tset = T.TBase()

    datas = [5, 3, 1, 8, 10, 7, 2]

    # create a tree from scratch with all the datas
    for k in datas:
        result = tset.add(k)
        expected = True
        assert result == expected, str_form(test_objective, result, expected, reason + ': sequential insertion failed for '+str(k))

    deleted = []
    for k in datas:
        # remove just the one data k
        result = tset.remove(k)
        expected = True
        assert result == expected, str_form(test_objective, result, expected,
                                             reason + ': failed to find data ' + str(k))
        deleted.append(k)

        # check that the others are all still there
        for v in datas:
            if v not in deleted:
                result = tset.member(v)
                expected = True
                assert result == expected, str_form(test_objective, result, expected,
                                                     reason + ': failed to find data ' + str(k))


all_of_em = [
            test_1, test_2, test_3, test_4, test_5, test_6, test_7,
            test_8, test_9, test_10, test_11, test_12, test_13,
            test_14, test_15, test_16, test_17, test_18, test_19,
            test_0
            ]

score_table = [(1,0), (9,5), (18,8), (20,10)]
Test.runemall("A10Q3", 10, score_table, all_of_em)