# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.

# Defines functions for primitive Binary Search Tree data structure
#
# A Primitive Binary Tree is defined as follows:
# 1. The value None is a primitive binary tree;
#    (None represents an empty tree)
# 2. If lt and rt are primitive binary trees, and d is any value
#    treenode(d, lt, rt) is a primitive binary tree.
#
# A treenode is analogous to a Node in a node-chain. 
# 
# A Primitive Binary Tree t satisfies the Binary Search Tree (BST)
# property if all of the following hold:
# 0. If t is None, it satisfies the BST property by definition.
#    Otherwise, we assume t is a treenode object
# 1. The data value stored at treenode t is greater than any data
#    value in t's left subtree (if any)
# 2. The data value stored at treenode t is smaller than any data
#    value in t's right subtree (if any)
# 3. t's left subtree satisfies the BST property
# 4. t's right subtree satisfies the BST property


import treenode as TN


def member_prim(tnode, target):
    """
    Purpose:
        Check if target is stored in the binary search tree.
    Pre-Conditions:
        :param tnode: a treenode with the BST property
        :param target: a value
    Post-Conditions:
        none
    Return
        :return: True if target is in the tree
    """

    if tnode is None:
        return False
    elif tnode.data is target:
        return True
    elif target < tnode.data:
        left = tnode.left
        return member_prim(left, target)
    else:
        right = tnode.right
        return member_prim(right, target)


def insert_prim(tnode, value):
    """
    Purpose:
        Insert a new value into the binary search tree.
    Pre-Conditions:
        :param tnode: a treenode with the BST property
        :param value: a value
    Post-Conditions:
        If the value is not already in the tree, it is added
    Return
        :return: tuple (flag, tree)
            flag is True if insertion modified the tree;
                    tree contains the new value
            flag is False if the value is already in the tree,
                    tree returned unchanged
    """

    if tnode is None:
        return True, TN.treenode(value)
    else:
        if tnode.data == value:
            return False, tnode
        elif value < tnode.data:
            left_flag, left_tree = insert_prim(tnode.left, value)
            if left_flag:
                tnode.left = left_tree
            return left_flag, tnode
        else:
            right_flag, right_tree = insert_prim(tnode.right, value)
            if right_flag:
                tnode.right = right_tree
            return right_flag, tnode


def delete_prim(tnode, target):
    """
    Purpose:
        Delete a target from the binary search tree.
    Pre-Conditions:
        :param tnode: a treenode with the BST property
        :param target: a value
    Post-Conditions:
        If the target is in the tree, it is deleted.
        If the target is not there, there is no change to the tree.
    Return
        :return: tuple (flag, tree)
           -flag is True if deletion succeeded; 
            tree is the resulting without the value
           -flag is False if the value was not in the tree,
            tree returned unchanged
    """

    def delete(tnode):
        '''Internal function does most of the deleting work.
            returns a tuple (flag, tree)
        '''
        if tnode is None:
            return False, None
        else:
            if tnode.data == target:
                return reconnect(tnode)
            elif tnode.data > target:
                flag, subtree = delete(tnode.left)
                if flag:
                    tnode.left = subtree
                return flag, tnode
            else:
                flag, subtree = delete(tnode.right)
                if flag:
                    tnode.right = subtree
                return flag, tnode

    def reconnect(delthisnode):
        '''
            Reconnect the tree by removing delthisnode.
            Internal function implements the 4 cases.
            returns a tuple (flag, tree)
        '''
        if delthisnode.left is None and delthisnode.right is None:
            # Case 1: delthisnode has no children
            return True, None

        elif delthisnode.left is None:
            # Case 2: delthisnode has a right child only
            return True, delthisnode.right

        elif delthisnode.right is None:
            # Case 3: the deleted node has a left child only
            return True, delthisnode.left

        else:
            # Case 4: delthisnode has two children
            oldleft = delthisnode.left
            oldright = delthisnode.right

            walker = oldleft
            while walker.right is not None:
                walker = walker.right

            walker.right = oldright
            return True, oldleft

    # the body of delete_prim() is very short:
    return delete(tnode)

