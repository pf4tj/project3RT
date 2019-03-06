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
int nextSeqnum;
int lastSentAcknum;
unsigned char timeoutA;
unsigned char timeoutB;

/**** A ENTITY ****/
int calcCheckSum(struct pkt packet) {

    unsigned int sum = 0;
    unsigned short* word16;

    //Add up 16 bit pieces of header
    if(sizeof(packet.seqnum) > 2) {
      word16 = (unsigned short*)&packet.seqnum;
      sum += word16[0];
      sum += word16[1];
    }
    else {
      sum += packet.seqnum;
    }
    if(sizeof(packet.acknum) > 2) {
      word16 = (unsigned short*)&packet.acknum;
      sum += word16[0];
      sum += word16[1];
    }
    else {
      sum += packet.acknum;
    }
    if(sizeof(packet.length) > 2) {
      word16 = (unsigned short*)&packet.length;
      sum += word16[0];
      sum += word16[1];
    }
    else {
      sum += packet.length;
    }
    for(int i  = 0; i < packet.length; i++) {
      if(sizeof(packet.payload[i]) > 2) {
          word16 = (unsigned short*)&packet.payload[i];
          sum += word16[0];
          sum += word16[1];
      }
      else {
          sum += packet.payload[i];
      }
    }

    //Get ones complement result
    while(sum >> 16)
      sum = (sum & 0xFFFF)+(sum>>16);

    unsigned short checkSum = ~sum;

    return checkSum;

}

void A_init() {
    base = 0;
    nextSeqnum = 0;
    lastSentAcknum = 0;
    timeoutA = 0;
}
//Called by the simulator with data passed from the application layer to your transport layer
//containing data that should be sent to B. It is the job of your protocol to ensure that
//the data in such a message is delivered in-order
void A_output(struct msg message) {
  //Convert message to packet
  pkt sendPacket;

  sendPacket.seqnum = nextSeqnum;
  sendPacket.acknum = nextSeqnum;
  sendPacket.length = message.length;
  for(int i = 0; i < sendPacket.length; i++) {
    sendPacket.payload[i] = message.data[i];
  }
  sendPacket.checksum = calcCheckSum(sendPacket);

  //Send packet to layer 3
  tolayer3_A(sendPacket);

  //Send packet to layer 3
  starttimer_A(1);

  nextSeqnum++;
  lastSentAcknum = sendPacket.acknum;
}
//Called whenever a packet is sent from B to A. Packet may be corrupted
void A_input(struct pkt packet) {
  //Check if timeout occurs before response
  if(timeoutA) {
    timeoutA = 0;
    starttimer_A();
  }
  //Fix corruption of packet
  
  //Resend packet

}
//Routine to control retransmission of packets
void A_timerinterrupt() {
  timeoutA = 1;
}


/**** B ENTITY ****/

void B_init() {
}
//Packet received from A possibly corrupted
void B_input(struct pkt packet) {
  //check if packet is corrupted or ACK is wrong or timed out
  if(packet.checksum != calcCheckSum(packet) || lastSentAcknum != packet.acknum) {
      //if so send back the packet and retart the timer
      tolayer3_B(packet);
  }
  //Otherwise send to next state and stop the timer
  else {
    tolayer5_B(packet);
  }

  //update sequence number?

}

void B_timerinterrupt() {
}
