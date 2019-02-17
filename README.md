# project3RT
In this assignment, you will be writing the sending and receiving transport-level code for implementing a simple reliable data transfer protocol. There are two versions of this project, the Alternating-Bit version and the Go-Back-N version. This project should be fun since your implementation will differ very little from what would be required in a real-world situation.  Since you probably don't have standalone machines (with an OS that you can modify), your code will have to execute in a simulated hardware/software environment. However, the programming interface provided to your routines, i.e., the code that would call your entities from above and from below is very close to what is done in an actual UNIX environment. (Indeed, the software interfaces described in this programming assignment are much more realistic that the infinite loop senders and receivers that many texts describe).  The goal of your protocol is successfully and reliably transfer data from a sending entity (entity "A") to a receiver (entity "B"). In this project only unidirectional data (from A to B) is required, however, B will have to send acknowledgments (or negative acknowledgments) back to A. Your task is to implement the functions specified in entity.c for the alternating bit protocol and the go-back-N protocol.

# Simulation Enviroment

To provide a realistic network this project includes a network simulator. The network is capable of corrupting and losing packets, but it will not reorder packets. The network simulator is configurable can adjust to simulate a very reliable network or one that is very lossy or likely to corrupt packets.

The following configuration options exist:

    Packet Loss Probability: A number between 0.0 (no loss) and 1.0 (complete) loss that indicates how likely the network is to lose a packet. A value of 0.1 would mean that one in ten packets (on average) are lost.
    Packet Corruption Probability: A number between 0.0 (no corruption) and 1.0 (every packet is corrupted) that indicates how likely the network is to corrupt a packet. Note that any field (in the header or data) of a packet may be corrupted.
    Packet Interval Rate: This is the average amount of time between messages sent on the "A" entity side. The application layer (layer 5) in the simulator will pass messages to your code (layer 4) at this rate (on average). This is specified in simulator time units. Note that the smaller the value you choose, the faster packets will be be arriving to your sender. A value of 10 is a reasonable starting point, but you should experiment with other values.
    Random Number Generator Seed: This is the integer value used to seed the random number generator used by the simulator. You should test with different values, but for debugging it is helpful to keep this constant.
    Debug Level: How much debugging output you want from the simulator. Valid values are 0, 1, 2, or 3.

The simulator will attempt to transfer the contents of a file from entity "A" to entity "B" by passing chunks of the file (in order) to the transport layer on entity "A". The name of the file to transfer is specified as a command line argument. The simulator will write the data received at entity "B" to a file called output.dat.

The simulator also includes functionality for a single timer on each entity. This timer can be used to trigger an event a specified number of time units in the future. Only one timer can be active at each entity at a time.

# Network Simulator Data Structures

There are two main data structures that the reliable transport protocols need to use and they are defined in simulator.h.

The unit of data passed between the upper layers and your protocols is a message, which is declared as:

struct msg {
  int length;
  char data[20];
};

This structure can contain up to 20 bytes of data, and the number of valid bytes is defined by the length value. (Length should never be greater than 20). The simulator will read in the specified input file and divide the contents of the input file into messages which are passed from layer 5 to the transport layer that you implement.

The unit of data passed between your routines and the network layer is the packet, which is declared as:

struct pkt {
  int seqnum;
  int acknum;
  int checksum;
  int length;
  char payload[20];
};

Your routines will fill in the payload and length fields from the message data passed down from layer 5. The other packet fields will be used by your protocols to ensure reliable delivery. You may not change these data structures, and your reliable protocols will have to use only these provided fields.
Network Simulator Provided Functions

The simulator provides some public functions which are defined in simulator.h:

    For entity "A", and these functions must ONLY be called by entity "A":
        void starttimer_A (float increment): Set a timer for some simulator time units in the future.
        void stoptimer_A (): Stop the entity "A" timer.
        void tolayer3_A (struct pkt packet): Pass a packet to the network.

    For entity "B", and these functions must ONLY be called by entity "B":
        void starttimer_B (float increment)
        void stoptimer_B ()
        void tolayer3_B (struct pkt packet)
        void tolayer5_B (struct msg message): Pass a message to the application layer on the receiving B entity.

More detail on these functions is in simulator.h.
Transport Layer Functions

The simulator expects the following functions to be correctly implemented to enable a reliable transport protocol. These functions are defined in entity.h and a template is provided in entity.c.

    A_init(): This routine will be called once, before any other A functions are called. It can be used to do any required initialization.
    A_output(struct msg message): This is called by the simulator with data passed from the application layer to your transport layer containing data that should be sent to B. It is the job of your protocol to ensure that the data in such a message is delivered in-order, and correctly, to the receiving side upper layer.
    A_input(struct pkt packet): This will be called whenever a packet sent from B arrives at A. packet is the (possibly corrupted) packet sent from B.
    A_timerinterrupt(): This routine will be called when A's timer expires (thus generating a timer interrupt). You'll probably want to use this routine to control the retransmission of packets.
    B_init(): This routine will be called once, before any other B routines are called. It can be used to do any required initialization.
    B_input(struct pkt packet): This routine will be called whenever a packet sent from A arrives at B. packet is the (possibly corrupted) packet sent from A.
    B_timerinterrupt(): This routine will be called when B's timer expires.

#Compiling and Running the Simulator

You should be able to compile the simulator with a simple gcc as follows:

$ gcc entity.c simulator.c -o myproject

Then running it like so:

$ ./myproject 0.0 0.0 10 500 3 test1.txt

The generic command line arguments are:

$ ./myproject <loss prob> <corrupt prob> <pkt interval> <seed> <debug> <input file>

#Part 1: The Alternating-Bit-Protocol

You are to implement A_output(), A_input(), A_timerinterrupt(), A_init(), B_input(), B_init(), and B_timerinterrupt() which together will implement a stop-and-wait (i.e., the alternating bit protocol, which we referred to as rdt3.0 in the text) unidirectional transfer of data from entity A to entity B. Your protocol should use both ACK and NACK messages.

You should choose a very large value for the average time between messages from A's layer 5, so that your sender is never called while it still has an outstanding, unacknowledged message it is trying to send to the receiver. I'd suggest you choose a value of 1000. You should also perform a check in your sender to make sure that when A_output() is called, there is no message currently in transit. If there is, you can simply ignore (drop) the data being passed to the A_output() routine.

You should put your implementations in the attached entity.c file. While completing this portion will be very helpful for completing the Go-Back-N version, you do not need to and should not submit this version.

Make sure you read the "helpful hints" for this project following the description of the Go-Back-N version of this project.
Part 2: Go-Back-N

You are to implement A_output(), A_input(), A_timerinterrupt(), A_init(), B_input(), B_init(), and B_timerinterrupt() which together will implement a Go-Back-N unidirectional transfer of data from entity A to entity B, with a window size of 8. Your protocol should use both ACK and NACK messages.

You should work on this portion after completing part 1. There are some extra considerations for this version which do not apply to the alternating-bit version:

    Your A_output() routine will now sometimes be called when there are outstanding, unacknowledged messages in the network. This means you will need to buffer multiple messages in your sender. Also, you'll need buffering in your sender because of the nature of Go-Back-N: sometimes your sender will be called but it won't be able to send the new message because the new message falls outside of the current window.

    Rather than have you worry about buffering an arbitrary number of messages, it will be OK for you to have some finite, maximum number of buffers available at your sender, but you should be able to buffer at least 1000 messages. Should all 1000 buffers be in use at one point it is ok to give up and exit. (In the "real-world", of course, one would have to come up with a more elegant solution to the finite buffer problem!)

    Remember that you've only got one timer, and may have many outstanding, unacknowledged packets in the medium, so you'll have to think a bit about how to use this single timer.

#Testing

You should run the simulator with various settings, and then use a diff tool to compare the input file to output.dat. You should also vary the random seed used.

For example you may want to test your working version with the following settings:

    packet loss = 0.1
    corruption = 0.3
    interval = 10

#Submission

Your submission should include:

    Your completed entity.c file. You do not need to include the other .c and .h files, and if you do they will not be used for grading.

    An example test file that you used during testing.

    A file with output from running your transport layer when transferring the test case from item number 2. This file should include the command line you used to run the test with the following settings: packet loss=0.14, corruption=0.25, interval=10. Your output file should clearly indicate two features: 1) Where entity "A" sent a packet before a previous packet was successfully acknowledged; and 2) Where a packet was resent due to a timeout. You can use printf()s in your code to identify these occurrences.

#Helpful Hints and the Like

    Checksumming. You can use whatever approach for checksumming you want. Remember that the sequence number, ack field, and length fields can also be corrupted. We would suggest a TCP-like checksum, which consists of the sum of the (integer) sequence and ack field values, added to a character-by-character sum of the valid payload field of the packet (i.e., treat each character as if it were an 8 bit integer and just add them together).
    Note that any shared "state" among your routines needs to be in the form of global variables. Note also that any information that your procedures need to save from one invocation to the next must also be a global (or static) variable. For example, your routines will need to keep a copy of a packet for possible retransmission. It would probably be a good idea for such a data structure to be a global variable in your code. Note, however, that if one of your global variables is used by your sender side, that variable MUST NOT be accessed by the receiving side entity, since in real life, communicating entities connected only by a communication channel can not share global variables. Violating this rule will result in a low score on this project (or a 0).
    Start simple. Set the probabilities of loss and corruption to zero and test out your routines. Better yet, design and implement your procedures for the case of no loss and no corruption, and get them working first. Then handle the case of one of these probabilities being non-zero, and then finally both being non-zero.
    Debugging. We recommend that you set the tracing level to 2 or 3 and put many printf()s in your code while your debugging your procedures.
    Be sure that you pass a float value to the starttimer() functions. For example, node A can set a timer for 200 time units by calling starttimer_(200.0). The decimal point is important here: gcc will not give you a compiler error if you pass an int as the float parameter.
    Be sure to dereference pointers: gcc will not give you a compiler error if you pass a pointer to the tolayer3 function. Most likely, the receiver will get a packet with the wrong data.

