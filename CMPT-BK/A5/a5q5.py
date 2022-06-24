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
#    Assignment 5 Question 5

import node as N


def split_chain(node_chain):
    """
    Purpose:
        Splits the given node chain in half, returning the second half.
        If the given chain has an odd length, the extra node is part of
        the second half of the chain.
    Pre-conditions:
        :param node_chain: a node-chain, possibly empty
    Post-conditions:
        the original node chain is cut in half!
    Return:
        :return: A tuple (nc1, nc2) where nc1 and nc2 are node-chains
         each containing about half of the nodes in node-chain
    """
    counter=0
    walker=node_chain
    while walker is not None:
        counter+=1
        walker=walker.get_next()
    nc1=None
    nc2=None
    if counter==1:
        nc2=node_chain
    else:
        nc1=node_chain
        half=counter//2
        walker=node_chain
        counter=0
        while counter !=half:
            counter+=1
            if counter==half and walker is not None:
                pre_walker=walker
            walker=walker.get_next()
            if counter==half and walker is not None:
                pre_walker.set_next(None)
        if walker is not None:
            nc2=walker
    return nc1,nc2


def merge(nc1, nc2):
    """
    Purpose:
        Combine the two sorted node-chains nc1 and nc2 into a single
        sorted node-chain.
    Pre-conditions:
        :param nc1: a node-chain, possibly empty,
        containing values sorted in ascending order.
        :param nc2: a node-chain, possibly empty,
        containing values sorted in ascending order.
    Post-condition:
        None
    Return:
        :return: a sorted node chain (nc) that contains the 
        values from nc1 and nc2. If both node-chains are 
        empty an empty node-chain will be returned.
    """
    if nc1 is None:
        return nc2
    elif nc2 is None:
        return nc1
    else:
        nc3=None
        walker1=nc1
        walker2=nc2
        walker3=nc3
        new_node=None
        while walker1 is not None and walker2 is not None:
            value1=walker1.get_data()
            value2=walker2.get_data()
            if value1 < value2:    
                new_node=N.node(value1,None)
                walker1=walker1.get_next()
            else:
                new_node=N.node(value2,None)
                walker2=walker2.get_next()
            if nc3 is None:
                nc3=new_node
                walker3=nc3
            else:
                walker3.set_next(new_node)
                walker3=walker3.get_next()
        if walker1 is not None:
            walker3.set_next(walker1)
        if walker2 is not None:
            walker3.set_next(walker2)
    return nc3


def merge_sort(node_chain):
    """
    Purpose:
        Sorts the given node chain in ascending order using the
        merge sort algorithm.
    Pre-conditions:
        :param node_chain: a node-chain, possibly empty,
        containing only numbers
    Post-condition:
        the original node_chain may be modified and will likely
        not contain all the original elements
    Return
        :return: a new node-chain with the same values as the 
        original node chain sorted in ascending order.
        Ex: 45->1->21->5. Becomes 1->5->21->45
    """
    if node_chain is None or node_chain.get_next() is None:
        return node_chain
    nc1,nc2=split_chain(node_chain)
    nc1=merge_sort(nc1)
    nc2=merge_sort(nc2)
    nc=merge(nc1,nc2)
    return nc
