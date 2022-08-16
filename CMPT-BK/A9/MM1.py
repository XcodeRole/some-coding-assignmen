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
#   Application: M/M/1 Queuing simulation
#   (based on example by Sedgewick, Wayne, Dondero)
#
# Simulate the arrival and service of customers using a queue.
# Calculate the average wait time for customers, given 
# assumptions about the rate of arrival and service.

import Queue as Q
import Statistics as S
import random as rand

# the assumptions
arrival_rate = 20/60  # the number of customer arrivals per minute
service_rate = 20/60  # the number of customers served per minute
sim_length = 8*60  # the end of the simulation in minutes


def sample_time(x):
    '''
    Return a random sample time until an event.
    In the long run, the events will have a rate of x
    events per unit time.

    Preconditions:
        x: the desired arrival rate, per unit time
    Post-conditions:
        none
    Return:
        a random sample time that obeys the rate x
    '''
    return rand.expovariate(x)


# customers wait in a service queue
service_queue = Q.Queue()

# we're interested in the average time in the queue
waiting = S.Statistics()

# keep track of when a new customer will arrive
nextArrival = sample_time(arrival_rate)

# keep track of when a service event is complete
nextService = nextArrival + sample_time(service_rate)


# simulate the arrival-service process
while nextService < sim_length:

    # handle all arrivals that occur before service is complete
    while nextArrival < nextService:
        service_queue.enqueue(nextArrival)
        nextArrival = nextArrival + sample_time(arrival_rate)

    # service time is over; serve first customer in the queue
    this_arrival = service_queue.dequeue()

    # how long was the customer waiting?
    waited = nextService - this_arrival
    waiting.add(waited)

    # determine when service to the next customer will end
    if service_queue.is_empty():
        nextService = nextArrival + sample_time(service_rate)
    else:
        nextService = nextService + sample_time(service_rate)

print('Average wait time:', waiting.mean())
