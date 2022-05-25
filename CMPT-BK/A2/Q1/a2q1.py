# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.



def copyA(data):
    acopy = []
    for d in data:
        acopy.append(d)
    return acopy


def copyB(data, acopy):
    for d in data:
        acopy.append(d)
        data.remove(d)


def copyC(data):
    acopy = data
    return acopy


def copyD(data, acopy):
    for d in data:
        acopy.append(d)


def copyE(data):
    acopy = []
    for d in data:
        acopy += [d]
    return acopy


def selection_sort(unsorted):
    """
    Returns a list with the same values as unsorted,
    but reorganized to be in increasing order.
    :param unsorted: a list of comparable data values
    :return:  a sorted list of the data values
    """

    result = list()

    # TODO use one of the copy() functions here
    
    while len(acopy) > 0:
        out = min(acopy)
        acopy.remove(out)
        result.append(out)

    return result
