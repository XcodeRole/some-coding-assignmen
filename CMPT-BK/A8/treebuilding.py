# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.
# 
# Synopsis:
#   Defines some functions to build trees in various ways.

import treenode as TN

def build_lecture_example():
    """
    Purpose: 
        returns the example found in the lecture slides
    Return:
        One of the common example trees found in the lectures
    """
    atree = TN.treenode(2)
    a = TN.treenode(7)
    b = TN.treenode(5)
    atree.left = a
    atree.right = b
    c = TN.treenode(11)
    d = TN.treenode(6)
    a.left = c
    a.right = d
    return atree


def build_turtle():
    """
    Purpose: 
        returns a simple tree with letters as data values
        I don't remember why I used the word turtle for this.
    Return:
        A simple tree with letters as data values
    """
    atree = TN.treenode('t', 
                   TN.treenode('u', 
                         TN.treenode('t'), 
                         TN.treenode('r')), 
                   TN.treenode('e', 
                         TN.treenode('l'), 
                         None))
    return atree


def build_xtree_me():
    """    
    Purpose: 
        returns a slightly larger, more or less undistinguished tree.
        The indentation of the code reflects the structure of the tree.
    """
    xtree = TN.treenode(5,
                  TN.treenode(1,
                        None,
                        TN.treenode(4,
                              TN.treenode(3,
                                    TN.treenode(2, None, None),
                                    None),
                              None)),
                  TN.treenode(9,
                        TN.treenode(8,
                              TN.treenode(7,
                                    TN.treenode(6, None, None),
                                    None),
                              None),
                        TN.treenode(1,
                              TN.treenode(3,None,None),
                              TN.treenode(3,None,None))))
    return xtree


def build_expr_tree():
    """
    Purpose:
        Returns a particular tree that reflects an arithmetic expression
        The expression is '((2.0 + 3.0) + (3.0)) * ((4.0) + (2.0 / (89.0 + 3.0)))'
        (but the tree uses hierarchy to avoid the need for brackets!)
    """
    expr_tree = TN.treenode('*',
                      TN.treenode('+',
                                TN.treenode('+',
                                          TN.treenode(2.0, None, None),
                                          TN.treenode(3.0, None, None)),
                                TN.treenode(3.0, None, None)),
                      TN.treenode('+',
                                TN.treenode(4.0, None, None),
                                TN.treenode('/',
                                          TN.treenode(2.0, None, None),
                                          TN.treenode('+',
                                                    TN.treenode(89.0, None, None),
                                                    TN.treenode(3.0, None, None)))))
    return expr_tree


def treeify(alist):
    """
    Purpose:
        Create a tree using the given list.
        The first node in the tree is the data value of the root.
        The first half of the remaining nodes form the left subtree.
        The second half of the remaining nodes form the right subtree.
        If the list has an even length, the left subtree will be slightly 
        bigger than the right
    Pre-conditions:
        :param alist: a Python list
    Return:
        :return: a primitive tree structure (tnode)
    """

    if len(alist) == 0:
        return None
    elif len(alist) == 1:
        return TN.treenode(alist[0])
    else:
        mid = 1 + len(alist)//2
        return TN.treenode(alist[0], treeify(alist[1:mid]), treeify(alist[mid:]))


def build_complete(height, d=0):
    """
    Purpose:
        Create a complete binary tree of the given height.
        The data value for the root of the tree is d
    Pre-conditions:
        :param height: a non-negative integer
        :param d: an integer, used as the value for the root of the tree
    Return:
        :return: a complete primitive binary tree whose root has data value d
    """
    if height == 0:
        return None
    else:
        return TN.treenode(d, build_complete(height-1,2*d+1), 
                              build_complete(height-1,2*d+2))


def build_fibtree(n):
    """
    Purpose:
        Build a tree whose structure represents the Fibonacci numbers.
        The root of the tree has data value Fib(n).
    Pre-conditions:
        :param n: a non-negative integer
    Return:
        :return: a primitive binary tree whose structure reflects the calculation of fib(n)
    """
    assert n >= 0, "No fibtree for negative n!"
#     assert n < 20, "If you want a fibtree that big, turn off this assertion!"
    
    if n <= 1:
        return TN.treenode(n)
    else:
        ltree = build_fibtree(n-1)
        rtree = build_fibtree(n-2)
        return TN.treenode(ltree.data+rtree.data, ltree, rtree)

