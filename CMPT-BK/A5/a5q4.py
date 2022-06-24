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
#    Assignment 5 Question 4

import node as N


def copync(node_chain):
    """
    Purpose:
        creates a duplicate of the given node chain
    Pre-conditions:
        :param node_chain: a node-chain, possibly empty
    Post-conditions:
        None
    Return:
        :return: a new node chain, a node-for node copy 
                 of the given one
    """
    new_node=None
    if node_chain is not None:
        new_node=N.node(node_chain.get_data(),copync(node_chain.get_next()))
    return new_node



def double_up(node_chain):
    """
    Purpose:
        Modifies the node chain so that every node is doubled.
        E.g., given 1 -> 2 -> 3
              changed to 1 -> 1 -> 2 -> 2 -> 3 -> 3
    Pre-conditions:
        :param node_chain: a node-chain, possibly empty

    Post-conditions:
        The chain is modified to have each node repeated once.
    Return:
        None
    """

    walker=node_chain
    while walker is not None:
        value=walker.get_data()
        pre_walker=walker
        walker=walker.get_next()
        new_node=N.node(value,walker)
        pre_walker.set_next(new_node)
    return None

  