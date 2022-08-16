# CMPT 145 Course material
# Copyright (c) 2017-2020 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.
# 
# Synopsis:
#   Defines the Stack ADT
#
# A stack (also called a pushdown or LIFO stack) is a compound 
# data structure in which the data values are ordered according 
# to the LIFO (last-in first-out) protocol.
#
# Implementation:
# This implementation uses Python lists directly.  

class Stack(object):
    def __init__(self):
        """
        Purpose
            initializes an empty Stack object
        """
        self.data = list()


    def is_empty(self):
        """
        Purpose
            checks if the stack has no data in it
        Return:
            True if the stack has no data, or false otherwise
        """
        return len(self.data) == 0


    def size(self):
        """
        Purpose
            returns the number of data values in the stack
        Return:
            The number of data values in the queue
        """
        return len(self.data)


    def push(self, value):
        """
        Purpose
            adds the given data value to the stack
        Pre-conditions:
            value: data to be added
        Post-condition:
            the value is added to the stack
        Return:
            (none)
        """
        self.data.append(value)


    def pop(self):
        """
        Purpose
            removes and returns a data value from the stack
        Post-condition:
            the top value is removed from the stack
        Return:
            returns the value at the top of the stack
        """
        return self.data.pop()

    def peek(self):
        """
        Purpose
            returns the value from the front of stack
            without removing it
        Post-condition:
            None
        Return:
            the value at the front of the stack
        """
        return self.data[-1]
