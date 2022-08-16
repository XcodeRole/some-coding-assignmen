# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this 
# file to a public or private website, or providing this file to a person 
# not registered in CMPT 145, constitutes Academic Misconduct, according 
# to the University of Saskatchewan Policy on Academic Misconduct.

# A10Q4 
# Synopsis:  
# This file collects data comparing the practical runtimes of THREE implementations
# of the TBase class.  
#     A10Q2: a10q2 implementation
#     A10Q3: Python list implementation (simple unsorted list)
#     A10Q3: Python list implementation (sorted list with binary search)
# 
# To collect practical empirical data, a number of "command files"  were made 
# available, containing random directives to add, remove or check the membership 
# of values in the TBase container.  Each file contains N commands (roughly), and 
# files were made available for increasing N.
# 
# This script runs all the files with all of the implementations, and collects timing data.
# It also collects doubling ratios, i.e., it calculates 
#     ratio_N = Time(2N)/Time(N)
# 
# The timing data and Doubling ratio are plotted using MatPlotLib.


# import the assignment classes
 
import a10q3 as a10q3
import a10q2 as a10q2


# importing Python modules 
import random as rand
import sys as sys
import time as time
import gc as gc
import matplotlib.pyplot as plot


# a few global constants to make the code more adaptable
add_str = 'a'
remove_str = 'r'
member_str = 'm'


def read_script(name):
    """
    Purpose:
        Read a script consisting of a/r/m commands
    Pre-Conditions:
        :param name: a valid filename
    Return
        :return: a list of tuples (c, v)
                 where c is a character, and v is an integer
    """
    file = open(name)
    commands = []
    for line in file:
        line = line.rstrip().split()
        commands.append( (line[0], int(line[1])) )
    file.close()
    return commands


def exec_script(base, script):
    """
    Purpose:
        Execute the list of commands (c, v).  
        We're simulating an application, which will be timed.
        So there is no return. 
    Pre-Conditions:
        :param script: a list of commands (c, v)
                       where c is one of "arm"
                             v is an integer
    Return
        :return: None
    """
    for c, v in script:
        if c == add_str:
            f = base.add(v)
        elif c == remove_str:
            f = base.remove(v)
        elif c == member_str:
            f = base.member(v)
        else:
            assert False, 'Unknown command '+str(c)

 
def time_exec(tbase, script, nruns):
    """
    Purpose:
        Runs the experiment nruns times, and returns the minimum
        out of all the runs.  
    Pre-Conditions:
        :param tbase: a container with the TBase interface 
        :param script: a list of commands (c, v)
                       where c is one of "a", "r", "m"
                             v is an integer
        :param nruns: how often to repeat the execution
    Return
        :return: the minimum time to run the experiment once
    """
    # do the task once, to get a baseline
    start_time = time.perf_counter()
    exec_script(tbase, script)
    end_time = time.perf_counter()
    result = end_time - start_time
    
    # the best time of a number of executions is returned
    for i in range(nruns):
        # tidy up before running the script
        gc.collect()
        
        start_time = time.perf_counter()
        exec_script(tbase, script)
        end_time = time.perf_counter()
        t_time = end_time - start_time
        
        # the minimum because errors don't decrease time!
        result = min(result,t_time)
        
    return result


def run_experiment(sizes, nruns):
    """
    Purpose:
        Run the files indicated by sizes.  Repeat the execution some number of times
    Pre-Conditions:
        :param script: a list of integers N, where N appears in the filename of our data
        :param nruns: a positive integer; the number of times to repeat the execution

    Return
        :return: a dictionary of times.  
                 The key is a size from sizes.  
                 The value is a dictionary too, with one entry for each implementation.
    """
    
    # a list of filenames built from the N values
    names = ['A10Q4/expt'+str(s) for s in sizes]
    
    # collect the data for display and plotting
    data_dict = {}

    # how often to repeat each file
    for size, filename in zip(sizes, names):
        # load the file
        script = read_script(filename+'.txt')

        # time the execution for TBase (a10q2)
        a10q2_time = time_exec( a10q2.TBase(), script, nruns)
                    
        # time the execution for LBase (a10q3)
        a10q3_time = time_exec( a10q3.TBase(), script, nruns)
    
        # store the results
        data_dict[size] = {'T': a10q2_time, 'L':a10q3_time}

    return data_dict


# the main script reads each of the given experiment files
if __name__ == '__main__':

    # check to see if the number of repetitions is given
    if len(sys.argv) == 2:
        nruns = int(sys.argv[1])
    else:
        nruns = 3

    # run the experiment
    sizes = [1000*v for v in [1,2,3,4,5,6,7,8,10,12,14]]
    data_dict = run_experiment(sizes, nruns)
    
    # display timing data in text
    print("Timing Data\n")
    print("{:4} {:6} {:6}".format('N', "a10q2", "a10q3"))
    for size in sizes:
        print("{} {:.4f} {:.4f}".format(size, data_dict[size]['T'], data_dict[size]['L']))

    # display doubling data
    print("\nDoubling Ratios\n")
    print("{:4} {:4} {:4}".format('N', "a10q2", "a10q3"))
    for size in sizes:
        double = 2*size
        # we only want to calculate a doubling ratio is N and 2N are calculated
        if double in sizes:
            print("{} {:.2f} {:.2f}".format(double, data_dict[double]['T']/data_dict[size]['T'], 
                                                           data_dict[double]['L']/data_dict[size]['L']))

    # calculate average doubling ratios.
    print("\nAverage Doubling Ratios\n")
    print("{:4} {:4}".format("a10q2", "List"))
    sum_a10q2 = 0
    sum_a10q3 = 0
    d = 0
    for size in sizes:
        double = 2*size
        # we only want to calculate a doubling ratio is N and 2N are calculated
        if double in sizes:
            d += 1
            sum_a10q2 += data_dict[double]['T']/data_dict[size]['T']
            sum_a10q3 += data_dict[double]['L']/data_dict[size]['L']
                 
    print("{:.2f} {:.2f}".format(sum_a10q2/d, sum_a10q3/d))
 
    # plot the results
    # we plot runtimes and doubling ratios side-by-side
    fig, (ax1, ax2) = plot.subplots(1,2)

    ax1.set(title="Timing curves", ylabel="seconds", xlabel="N")
    ax1.plot(sizes, [data_dict[s]['T'] for s in sizes],'o-')
    ax1.plot(sizes, [data_dict[s]['L'] for s in sizes],'o-')
    ax1.legend(['a10q2', 'a10q3'])

    ax2.set(title="Doubling ratios",  xlabel="N")
    doubles = [s for s in sizes if 2*s in sizes]
    ax2.plot(doubles, [data_dict[2*s]['T']/data_dict[s]['T'] for s in sizes if 2*s in sizes],'o-')
    ax2.plot(doubles, [data_dict[2*s]['L']/data_dict[s]['L'] for s in sizes if 2*s in sizes],'o-')
    ax2.legend(['a10q2', 'a10q3'])

    plot.show()    

# end of file