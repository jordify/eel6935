#!/usr/bin/python2
"""
File: hw1.py 
Last Modified: Tue Jan 25, 2011 at 11:01
Author: Jorge Gomez

Description:
Simulation of a gossip-based topology managment algorithm for peer-to-peer
systems. Takes as input system size, neighbor list size, and simulation cycles
and outputs the sum of distances in all node neighbor lists for each cycle.
Optionally, a graph of total distances vs cycle number is produced.
"""

import sys
import argparse, math, unittest, random

class Node(object):
  '''Node class defines the behavior of a single node in this distributed system'''
  def __init__(self,identifier,N,k):
    self.ID = identifier
    self.k = k
    self.neighbors = []
    # bits in the address space:
    self.bits = int(math.log(N,2))
    self.mask = 1 << (self.bits-1)
    # current neighbor to gossip with
    self.currNeighbor = 0

  def FindDistance(self,nodeID):
    '''Calculate my distance from a given nodeID'''
    a = self.ID
    b = nodeID
    alevel = self.bits
    blevel = self.bits
    commonPrefix = 0
    # Find a level
    while (self.mask & a) == 0:
      a <<= 1
      alevel -= 1
    # Find b level
    while (self.mask & b) ==0:
      b <<= 1
      blevel -= 1
    # Find commonprefix
    length = min(alevel, blevel)
    while (self.mask & ~(a ^ b)) != 0 and length>0:
      a <<= 1
      b <<= 1
      commonPrefix += 1
      length -= 1
    return alevel + blevel - (2 * commonPrefix)


  def ChooseNewNeighbors(self,newNodesList,logging=False):
    '''Given a list of new nodes and my list of neighbors, choose the closest k
    nodes to become my new neighbor list.'''
    # Join my nodes with new nodes
    allNodes = self.neighbors + newNodesList
    if logging:
      neighborL = []
      for node in allNodes:
        neighborL.append(node.ID)
      print self.ID, 'allnodes', neighborL
    # Remove duplicates
    allNodes = list(set(allNodes))
    if logging:
      neighborL = []
      for node in allNodes:
        neighborL.append(node.ID)
      print self.ID, 'allnodes w/o duplicates', neighborL
    # Get all node distances
    distances = []
    for node in allNodes:
      distances.append(self.FindDistance(node.ID))
    if logging:
      print self.ID, 'distances', distances
    # Zip the lists together and sort by distances
    nodesByDist = zip(distances,allNodes)
    if logging:
      print nodesByDist
    nodesByDist.sort()
    if logging:
      print nodesByDist
    distances,allNodes = zip(*nodesByDist)
    if logging:
      neighborL = []
      for node in allNodes:
        neighborL.append(node.ID)
      print self.ID, 'allnodes sorted', neighborL
    # Take the k closest, except not the closest because that's me
    if allNodes[0] == self:
      self.neighbors = list(allNodes[1:self.k+1])
    else:
      self.neighbors = list(allNodes[0:self.k])

  def Gossip(self, logging=False):
    '''Send a neighbor my ID and my neighbors, receive there neighbor list'''
    # Call ChooseNewNeighbors on my neighbor with my node list
    try:
      self.neighbors[self.currNeighbor].ChooseNewNeighbors(self.neighbors)
    except:
      print type(self.neighbors)
    # Get new nodes from neighbor and run ChooseNewNeighbors method with it
    try:
      self.ChooseNewNeighbors(self.neighbors[self.currNeighbor].neighbors, logging)
    except:
      print type(self.neighbors[self.currNeighbor].neighbors)
    # increment currNeighbor to gossip with someone new next time
    self.currNeighbor += 1
    if self.currNeighbor == self.k:
      self.currNeighbor = 0

  def SumCurrentDistances(self):
    '''Report on my current neighbors' distances from me'''
    totalDistance = 0
    for node in self.neighbors:
      totalDistance += self.FindDistance(node.ID)
    return totalDistance


class Simulator(object):
  '''The Simulator class will handle simulating a distributed system of nodes'''
  def __init__(self,cycles,N,k,logging=False):
    '''This method will represent the network initialization phase'''
    self.cycles = cycles
    self.n = N
    self.k = k
    self.nodes = []
    if logging:
      print 'Runnning a simulation of %d cycles with size %d and neighbor \
list length %d.' % (self.cycles, self.n, self.k)
    # Initialize the nodes in the system
    for i in range(1,N):
      self.nodes.append(Node(i,N,k))
    # Give them random neighbors
# !!!!!BUG: Will give the same random neighbor twice!!!
    nodeID = 1
    for node in self.nodes:
      for i in range(k):
        # Make sure neighbor isn't node
        index = random.randint(1,N-1)
        while index==nodeID:
          index = random.randint(1,N-1)
        node.neighbors.append(self.nodes[index-1])
      nodeID += 1

  def Simulate(self):
    '''This method will carry out the network evolution phase for self.cycles'''
    # Data will be stored as a list of cycle,sumOfDistance tuples
    simData = []
    for cycle in range(self.cycles):
      # Gossip with a node
      for node in self.nodes:
        node.Gossip()
      # Get node distance now that everyone is settled
      totalDistances = 0
      for node in self.nodes:
        totalDistances += node.SumCurrentDistances()
      simData.append((cycle,totalDistances))
    return simData


class TestNodeClass(unittest.TestCase): #{{{
  '''Unit Tests used during development to incrementally test features'''
  def test_init(self):
    smallNode = Node(identifier=8,N=16,k=3)
    self.assertEqual(smallNode.ID, 8)
    self.assertEqual(smallNode.k, 3)
    self.assertEqual(smallNode.neighbors, [])
    self.assertEqual(smallNode.bits, 4)
    self.assertEqual(smallNode.mask, 8)

  def test_FindDistance(self):
    smallNode = Node(8,16,3)
    self.assertEqual(smallNode.FindDistance(5),3)
    self.assertEqual(smallNode.FindDistance(15),6)
    self.assertEqual(smallNode.FindDistance(1),3)
    smallNode = Node(5,16,3)
    self.assertEqual(smallNode.FindDistance(5),0)
    self.assertEqual(smallNode.FindDistance(15),5)
    self.assertEqual(smallNode.FindDistance(1),2)

  def test_SumCurrDistances(self):
    node4 = Node(4,16,3)
    node1 = Node(1,16,3)
    node5 = Node(5,16,3)
    node2 = Node(2,16,3)
    node2.neighbors = [node4, node1, node5]
    self.assertEqual(node2.SumCurrentDistances(), 3)
    node15 = Node(15,16,3)
    node2.neighbors = [node15, node1, node5]
    self.assertEqual(node2.SumCurrentDistances(), 6)

  def test_ChooseNewNeighbors(self):
    node1 = Node(1,16,3)
    node2 = Node(2,16,3)
    node3 = Node(3,16,3)
    node4 = Node(4,16,3)
    node5 = Node(5,16,3)
    node6 = Node(6,16,3)
    node7 = Node(7,16,3)
    node8 = Node(8,16,3)
    node2.neighbors = [node8, node1, node7]
    node2.ChooseNewNeighbors([node3,node4,node2])
    for node in node2.neighbors:
      self.assertLess(node.ID, 5)

  def test_GossipRotateNeighbors(self):
    node1 = Node(1,16,3)
    node2 = Node(2,16,3)
    node3 = Node(3,16,3)
    node4 = Node(4,16,3)
    node8 = Node(8,16,3)
    node1.neighbors = [node2, node3, node8]
    node2.neighbors = [node3, node4, node1]
    node3.neighbors = [node4, node1, node8]
    node4.neighbors = [node1, node2, node3]
    self.assertEqual(node1.currNeighbor, 0)
    self.assertEqual(node1.neighbors, [node2, node3, node8])
    node1.Gossip()
    self.assertEqual(node1.currNeighbor, 1)
    self.assertEqual(node1.neighbors, [node2, node3, node4])
    node1.Gossip()
    self.assertEqual(node1.currNeighbor, 2)
    self.assertEqual(node1.neighbors, [node2, node3, node4])
    node1.Gossip()
    self.assertEqual(node1.currNeighbor, 0)
    self.assertEqual(node1.neighbors, [node2, node3, node4])

  def test_GossipChooseNewNeighbors(self):
    # Still an error in taking neighbors with same distances:
    # they get overwritten when you make the dictionary
    node1 = Node(1,16,3)
    node2 = Node(2,16,3)
    node3 = Node(3,16,3)
    node4 = Node(4,16,3)
    node5 = Node(5,16,3)
    node6 = Node(6,16,3)
    node7 = Node(7,16,3)
    node8 = Node(8,16,3)
    node1.neighbors = [node8, node6, node7]
    node2.neighbors = [node8, node6, node7]
    node8.neighbors = [node2, node3, node1]
    node1.Gossip(logging=True) # With node 8 persumably
    neighborList = []
    for node in node1.neighbors:
      neighborList.append(node.ID)
    print node1.ID, neighborList
    self.assertEqual(node1.neighbors, [node2, node3, node7])
    # This is just convention, really nodes 4, 5, 6, and 7 are all equidistant
    # from 1
    self.assertEqual(node8.neighbors, [node2, node1, node3])
    node1.currNeighbor = 0
    node1.Gossip() # With node 2 persumably
#     self.assertEqual(node1.neighbors, [node2, node3, node4]) 
#     self.assertEqual(node2.neighbors, [node4, node3, node8]) 
#}}}


class TestSimulatorClass(unittest.TestCase): #{{{
  '''Unit Tests used during development to incrementally test features'''
  def test_init(self):
    smallSim = Simulator(15,16,3,logging=False)
    self.assertEqual(smallSim.cycles, 15)
    self.assertEqual(smallSim.n, 16)
    self.assertEqual(smallSim.k, 3)
    self.assertEqual(len(smallSim.nodes), 15)
    for node in smallSim.nodes:
      self.assertEqual(len(node.neighbors), 3)
      for neighbor in node.neighbors:
        self.assertNotEqual(neighbor,node)
    largeSim = Simulator(15,1024,4,logging=False)
    self.assertEqual(largeSim.cycles, 15)
    self.assertEqual(largeSim.n, 1024)
    self.assertEqual(largeSim.k, 4)
    self.assertEqual(len(largeSim.nodes), 1023)
    for node in largeSim.nodes:
      self.assertEqual(len(node.neighbors), 4)
      for neighbor in node.neighbors:
        self.assertNotEqual(neighbor,node)

  def test_smallSimulation(self):
    smallSim = Simulator(5,8,2,logging=False)
    simData = smallSim.Simulate()
    self.assertEqual(len(simData),5)
    cycles,distances = zip(*simData)
    self.assertEqual(list(cycles),range(5))

#   The following is just useful to peek at the nodes, maybe one day
#   I'll figure out an actual unit test for this.
#   def test_nodeBehaviorPostSim(self): 
#     smallSim = Simulator(10,8,3,logging=False) 
#     for node in smallSim.nodes: 
#       neighborIDs = [] 
#       for n in node.neighbors: 
#         neighborIDs.append(n.ID) 
#       print node.ID, neighborIDs 
#     smallSim.Simulate() 
#     for node in smallSim.nodes: 
#       neighborIDs = [] 
#       for n in node.neighbors: 
#         neighborIDs.append(n.ID) 
#       print node.ID, neighborIDs 
# #}}} 


if __name__ == "__main__":
# argparse object to set up the arguments for the program
# Run the program with the -h or --help flags to read the detailed usage
  parser = argparse.ArgumentParser(description='''Simulate a simple, gossip \
      based construction algorithm and report how node neighbor distances \
      develop over time.''')
  parser.add_argument('-N', action='store', type=int, help='total number of \
      nodes in the network (default=1024)')
  parser.add_argument('-k', action='store', type=int, help='number of \
      neighbors each node maintains (default=3)')
  parser.add_argument('--cycles','-c', action='store', type=int, \
      help='number of cycles to simulte (default=15)')
  parser.add_argument('--graph','-g', action='store_true', default=False, \
      help='produce a graph named simData.png of neighbor distances vs.\
      cycle (Default prints data)')
  parser.add_argument('--test', action='store_true', default=False, \
      help='run the unit tests')
  parser.set_defaults(cycles=50,N=1024,k=3)

# Get the command line arguments
  cli = parser.parse_args()
  if cli.test:
    sys.argv.remove('--test')
    unittest.main()
  else:
    sim = Simulator(cli.cycles,cli.N,cli.k,logging=True)
    simData = sim.Simulate()
    if cli.graph:
      # Graph
      import pylab
      x,y = zip(*simData)
      pylab.plot(x,y)
      pylab.xlabel('Simulation Cycle')
      pylab.ylabel('Total Neighbor List Distances')
      pylab.savefig('simData')
      pylab.show()
    else:
      # Dump results to the Screen
      for line in simData:
        print '%d\t%d' % line
