from treenode import treenode
def collect_inorder(tnode:treenode):
    """
    Purpose
        Returns a list of data values from 
        the given primitive binary tree consistent with 
        an in-order traversal of the tree
    Post-condition:
        tnode:the binary tree
    Return:
        a list
    """
    if tnode is None:
        return []
    lchild=collect_inorder(tnode.left)
    rootdata=[tnode.data]
    rchild=collect_inorder(tnode.right)
    return lchild+rootdata+rchild

def collect_preorder(tnode):
    """
    Purpose
        Returns a list of data values from the given primitive 
        binary tree consistent with a pre-order traversal of the tree
    Post-condition:
        tnode:the binary tree
    Return:
        a list
    """
    if tnode is None:
        return []
    rootdata=[tnode.data]
    lchild=collect_preorder(tnode.left)
    rchild=collect_preorder(tnode.right)
    return rootdata+lchild+rchild

def collect_postorder(tnode):
    """
    Purpose
        Returns a list of data values from the given primitive 
        binary tree consistent with a post-order traversal of the tree
    Post-condition:
        tnode:the binary tree
    Return:
        a list
    """
    if tnode is None:
        return []
    lchild=collect_postorder(tnode.left)
    rchild=collect_postorder(tnode.right)
    rootdata=[tnode.data]
    return lchild+rchild+rootdata