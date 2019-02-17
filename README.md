# project3RT
In this assignment, you will be writing the sending and receiving transport-level code for implementing a simple reliable data transfer protocol. There are two versions of this project, the Alternating-Bit version and the Go-Back-N version. This project should be fun since your implementation will differ very little from what would be required in a real-world situation.  Since you probably don't have standalone machines (with an OS that you can modify), your code will have to execute in a simulated hardware/software environment. However, the programming interface provided to your routines, i.e., the code that would call your entities from above and from below is very close to what is done in an actual UNIX environment. (Indeed, the software interfaces described in this programming assignment are much more realistic that the infinite loop senders and receivers that many texts describe).  The goal of your protocol is successfully and reliably transfer data from a sending entity (entity "A") to a receiver (entity "B"). In this project only unidirectional data (from A to B) is required, however, B will have to send acknowledgments (or negative acknowledgments) back to A. Your task is to implement the functions specified in entity.c for the alternating bit protocol and the go-back-N protocol.

Simulation Environment

To provide a realistic network this project includes a network simulator. The network is capable of corrupting and losing packets, but it will not reorder packets. The network simulator is configurable can adjust to simulate a very reliable network or one that is very lossy or likely to corrupt packets.

The following configuration options exist:

    Packet Loss Probability: A number between 0.0 (no loss) and 1.0 (complete) loss that indicates how likely the network is to lose a packet. A value of 0.1 would mean that one in ten packets (on average) are lost.
    Packet Corruption Probability: A number between 0.0 (no corruption) and 1.0 (every packet is corrupted) that indicates how likely the network is to corrupt a packet. Note that any field (in the header or data) of a packet may be corrupted.
    Packet Interval Rate: This is the average amount of time between messages sent on the "A" entity side. The application layer (layer 5) in the simulator will pass messages to your code (layer 4) at this rate (on average). This is specified in simulator time units. Note that the smaller the value you choose, the faster packets will be be arriving to your sender. A value of 10 is a reasonable starting point, but you should experiment with other values.
    Random Number Generator Seed: This is the integer value used to seed the random number generator used by the simulator. You should test with different values, but for debugging it is helpful to keep this constant.
    Debug Level: How much debugging output you want from the simulator. Valid values are 0, 1, 2, or 3.

The simulator will attempt to transfer the contents of a file from entity "A" to entity "B" by passing chunks of the file (in order) to the transport layer on entity "A". The name of the file to transfer is specified as a command line argument. The simulator will write the data received at entity "B" to a file called output.dat.

The simulator also includes functionality for a single timer on each entity. This timer can be used to trigger an event a specified number of time units in the future. Only one timer can be active at each entity at a time.
