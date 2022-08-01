from audioop import reverse


def path_to(tnode, value):
    '''
    My train of thought:
        Use the stack data structure, and then traverse the tree structure. 
        When the target node is accessed, the data saved in the stack is the path
    Purpose:
        returns the tuple (True, alist) if the given value appears in the tree, 
        where alist is a Python list with the data values found on the path, 
        ordered as described above. If the value does not appear in the tree at all, 
        return the tuple (False, None)
    Pre -conditions:
        :param tnode: a primitive binary tree
        :param value: the given value
    return :
        a tuple which has been explained in the purpose 
    '''
    if tnode is None:
        return False
    node=tnode
    stack = []
    pre = None
    while stack or node:
        if node:
            stack.append(node)
            node=node.left
        else:
            node=stack.pop()
            if node.right and node.right is not pre:
                stack.append(node)
                node=node.right
                stack.append(node)
                node=node.left
            else:
                # post visits
                if node.data==value:
                    res=[]
                    for anode in stack:
                        res.append(anode.data)
                    return True,res
                pre=node
                node=None
    return False,None

def find_path(tnode, val1, val2):
    find1,path1=path_to(tnode,val1)
    find2,path2=path_to(tnode,val2)
    if find1 and find2:
        pass
