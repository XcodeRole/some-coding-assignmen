from treenode import *
from Queue import *
def nodes_at_level(tnode, level):
    """
    Purpose
        Counts the number of nodes in the given primitive tree are
        at the given level, and returns the count. If level is too 
        big or too small, a zero count is returned.
    Pre-conditions:
        tnode: primitive tree object
        level: a number represent level
    Return:
        The number of node in the given level
    """
    assert(level>=0)
    if tnode is None:
        return None
    q=Queue()
    q.enqueue(tnode)
    atlevel=0
    while not q.is_empty():
        atlevel+=1
        if atlevel==level:
            break
        count=q.size()
        for i in range(count):
            anode=q.dequeue()
            if anode.left is not None:
                q.enqueue(anode.left)
            if anode.right is not None:
                q.enqueue(anode.right)
    if atlevel!=level:
        return 0
    else:
        return q.size()



def largest_leaf_value(tnode):
    """
    Purpose
        searches the given primitive tree and returns the largest 
        data value stored at any leaf node.
    Pre-conditions:
        tnode: primitive tree object
    Return:
        the largest data value stored at any leaf node
    """
    if tnode is None:
        return None
    max=tnode.data
    q=Queue()
    q.enqueue(tnode)
    while not q.is_empty():
        anode=q.dequeue()
        if anode.data > max:
            max=anode.data
        if anode.left is not None:
            q.enqueue(anode.left)
        if anode.right is not None:
            q.enqueue(anode.right)
    return max
        

def closest(tnode, target):
    """
    Purpose
        searches the given primitive binary tree, and returns the data value
        that is closest to the given target
    Pre-conditions:
        tnode: primitive tree object
        target : a given number
    Return:
        the data value that is closest to the given target
    """
    if tnode is None:
        return None
    min=abs(tnode.data-target)
    minnode=tnode
    q=Queue()
    q.enqueue(tnode)
    while not q.is_empty():
        anode=q.dequeue()
        if abs(anode.data-target) < min:
            min=abs(anode.data-target)
            minnode=anode
        if anode.left is not None:
            q.enqueue(anode.left)
        if anode.right is not None:
            q.enqueue(anode.right)
    return minnode.data
