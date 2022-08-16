# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.


# A TBase is a collection of comparable values (number, strings, etc) 
# This implementation uses a pPython list to store the values.
# Methods are:
#    is_empty(self): 
#       Is the collection empty?
#    size(self):  
#       How many unique values are stored?
#    member(self, value):  
#       Is the given value in the collection
#    add(self, value):   
#       Add the given value to the collection
#    remove(self, value):  
#       Remove the given value from the collection

class TBase(object):

    def __init__(self):
        # the stuff will be stored in a normal Python list
        self.__stuff = []


    def is_empty(self):
        """
        Purpose:
            Check if the TBase object is empty.
        Return
            :return: True if it's empty, False otherwise
        """
        return len(self.__stuff)==0
      
        
    def size(self):
        """
        Purpose:
            Return the number of elements in the TBase object
        Return
            :return: an integer indicating the number of elements
        """
        return len(self.__stuff)


    def member(self, value):
        """
        Purpose:
            Check if target is stored in the TBase object.
        Pre-Conditions:
            :param value: a value, 
             Must be comparable to the other values in the TBase object
        Return
            :return: True if value is in the object
        """
        return value in self.__stuff


    def add(self, value):
        """
        Purpose:
            Store the given value in the TBase object.
        Pre-Conditions:
            :param value: a value
             Must be comparable to the other values in the TBase object
        Return
            :return: True if value was added
                     False if it was already there
        """
        if self.member(value):
            return False
        self.__stuff.append(value)
        return True 
        
    def remove(self, value):
        """
        Purpose:
            Remove the given value from the TBase object.
        Pre-Conditions:
            :param value: a value
             Must be comparable to the other values in the TBase object
        Return
            :return: True if value was removed
                     False if the value was not there at all
        """
        if not self.member(value):
            return False
        self.__stuff.remove(value)
        return True

