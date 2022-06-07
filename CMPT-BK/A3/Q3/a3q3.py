# CMPT 145 Course material
# Copyright (C) 2017-2020 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.
# 
# Synopsis:
#    SOme functions to test.


def newton(x):
    """
    Purpose
         Computes the square root of a non-negative number
    Preconditions:
        :param x: a non-negative number
    Post-conditions:
        None
    Return
        :return: a number
    """
    root = 1
    while abs(x - root * root) > 0.00001:
        root = (x/root + root) / 2.0
    return root
 
 
def euclid(a, b):
    """
    Purpose
         Calculates the greatest common divisor of two numbers 
    Preconditions:
        :param a: integer must be greater than 0
        :param b: integer must be greater than 0
    Post-conditions:
        None
    Return
        :return: the biggest positive integer that evenly divides both a and b
    """
    while a != b:
        if a > b:
            a = a - b
        else:
            b = b - a
    return a


def pascal(filename):
    """
    Purpose
         Reads a file and puts the contents into a list.
    Preconditions:
        :param filename: a string
    Post-conditions:
        None
    Return
        :return: a tuple (size, data)
    """
    file = open(filename)
    triangle = []
    for line in file:
        line = line.rstrip().split()
        line = [int(d) for d in line]
        triangle.append(line)
    file.close()
    size = triangle[0][0]
    triangle = triangle[1:]
    return (size, triangle)


def turing(alist):
    """
    Purpose
         Removes duplicates in the given list
    Preconditions:
        :param alist: a list
    Post-conditions:
        duplicate values are removed
    Return
        :return: None
    """
    alist.reverse()
    for i in range(len(alist)-1):
        #Add this line , otherwise the proigram will catch IndexError: list index out of range
        if i < len(alist):
            while alist[i] in alist[i+1:]:
                del alist[i]
    alist.reverse()
    # Add this line to return alist in order to using assert
    return alist

