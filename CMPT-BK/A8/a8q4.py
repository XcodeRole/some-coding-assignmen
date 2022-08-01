from Queue import *
import math
def complete(tnode):
    """
    Purpose:
        Determine if the given tree is complete.
    Pre -conditions:
        :param tnode: a primitive binary tree
    Return
        :return: A tuple (True , height) if the tree is complete ,
        A tuple (False , None) otherwise.
    """
    finish = False
    queue = [tnode]
    count=1
    for node in queue:
        if node.left:
            if finish:
                return False,None
            queue.append(node.left)
            count+=1
        else:
            finish = True
        if node.right:
            if finish:
                return False,None
            queue.append(node.right)
            count+=1
        else:
            finish = True
    return True,math.floor(math.log2(count))+1

