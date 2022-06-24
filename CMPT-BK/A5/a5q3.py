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
#    Assignment 5 Question 3

from itertools import count
import node as N


def sumnc(node_chain):
    """
    Purpose:
        Given a node chain with numeric data values, calculate 
        the sum of the data values.
    Pre-conditions:
        :param node_chain: a node-chain, possibly empty, containing 
                           numeric data values
    Post-condition:
        None
    Return
            :return: the sum of the data values in the node chain
    """
    walker=node_chain
    sum=0
    while walker is not None:
        value=walker.get_data()
        walker=walker.get_next()
        sum+=value
    return sum
    

def count_in(node_chain, value):
    """
    Purpose:
        Counts the number of times a value appears in a node chain
    Pre-conditions:
        :param node_chain: a node chain, possibly empty
        :param value: a data value
    Return:
        :return: The number times the value appears in the node chain
    """
    walker=node_chain
    counter=0
    while walker is not None:
        val=walker.get_data()
        walker=walker.get_next()
        if value==val:
            counter+=1
    return counter


def replace_in(node_chain, target, replacement):
    """
    Purpose:
        Replaces each occurrence of the target value with the replacement
    Pre-conditions:
        :param node_chain: a node-chain, possibly empty
        :param target: a value that might appear in the node chain
        :param replacement: the value to replace the target
    Pre-conditions:
        Each occurrence of the target value in the chain is replaced with 
        the replacement value.
    Return:
        None
    """
    walker=node_chain
    while walker is not None:
        value=walker.get_data()
        if value==target:
            walker.set_data(replacement)
        walker=walker.get_next()
    return None


