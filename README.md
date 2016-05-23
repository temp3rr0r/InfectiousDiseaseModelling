# Parallel N-Body Simulator#

Modeling the spread of infectious diseases with computer simulations is
an important and useful step in learning how populations could respond to
hypothetical treatments or predict pandemics.  The focus of this assignment
is  a  simplified  model  of  an  infectious  disease  spreading  over  a  population.
This assignment is inspired by the work of Eubank et al.  [1].  The individ-
uals  are  moving  around  in  a  random  fashion  over  a  graph  that  represents
spatial locations.  Spreading of the disease is possible whenever an infected
and a healthy individual share the same location.
Design  and  implement  a  parallel  algorithm  for  the  above  simulation.
Write code in C/C++ and use OpenMP for parallelization.

###Implementation
Three versions of the source code where created:

1. Serial Naive"  Undirected Acyclic Graph algorithm, utilizing map look-up tables, native C++ 11, using STD, STL and Boost Graph library[4].
2. Serial:   Undirected  Acyclic  Graph  algorithm,  utilizing  map  look-up tables, native C++ 11, using STD, STL and Boost Graph library. Some algorithmic optimizations were applied:
	- Breaking  an  inner for loop  that  was  comparing  if  a  susceptible individuals got infected individuals.
	- Skipping iteration loops for individuals that are already infected.
3. Parallel:  Undirected  Acyclic  Graph  algorithm,  utlizing  map  look-up tables,  native C++  11, using  STD,  STL, Boost  Graph library and OpenMP  version  4.1 (most  of  the  features).   Also  the  optimizations mentioned above were applied.

### Documentation
Documentation comparing the speed-up between the serial and parallel versions: 

###References
1. *Intel Developer zone - n-bodies: a parallel TBB solution: computing accelerations? or forces?* - https://software.intel.com/enus/blogs/2009/09/22/n-bodies-a-parallel-tbb-solution-computingaccelerations-or-forces
2. *Wikipedia: Fast Inverse Square Root* - https://en.wikipedia.org/wiki/Fastinversesquareroot
3. *Wikipedia: Barnes-Hut simulation* - https://en.wikipedia.org/wiki/BarnesHutsimulation
4. *Tutorial: Scalable Memory Allocator* - https://www.threadingbuildingblocks.org/tutorial-intel-tbb-scalablememory-allocator