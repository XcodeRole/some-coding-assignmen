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
#   Implements the Queue ADT
#
# A queue (also called a FIFO queue) is a compound data 
# structure in which the data values are ordered according to 
# the FIFO (first-in first-out) protocol.
#
# Implementation notes:
# This implementation uses the linked node structure.


class node(object):
    def __init__(self, data, next=None):
        """
        Purpose
            creates an empty queue
        """
        self.data = data     
        self.next = next


class Queue(object):

    def __init__(self):
        """
        Purpose
            creates an empty queue
        """
        self.__size = 0      # how many elements in the queue
        self.__front = None  # the node chain starts here
        self.__back = None   # the node chain ends here


    def size(self):
        """
        Purpose
            returns the number of data values in the queue
        Pre-conditions:
            queue: a queue object
        Return:
            The number of data values in the queue
        """
        return self.__size


    def is_empty(self):
        """
        Purpose
            checks if the given queue has no data in it
        Return:
            True if the queue has no data, or false otherwise
        """
        return self.__size == 0


    def enqueue(self, value):
        """
        Purpose
            adds the given data value to the queue
        Pre-conditions:
            value: data to be added
        Post-condition:
            the value is added to the queue
        Return:
            (none)
        """
        new_node = node(value, None)

        if self.is_empty():
            self.__front = new_node
            self.__back = new_node
        else:
            prev_last_node = self.__back
            prev_last_node.next = new_node
            self.__back = new_node

        self.__size += 1



    def dequeue(self):
        """
        Purpose
            removes and returns a data value from the queue
            Note: the queue cannot be empty!
        Post-condition:
            the first value is removed from the queue
        Return:
            the first value in the queue, or None
        """
        assert not self.is_empty(), 'dequeued an empty queue'

        prev_first_node = self.__front
        result = prev_first_node.data
        self.__front = prev_first_node.next
        self.__size -= 1
        if self.__size == 0:
            self.__back = None
        return result


    def peek(self):
        """
        Purpose
            returns the value from the front of queue
            without removing it
            Note: the queue cannot be empty!
        Post-condition:
            None
        Return:
            the value at the front of the queue
        """
        assert not self.is_empty(), 'peeked into an empty queue'

        first_node = self.__front
        result = first_node.data
        return result
