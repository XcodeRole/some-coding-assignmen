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
#   Implements the Queue ADT
#
# A queue (also called a FIFO queue) is a compound data 
# structure in which the data values are ordered according to 
# the FIFO (first-in first-out) protocol.

class Queue(object):

    def __init__(self):
        """
        Purpose
            creates an empty queue
        Return
            an empty queue
        """
        self.data = list()

    def is_empty(self):
        """
        Purpose
            checks if the given queue has no data in it
        Return:
            True if the queue has no data, or false otherwise
        """
        return len(self.data) == 0


    def size(self):
        """
        Purpose
            returns the number of data values in the given queue
        Return:
            The number of data values in the queue
        """
        return len(self.data)


    def enqueue(self, value):
        """
        Purpose
            adds the given data value to the given queue
        Pre-conditions:
            queue: a queue created by create()
            value: data to be added
        Post-condition:
            the value is added to the queue
        Return:
            (none)
        """
        self.data.append(value)


    def dequeue(self):
        """
        Purpose
            removes and returns a data value from the given queue
        Pre-conditions:
            queue: a queue created by create()
        Post-condition:
            the first value is removed from the queue
        Return:
            the first value in the queue
        """
        return self.data.pop(0)

    def peek(self):
        """
        Purpose
            returns the value from the front of given queue
            without removing it
        Pre-conditions:
            queue: a queue created by create()
        Post-condition:
            None
        Return:
            the value at the front of the queue
        """
        return self.data[0]
