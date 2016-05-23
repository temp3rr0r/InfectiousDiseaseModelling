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

1. Serial Naive:  Undirected Acyclic Graph algorithm, utilizing map look-up tables, native C++ 11, using STD, STL and *Boost Graph library*[4].
2. Serial:   Undirected  Acyclic  Graph  algorithm,  utilizing  map  look-up tables, native C++ 11, using STD, STL and Boost Graph library. Some algorithmic optimizations were applied:
	- Breaking  an  *inner for-loop*  that  was  comparing  if  a  susceptible individuals got infected individuals.
	- *Skipping iteration* loops for individuals that are already infected.
3. Parallel:  Undirected  Acyclic  Graph  algorithm,  utlizing  map  look-up tables,  native C++  11, using  STD,  STL, Boost  Graph library and *OpenMP*  version  4.1 (most  of  the  features).   Also  the  optimizations mentioned above were applied.

### Documentation
Documentation comparing the speed-up between the serial and parallel versions: https://onedrive.live.com/redir?resid=F3C315EB7F683B03!20269&authkey=!AG_HeLbTwupstY0&ithint=file%2cpdf

###References
1. Eubank et al., Modelling disease outbreaks in realistic ur- ban social networks, Nature 429, 180-184 (13 May 2004) - http://www.nature.com/nature/journal/v429/n6988/full/nature02541.html
2. 32 OpenMP Traps For C++ Developers - http://www.viva64.com/en/a/0054/
3. FIRSTPRIVATE Clause - https://computing.llnl.gov/tutorials/openMP/FIRSTPRIVATE
4. Exploring the Boost Graph Library - http://www.ibm.com/developerworks/aix/library/au-aix-boost-graph/
5. Learning the OpenMP framework with GCC - http://www.ibm.com/developerworks/aix/library/au-aix-openmp- framework/