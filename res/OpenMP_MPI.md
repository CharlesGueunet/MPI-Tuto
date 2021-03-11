Formation Hybrid programming
----------------------------

_Intro_


Moorse's law Number of transistor double every two years

-> Limitations:
Dissipated power = frea ^ 3
+ Limited by cm ^2 cooling
+ Energy cost
=> frequency is no longer increased
if transistor double : number of core incr. + new function added + new cache
(One core -> several threads)

-> The Memory Wall:
Memory not increase as quickly than proc
Latencies decrease slowly
Number of core per memory increase.
=> Processor waste + difficult to maximize exploit
Partial solution : More cache (=> More parallelization + Vectorisation)

(Today's phone = 2000 average supercalculator)

Machine architecture ++ complex : more and more layer
Non Uniform Memory Access (access memory through other core)

Amdahl
99.99 % parallilizable
speedup 1 000 cores : 909, 10 000 cores 5000 ! (Ideal)

Gustafson-Barsis is more optimistic and realistic

-> Programming
use many slow core
take care of memory
hight level of parallelism
I/O limit

Recode
Understand hardware
Cannot have your own code


-> Evolution

MPI is becoming a standard and evolve
MPI-OpenMP hybrid approach more used
GPU programming more used
Other hybrid (MPI+GPU) are tested
Parallel programming language are appearing

_OpenMP Parallel_

For shared memory.
  Create & manage threads
  Work shared between this threads
  Synchronisation
  Data sharing

  Scalability physically limited by size of shared memory node
  Limited by cumulated memory bandwidth inside an SMP node
  Pay attention to different layer of memory
  Cost linked to the machine (NUMA, false sharing...)
  Binding threads on core is the responsibility of the system
  Don't manage data locality
  Limited by Amdahl

-> Fine Grain
            Simplicity of implementation
            Incremental approach
            Lot of clause, even for load balancing

            Additional cost can be heavy
            Some algorithms ask for manual synchro
            limited scalability

-> Coarse Grain
            For low granularity: Additional cost are much less
            Very good scalability
            best perf on smp nodes

            intrusive in the code (separate sequential version)
            Incremental approach impossible
            manual synchronisationm, work share and load balancing
            as complex as mpi

Only coarse grain with consecutive iteration are really scalable and performant

-> Low level synch

Flush update memory hierarchy of a thread associated with the shared variable in argument
: Copy on shared memory or invalidated cache

Flush before read. Flush after write.
=> All shared used variables.

Pay attention to false cache values


-> Deal with double dependencies
    Hyperplan / software pipeline

    Hyperplan : diagonal by diagonal => Acces non contiguois elmt (bad perf)
    Pipelining : // and synchronize mannually



_MPI Parallel_

-> Provide
Exec environement
Derived datatype
Communicator / topology (point to Point, Collective , one sided)
Dynamic process management (nb core ~)
Parallel I/O
Profiling interface
MPI is in boost

-> Limitations:
Amdahl's law
Additional cost
Cost can increase with number of core (Certain communication)
No distinction between memory shared and distributed (incoming)
Few means provided in the standard match the hardware (in the standard)
You take care of load balancing (thread per core)

Remote Memory Accesses = RMA

Point to Point mode
Standard, Synchronous, Buffered, -Ready-
Ready => implies we assume receiver is already there.

-> Summary
Blocking : memory for call can be used immediately after the call exits
Immediate : fast return to cover communication, need MPI\_WAIT before (re)use memory.

Synchronous block communication before the two process are ready to Communicate
Buffered can involve an intermediate memory space

Standard means the implementation choose for you (often best)

-> Details
Synchronous implies
            header sent before the message
            few recources (no buffer)
            rapid if receiver ready

            waiting time if receiver not ready
            risk of deadlock

Buffered implies  : lot of work to do (few used)
            no need to wait the receiver
            no deadlock

            manage buffer manually (risk of waste if too bigm crash if too small)
            more resources used
            slower than synchronous due to copy time
            no guarantee of a good reception
            hidden buffers managed by MPI

Standard : buffer for small (eager), synchronous for big
            often the more efficient
            the most portable

            little control over the mode (take care of it !)
            risk of deadlock


-> Communications

Collective if possible / needed (Optimization)
Collective communication not include a barrier
Collective communication don't interfer with point to point

3 type of comm.
Barrier : avoid it
Data gathering (Bcast, scatter ...)
Reduction (Reduce, Reduce\_Scatter, scan...)

May require to create sub-communicator

Barrier don't means everyone return at the same time.

RMA : Read or write variable in distant memory without involving the distant proc.

-> Overlap

Network card directly read memory (Direct Mem. Access)
parallelize communication : non blocking call
            Hide communication
            Avoid deadlock

            Additional cost
            Add complexity
            Less efficient on machine that don't tolerate it

-> Derived Datatypes

Represent data structure of any degree of complexity
Important level of abstraction can be reached
Can be used in all MPI communications and even I/O

Datatype : Contigous, vector, indexed, struct
Be careful with the performances
            Readability
            high level of abstraction
            Non Contigous / Heterogeneous datatype possible
            message grouping
            usable in I/O

            Difficult to implement
            Heterogeneous can be hard to write
            High level of abstraction => less masterisation of is done
            Often lower perf
To avoid them, do not think about separate messages !
Use an intermediate structure (manual data management)
            often most efficient
            involve additionnal memory to memory copies
            use more memory resources

Or use MPI\_Pack / Unpack subroutines
            same disaventage than manual
            Not so efficient

-> Load Balancing

Cause of imbalance :
  Poor data partition
  AMR
  Different number of iteration
  External sources (Os, non dedicated resources ...)
Tips for re-balancing
  Dynamic balancing with exange of cells(use Hilbert space-filling curve)
  Master-slave approach (Work stealing ?)
  Partitioning library (PT-SCOTCHm ParMETIS...)
  Many sub-domains per proc.
  MPI-OpenMP hybrid approach

Process Mapping
  MPI_COMM_WORLD communicator : the global one
  rank : address inside a communicator (fix)
  process often fixed on a core (affinity / binding)

Mapping correspond to the relation between rank of proc and position in machine
Latency increase with the distance, and Network & Memory contention increases if message cross several links

Software adaptation. MPI Dims create, Cart create : indicate topology to MPI
Allow to communicate with close neighbors and divide the mesh to best match the machine

The thorus is a great way to organise if it correspond to the network mapping


_Hybrid Parallel_

When mixing technologies. Here MPI-OpenMP
            Reduce MPI Message
            More adequat to modern architecture (NUMA)
            Optimization of the memory consumption / reduce footprint
            Go beyon certain algorithms limitations (linked to dimension)
            Less memory use for the algorithm (less ghost cell) and for MPI library
            Fewer simultaneously in I/O
            Fewer file to manage if each process write its own file
            Cool if several level of granularity

            Ask for higher level of expertise
            Necesserity to have both good MPI-OpenMP perf.
            Extra cost

-> Appli that can be improved :
            Code having limited MPI scalability (All to All comm ...)
            Code limited by the scalability of their algorithm
            Code requiring load balancing
            Code limited by memory size
            Massively parallel appli
            Problems of fine grain or mix fine and coarse grain

Don t forget to use MPI\_Init\_thread => Use it to check level support
Requiring a higher level can impact performances
Rank only identify the mpi process (like when you phone home)

On thread per communicator can make a collective call at a given moment

Memory saving
  Shared memory avoid duplication and ghost cells.
  Limit the use of MPI Buffer

The choice of 1 MPI process per node and as many thread that a core naturally have,
it is not the most efficient way. Mainly because of network
So it depends on the architecture of the machine.

It seems funneled mode have lighter header

L1 cache not shared, L2 often
Network comm can be faster than inter node o0 (does mpi use 127.0.0.1 ?)

_Performances_

Hybrid allow a better scalability
The scalability observed depend on the size of the input and the architecture
The only way to be sure the code is better (perf / scalability) is to try it (strange effects)
Construction of Derived datatype can improve communication with a better cache usage
Derived datatype on hybrid can decrease performances (if only made by one thread) and may
 have to be replaced by buffer/array

On very scalable application, hybrid may not be a huge improvement


_Tools_

Scalasca : 
 Graphical tool for profiling (easy to use and powerfull)
Tracing mode compare different processes and threads (MPI Funneled)
=> Automatic analysis of what append to determine what impact performances

Compile with skin f90 / skin gcc
Execute with scan (-t) mpirun ...
visualize with square

Allow to see time used by OMP/ MPI, in each funct, in each comm, differents overloads....
=> Use fitler to avoid impact performances with analysis

TAU :
  Similar to Scalasca
  instrument loop separatly
  no Automatic analysis
  3D Visualisation
  More complexe

  TotalView :
  Proprietary and expansive


_Questions_

Architecture implies Consumption ?

Hybrid : CPU GPU / OpenMP MPI ?

Test MPI boucle local !
