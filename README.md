# traffic_simulation
## Atakan Kara Farrin Marouf Sofian
### project 2

In this project we created a traffic simulation. The program gets command line arguments as follows: 

  # ## timeLog(arg1) simulationTime(arg2) probability(arg3) seed(arg4) snapshot(arg5)
 compile main.c as follows: gcc -pthread main.c 
 run main.c as follows: ./a.out -s simulationTime(arg2) probability(arg3) seed(arg4) snapshot(arg5)
 
Where timeLog is -s. There are 4 condition variables per line and 2 additional ones one for determining whether a car is crossing/finished crossing and the other one is used to determine the start and finish of police actions. Based on the project description, if cars are crossing the intersection, the other cars will wait and they can only cross the intersection if they are signalled by the police. Initially, the mutex and the conditional variables are initialized and the mutex lock is contained by the main. 4 threads are created each representing the lanes. The main contains a while loop where which runs until simulation time. The basic logic of the program is as follows, in the begining the lock is contained by the main and then main signals the police and the police, based on the number of cars in the lanes, decides which lane should be signalled. Then the signalled lane signlas the main again when it is done. 

#### The logics, is that is cars are crossing a lane, other lanes wait. If in a lane a car is waiting more that 20 seconds, that lane is prioritized. if there are more than 5 cars in a lane, that lane has the next priority.


#### we implemented our own enqueue and dequeue methods in the utils file. 
