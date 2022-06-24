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
#    Assignment 5 Question 2

import node as N


def check_chains(chain1, chain2):
    """
    Purpose:
        Checks 2 node chains.  
           If they are identical (the same objects), 
                returns a string "same chain"
           If they are equal (same data values in the same order), 
                returns a string "same values"
           Otherwise, returns a string "different starting at i"
                where i is an integer indicating the first different
                data value; 
    Pre-conditions:
        :param chain1: a node-chain, possibly empty
        :param chain2: a node-chain, possibly empty
    Post-conditions:
        None
    Return:
        :return: a string
    """
    if chain1 is chain2:
        return "same chain"
    walker1=chain1
    walker2=chain2
    i=0
    while walker1 is not None and walker2 is not None:
        value1=walker1.get_data()
        value2=walker2.get_data()
        if value1 != value2:
            return "different starting at "+str(i)
        walker1=walker1.get_next()
        walker2=walker2.get_next()
        i+=1
    if walker1 is None and walker2 is None:
        return "same values"
    elif walker1 is None or walker2 is None:
        return "different starting at "+str(i)
    return None