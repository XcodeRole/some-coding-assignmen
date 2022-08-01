from treenode import *
def check_trees(t1, t2):
    """
    Purpose
        To check if tree t1 has exactly the same data values 
        in the same places as tree t2
    Post-condition:
        t1:the first binary tree
        t2:the second binary tree
    Return:
        True or False
    """
    if (t1 is None and t2 is not None) or (t1 is not None and t2 is None):
        return False
    if t1 is None and t2 is None:
        return True
    lcheck=check_trees(t1.left, t2.left)
    rcheck=check_trees(t1.right, t2.right)
    rootcheck=t1.data==t2.data
    return lcheck and rcheck and rootcheck

def replace(tnode, t, r):
    """
    Purpose
        To replace a target value t with a replacement value r 
        wherever it appears as a data value in the given tree.
    Post-condition:
        tnode:the given binary tree
        t:target value
        r:replacement value
    Return:
        None
    """
    if tnode is None:
        return None
    if tnode.data==t:
        tnode.data=r
    replace(tnode.left, t, r)
    replace(tnode.right, t, r)
    return None

def copy(tnode):
    """
    Purpose
        To create an exact copy of the given tree, with completely new treenodes, but
        exactly the same data values, in exactly the same places
    Post-condition:
        tnode:the given binary tree to copy with
    Return:
        None if tnode is None;
        A reference to the new tree if tnode is not None
    """
    if tnode is None:
        return None
    anode=treenode(tnode.data)
    anode.left=copy(tnode.left)
    anode.right=copy(tnode.right)
    return anode