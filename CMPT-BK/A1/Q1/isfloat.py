# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.

# Exceptions are covered in CMPT 270.  You don't need to understand how 
# exceptions work to use this function.


def isfloat(str_val):
    """
    Purpose:
        Check whether a string represents a floating point value
    Preconditions:
        str_val: a string
    Post-conditions:
        None
    Return: 
        True if str_val can be converted to a floating point number
    """
    try:
        float(str_val)
        return True
    except:
        return False
