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
# Synopsis: Binary trees
#    defines a few convenience functions that allow us to test functions on primitive trees
#    For functions that return primitive trees as created by treenode(), we need some way 
#    to check the structure of the trees.  One way is to convert a tree to a kind of string.
#    This assumes that the structure is a valid tree.

import Queue as Q

def in_to_string(tnode):
    """
    Purpose:
        Return a string representing a primitive binary tree.  
        The string is composed of the data values of the tree,
        ordered by the in-order traversal.
    Pre-conditions:
        :param tnode: a primitive tree
    Post-conditions:
        The tree is unaffected
    Return
        :return: a string representation
    """
    if tnode is None:
        return ""
    else:
        leftbits  = in_to_string(tnode.left)
        rightbits = in_to_string(tnode.right)
        return "{} {} {}".format(leftbits, tnode.data, rightbits)



def breadth_first_to_string(tnode):
    """
    Purpose:
        Return a string representing a primitive binary tree.  
        The string is composed of the data values of the tree,
        ordered by the breadth-first order traversal.
    Pre-conditions:
        :param tnode: a primitive tree
    Post-conditions:
        The tree is unaffected
    Return
        :return: a string representation
    """
    # the nodes queue keeps track of which nodes to look at
    nodes = Q.Queue()
    nodes.enqueue(tnode)
    
    # the order queue stores the data values of the nodes we've looked at
    order = Q.Queue()

    while nodes.size() > 0:
        current = nodes.dequeue()
        if current is not None:
            order.enqueue(current.data)
            nodes.enqueue(current.left)
            nodes.enqueue(current.right)

    # produce a string of the data values from the order queue
    result = []
    while not order.is_empty():
        n = order.dequeue()
        result.append(str(n))

    return " ".join(result)



def to_string_for_testing(tnode):
    """ 
    Purpose:    
        Create a complex string that uniquely identifies a tree.
        The combination of inorder nodes and breadth-first nodes uniquely identifies
        a tree structure, assuming the data values are not repeated.
    Pre-conditions:
        :param tnode: a primitive tree
    Post-conditions:
        The tree is unaffected
    Return
        :return: a string representation
    """
    return in_to_string(tnode) + '. ' + breadth_first_to_string(tnode)



def to_string_for_printing(tnode, level=0):
    """
    Purpose:
        Produce a formatted string to represent the hierarchy of
        a tree.  Tree diagrams usually have the root at the top.
        Here the root is at the top left.
        - every data value appears on its own line
         - the levels of a tree are columns from left to right
        - nodes at the same level start in the same column
        - very long data values might cause the presentation to get messy
        - subtrees appear below a parent, indented by a tab
            - left subtree immediately
            - right subtree after the entire left subtree
            - the level determines the number of tabs for indentation
    Pre-conditions:
        :param tnode: a primitive binary tree (treenode or None)
        :param level: the level of the tnode (default value 0)
    Return:
        A string with the hierarchy of the tree, that can be printed.
    """
    result = '\t'*level

    if tnode is None:
        return result+'EMPTY'
    else:
        result += str(tnode.data)
        if tnode.left is not None:
            result += '\n'+to_string_for_printing(tnode.left, level+1)
        if tnode.right is not None:
            result += '\n'+to_string_for_printing(tnode.right, level+1)
        return result
