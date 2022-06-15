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
#   Defines the Statistics ADT
#   Calculate mean and variance and other statistical summaries  
#   of numeric data.

# Implementation
# Does the calculations without storing all the data!
# A Statistics object has the following private attributes:
#   __count:     the number of data values added
#   __avg:       the running average of the values added

# These values are modified every time a new data value is 
# added, so that the mean and variance can be calculated quickly  
# as needed.  This approach means that we do not need to store  
# the data values themselves, which could save a lot of space.
#
# NOTE: This file does not contain the full ADT as shown in class.  
#       The calculations for var() and sampvar() were removed to 
#       keep the exercise concise.



class Statistics(object):
    def __init__(self):
        """
        Purpose:
            Initialize a Statistics object instance.
        """
        self.__count = 0      # how many data values seen so far
        self.__avg = 0        # the running average so far
        self.__max = None
        self.__min = None


    def add(self, value):
        """
        Purpose:
            Use the given value in the calculation of mean and 
            variance.
        Pre-Conditions:
            :param value: the value to be added
        Post-Conditions:
            The state of the Statistics object changes to reflect the addition the value
        Return:
            :return none
        """
        self.__count += 1      
        diff = value - self.__avg  # convenience
        self.__avg += diff / self.__count
        if self.__max==None: #if max is None,then min must be None either
            self.__max=value
            self.__min=value
        else:
            if value<self.__min:
                self.__min=value
            if value > self.__max:
                self.__max=value



    def mean(self):
        """
        Purpose:
            Return the average of all the values seen so far.
        Pre-conditions:
            (none)
        Post-conditions:
            (none)
        Return:
            The mean of the data seen so far.
            Note: if no data has been seen, 0 is returned.
                  This is clearly false, but a better technique is
                  in the future.
        """
        return self.__avg

    def count(self):
        """
        Purpose:
            Return the number of values seen so far.
        Pre-conditions:
            (none)
        Post-conditions:
            (none)
        Return:
            The number of values seen so far.
        """
        return self.__count
        
    def maximum(self):
        return self.__max

    def minimum(self):
        return self.__min