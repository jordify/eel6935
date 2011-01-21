#!/usr/bin/python2
"""
File: hw1.py
Last Modified: Fri Jan 21, 2011 at 09:59
Author: Jorge Gomez

Description:
  Will add soon I swear
"""

import sys, argparse

class Node(object):
  '''Node class defines the behavior of a single node in this distributed system'''
  def __init__(self,identifier,k):
    self.myID = identifier
    self.k = k
    self.neighbors = {}

  def FindDistance(self,nodeID):
    '''Calculate my distance from a given nodeID'''
    pass

  def ChooseNewNeighbors(self,newNodesList):
    '''Given a list of new nodes and my list of neighbors, choose the closest k
    nodes to become my new neighbor list'''
    pass

  def Gossip(self,nodeID):
    '''Send a node my ID and my neighbors, receive there neighbor list or
    vice-versa'''
    pass

  def SumCurrentDistances(self):
    '''Report on my current neighbors' distances from me'''
    return sum(self.neighbors.keys())


class Simulator(object):
  '''The Simulator class will handle simulating a distributed system of nodes'''
  def __init__(self,cycles,N,k):
    '''This method will represent the network initialization phase'''
    self.cycles = cycles
    self.n = N
    self.k = k

  def Simulate(self):
    '''This method will carry out the network evolution phase for self.cycles'''
    pass


if __name__ == "__main__":
# argparse Class to set up the arguments for the program
# Run the program with the -h or --help flags to read the detailed usage
  parser = argparse.ArgumentParser(description='''Simulate a simple, gossip \
      based construction algorithm and report how node neighbor distances \
      develop over time.''')
  parser.add_argument('-N', action='store', type=int, help='total number of \
      nodes in the network')
  parser.add_argument('-k', action='store', type=int, help='number of \
      neighbors each node maintains')
  parser.add_argument('--graph','-g', action='store_true', default=False, \
      help='produce a graph of neighbor distances vs. cycle')
  parser.set_defaults(N=1024, k=3)

# Get the command line arguments
  cli = parser.parse_args()
  print cli.N,cli.k,cli.graph
