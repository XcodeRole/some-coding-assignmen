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
#    Assignment 5 Question 1

import node as N

def to_string(node_chain):
    """
    Purpose:
        Create a string representation of the node chain.  E.g.,
        [ 1 | *-]-->[ 2 | *-]-->[ 3 | / ]
    Pre-conditions:
        :param node_chain:  A node-chain, possibly empty (None)
    Post_conditions:
        None
    Return: A string representation of the nodes.
    """
    # special case: empty node chain
    if node_chain is None:
        result = 'EMPTY'
    else:
        # walk along the chain
        value = node_chain.get_data()
        walker = node_chain.get_next()
        # print the data
        result = '[ {} |'.format(str(value))
        while walker is not None:
            value = walker.get_data()
            walker = walker.get_next()
            # represent the next with an arrow-like figure
            result += ' *-]-->[ {} |'.format(str(value))

        # at the end of the chain, use '/'
        result += ' / ]'

    return result

