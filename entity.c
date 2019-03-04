/******************************************************************************/
/*                                                                            */
/* ENTITY IMPLEMENTATIONS                                                     */
/*                                                                            */
/******************************************************************************/

// Student names:
// Student computing IDs:
//
//
// This file contains the actual code for the functions that will implement the
// reliable transport protocols enabling entity "A" to reliably send information
// to entity "B".
//
// This is where you should write your code, and you should submit a modified
// version of this file.
//
// Notes:
// - One way network delay averages five time units (longer if there are other
//   messages in the channel for GBN), but can be larger.
// - Packets can be corrupted (either the header or the data portion) or lost,
//   according to user-defined probabilities entered as command line arguments.
// - Packets will be delivered in the order in which they were sent (although
//   some can be lost).
// - You may have global state in this file, BUT THAT GLOBAL STATE MUST NOT BE
//   SHARED BETWEEN THE TWO ENTITIES' FUNCTIONS. "A" and "B" are simulating two
//   entities connected by a network, and as such they cannot access each
//   other's variables and global state. Entity "A" can access its own state,
//   and entity "B" can access its own state, but anything shared between the
//   two must be passed in a `pkt` across the simulated network. Violating this
//   requirement will result in a very low score for this project (or a 0).
//
// To run this project you should be able to compile it with something like:
//
//     $ gcc entity.c simulator.c -o myproject
//
// and then run it like:
//
//     $ ./myproject 0.0 0.0 10 500 3 test1.txt
//
// Of course, that will cause the channel to be perfect, so you should test
// with a less ideal channel, and you should vary the random seed. However, for
// testing it can be helpful to keep the seed constant.
//
// The simulator will write the received data on entity "B" to a file called
// `output.dat`.

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "simulator.h"
#include "entity.h"

int base;
int nextSeqNum;

/**** A ENTITY ****/
int calcCheckSum(struct pkt packet) {

<<<<<<< HEAD
=======
void A_init() {

>>>>>>> f26b2fb06c9a964ae5dc6292ff1203947559cacf
}
unsigned char checksum(struct pkt packet) {

}
void A_init() {
    base = 0;
    nextSeqNum = 1;
}
//Called by the simulator with data passed from the application layer to your transport layer
//containing data that should be sent to B. It is the job of your protocol to ensure that
//the data in such a message is delivered in-order
void A_output(struct msg message) {
  //Convert message to packet
  pkt sendPacket;

  sendPacket.seqnum = base;
  sendPacket.acknum = base;
  sendPacket.length = message.length;
  for(int i = 0; i < sendPacket.length; i++) {
    sendPacket.payload[i] = message.data[i];
  }
  sendPacket.checksum = calcCheckSum(sendPacket);

  //Send packet to layer 3
  tolayer3_A(sendPacket);

  //Send packet to layer 3
  starttimer_A(1);

}
//Called whenever a packet is sent from B to A. Packet may be corrupted
void A_input(struct pkt packet) {
}
//Routine to control retransmission of packets
void A_timerinterrupt() {
}


/**** B ENTITY ****/

void B_init() {
}
//Packet received from A possibly corrupted
void B_input(struct pkt packet) {
  //check if packet is corrupted or ACK is wrong
  //if so send back the packet and retart the timer
  //tolayer3
  //check if timed out
  //if so send back the packet and restart the timers
  //tolayer3
  //Otherwise send to next state and stop the timer
  //tolayer5

  //update sequence number?

}

void B_timerinterrupt() {
}
