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
#     Defines the List ADT
#  

from ast import walk
from errno import ESTALE
import ssl
from time import sleep


class node(object):
    """ A version of the node class with **public** attributes.
        This makes the use of node objects a bit more convenient for 
        implementing LList class.  
        
        IMPORTANT: Since there are no setters and getters, we use the attributes directly.
        
        This is safe because the node class is defined in this module.  
        No one else will use this version of the class.
    """

    def __init__(self, data, next=None):
        """
        Create a new node for the given data.
        Pre-conditions:
            data:  Any data value to be stored in the node
            next:  Another node (or None, by default)
        """
        self.data = data
        self.next = next
    
    # Note: use the attributes directly; no setters or getters!


class LList(object):
    def __init__(self):
        """
        Purpose
            creates an empty LList object
        """
        self._size = 0     # how many elements in the stack
        self._head = None  # the node chain starts here; initially empty
        self._tail = None


    def is_empty(self):
        """
        Purpose
            Checks if the given LList object has no data in it
        Return:
            :return True if the LList object has no data, or False otherwise
        """
        if self._size==0:
            return True
        return False


    def size(self):
        """
        Purpose
            Returns the number of data values in the given LList object
        Return:
            :return The number of data values in the LList object
        """
        return self._size


    def add_to_front(self, value):
        """
        Purpose
            Insert value at the front of the LList object
        Preconditions:
            :param value:   a value of any kind
        Post-conditions:
            The LList object increases in size.
            The new value is at index 0.
            The values previously in the LList object appear after the new value.
        Return:
            :return None
        """
        anode=node(value,self._head)
        self._head=anode
        self._size+=1
        if self._size==1:
            self._tail=anode
        return None


    def add_to_back(self, value):
        """
        Purpose
            Insert value at the end of the LList object
        Preconditions:
            :param value:   a value of any kind
        Post-conditions:
            The LList object increases in size.
            The new value is last in the LList object.
        Return:
            :return None
        """
        anode=node(value)
        if self.is_empty():
            self._head=anode
            self._tail=anode
            self._size=1
        else:
            self._tail.next=anode
            self._tail=anode
            self._size+=1
        return None


    def remove_from_front(self):
        """
        Purpose
            Removes and returns the first value 
        Post-conditions:
            The LList object decreases in size.
            The returned value is no longer in in the LList object.
        Return:
            :return The pair (True, value) if self is not empty
            :return The pair (False, None) if self is empty
        """
        if not self.is_empty():
            value=self._head.data
            self._head=self._head.next
            self._size-=1
            if self._size==0:
                self._tail=None
            return True,value
        else:
            return False,None


    def get_index_of_value(self, value):
        """
        Purpose
            Return the smallest index of the given value.
        Preconditions:
            :param value:   a value of any kind
        Post-conditions:
            none
        Return:
            :return True, index if the value appears in self
            :return False, None if the value does not appear in self
        """
        index=0
        walker=self._head
        while walker is not None:
            if walker.data==value:
                return True,index
            walker=walker.next
            index+=1
        return False,None


    def value_is_in(self, value):
        """
        Purpose
            Check if the given value is in the LList object
        Preconditions:
            :param value:   a value of any kind
        Post-conditions:
            none
        Return:
            :return True if the value is in the LList object, False otherwise
            :return (False if the LList object is empty)
        """
        walker=self._head
        while walker is not None:
            if walker.data==value:
                return True
            walker=walker.next
        return False
        


    def retrieve_data_at_index(self, index):
        """
        Purpose
            Return the value stored at the index
        Preconditions:
            :param index:   a non-negative integer
        Post-conditions:
            none
        Return:
            :return (True, value) if value is stored at index and index is valid
            :return (False, None) if the index is not valid for the LList object
        """
        if index<0 or index>=self._size:
            return False,None
        walker_index=0
        walker=self._head
        while walker is not None:
            if walker_index==index:
                return True,walker.data
            walker=walker.next
            walker_index+=1


    def set_data_at_index(self, index, value):
        """
        Purpose
            Store value at the index
        Preconditions:
            :param value:   a value of any kind
            :param index:   a non-negative integer
        Post-conditions:
            The value stored at index changes to value
        Return:
            :return True if the index was valid, False otherwise
        """
        if index < 0 or index >= self._size:
            return False
        walker=self._head
        walker_index=0
        while walker is not None:
            if walker_index==index:
                walker.data=value
                return True
            walker=walker.next
            walker_index+=1


    def remove_from_back(self):
        """
        Purpose
            Removes and returns the last value
        Post-conditions:
            The LList object decreases in size.
            The returned value is no longer in in the LList object.
        Return:
            :return The pair True, value if self is not empty
            :return The pair False, None if self is empty
        """
        if self.is_empty():
            return False,None
        anode=self._tail
        value=anode.data
        if self._size==1:
            self._head=self._tail=None
            self._size=0
        else:
            pre_tail=self._head
            while pre_tail.next is not self._tail:
                pre_tail=pre_tail.next
            pre_tail.next=None
            self._size-=1
            self._tail=pre_tail
        return True,value
        


    def insert_value_at_index(self, value, index):
        """
        Purpose
            Insert value at index
        Preconditions:
            :param value:   a value of any kind
            :param index:   a valid index for the LList object
        Post-conditions:
            The LList object increases in size.
            The new value is at index.
            The values previously in the LList object at index or later appear after the new value.
        Return:
            :return If the index is valid, insert_value_at_index returns True.
            :return If the index is not valid, insert_value_at_index returns False.
        """
        if index < 0  or index >= self._size+1:
            return False
        if index==0:
            self.add_to_front(value)
        elif index==self._size:
            self.add_to_back(value)
        else:
            pre_walker=None
            walker=self._head
            walker_index=0
            while walker is not None:
                if walker_index==index-1:
                    pre_walker=walker
                elif walker_index==index:
                    anode=node(value)
                    pre_walker.next=anode
                    anode.next=walker
                    self._size+=1
                    break
                walker=walker.next
                walker_index+=1
        return True



    def delete_item_at_index(self, index):
        """
        Purpose
            Delete the value at index.
        Preconditions:
            :param index:   a non-negative integer
        Post-conditions:
            The LList object decreases in size if the index is valid
            The value at index is no longer in the LList object.
        Return:
            :return True if index was valid, False otherwise
        """
        if index< 0 or index >= self._size:
            return False
        elif index==0:
            self.remove_from_front()
        elif index == self._size-1:
            self.remove_from_back()
        else:
            pre_walker=None
            walker=self._head
            walker_index=0
            while walker is not None:
                if walker_index==index-1:
                    pre_walker=walker
                elif walker_index==index:
                    pre_walker.next=walker.next
                    walker.next=None
                    self._size-=1
                    break
                walker=walker.next
                walker_index+=1
        return True