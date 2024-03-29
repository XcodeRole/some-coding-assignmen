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
#   Defines the Statistics ADT
#   Calculate mean and variance and other statistical summaries  
#   of numeric data.

# Implementation
# Do the calculations without storing all the data!
# Use a Python dictionary as a record to store three quantities:
#   _count':     the number of data values added
#   _avg':       the running average of the values added
#   _sumsqdiff': the sum of the square differences between the
#                values added and the mean so far
# These values can be modified every time a new data value is 
# added, so that the mean and variance can be calculated quickly  
# as needed.  This approach means that we do not need to store  
# the data values themselves, which could save a lot of space.


class Statistics(object):
    def __init__(self):
        """
        Purpose:
            Initialize a Statistics object instance.
        """
        self._count = 0      # how many data values seen so far
        self._avg = 0        # the running average so far
        self._sumsqdiff = 0  # the sum of the square differences

    def add(self, value):
        """
        Purpose:
            Use the given value in the calculation of mean and 
            variance.
        Pre-Conditions:
            :param value: the value to be added
        Post-Conditions:
            none
        Return:
            :return none
        """
        self._count += 1
        k = self._count           # convenience
        diff = value - self._avg  # convenience
        self._avg += diff / k
        self._sumsqdiff += ((k - 1) / k) * (diff ** 2)


    def mean(self):
        """
        Purpose:
            Return the average of all the values seen so far.
        Post-conditions:
            (none)
        Return:
            The mean of the data seen so far.
            Note: if no data has been seen, 0 is returned.
                  This is clearly false.
        """
        return self._avg

    def var(self):
        """
        Purpose:
            Return the variance of all the values seen so far.
            (variance is the average of the squared difference
            between each value and the average of all values)
        Post-conditions:
            (none)
        Return:
            The variance of the data seen so far.
            Note: if 0 or 1 data values have been seen, 0 is 
                  returned.  This is clearly false.
        """
        return self._sumsqdiff / self._count

    def sampvar(self):
        """
        Purpose:
            Return the sample variance of all the values seen 
            so far.
        Post-conditions:
            (none)
        Return:
            The sample variance of the data seen so far.
            Note: if 0 or 1 data values have been seen, 0 is 
                  returned.  This is clearly false.
        """
        return self._sumsqdiff / (self._count - 1)

