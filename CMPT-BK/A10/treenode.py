# CMPT 145: Binary trees

# This file is copyright (c) Michael C Horsch, provided solely for the
# use of CMPT 145 students.  Students are welcome to use this file
# for their own work, and make copies for their own personal use.
# This file should not be shared for any reason without explicit
# consent of the author.

# Defines the tree node ADT
#
# A treenode is a simple container with three attributes
#   data: the contained information
#   left:  a reference to another treenode or None
#   right: a reference to another treenode or None


class treenode(object):

    def __init__(self, data, left=None, right=None):
        """
        Purpose:
            Create a new treenode for the given data.
        Pre-conditions:
            data:  Any data value to be stored in the treenode
            left, right:  Another treenode (or None, by default)
        Post-condition:
            none
        """
        self.data = data
        self.left = left
        self.right = right

