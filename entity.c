/******************************************************************************/
/*                                                                            */
/* ENTITY IMPLEMENTATIONS                                                     */
/*                                                                            */
/******************************************************************************/

// Student names: Andrew Powell, Pierce Faraone
// Student computing IDs: awp6rd, pf4tj
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
#include "stdbool.h"

/*
 - Any shared state among routines needs to be in the form of a global variable
 - Any information that procedures need to save from one invocation to the next
   must also be a global variable
 - Need to keep a copy of a packet for possible retransmission
 - If global variable is used by sender side it cannot be used by receiving side
 -
*/

int base;
int nextSeqnum;
int lastSentAcknum;
unsigned char timeoutA;
unsigned char timeoutB;
pkt lastSentPacket;
pkt lastRcvPacket;
int windowsize;
int cumulativeAck;
pkt *sentPackets; 
int expectedAcknum;
int expectedSeqNum;



static const bool masterDebug = false;
static const bool debugAinit = false;
static const bool debugBinit = false;


/**** A ENTITY ****/
int calcCheckSum(struct pkt packet) {

    unsigned int sum = 0;
    unsigned short* word16;
    unsigned char* word8;

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
      word8 = (unsigned char*)&packet.payload[i];
      for(int j = 0; j < sizeof(packet.payload[i]); j++) {
        sum += word8[j];
      }
    }

    //Get ones complement result
    /*
    while(sum >> 16)
      sum = (sum & 0xFFFF)+(sum>>16);
    */

    unsigned short checkSum = ~sum;

    return checkSum;

}
int base;
int nextSeqnum;
int lastSentAcknum;
unsigned char timeoutA;
unsigned char timeoutB;
pkt lastSentPacket;
pkt lastRcvPacket;
int windowSize;
int cumulativeAck;
pkt *sentPackets;
int expectedSeqnum_A;
int windowSize;

void A_init() {
    base = 0;
    nextSeqnum = 0;
    lastSentAcknum = 0;
    windowSize = 8;
    sentPackets[windowSize];
    // size_t len = (sizeof(sentPackets)/sizeof(sentPackets[])
    expectedSeqnum_A = 0;
    windowSize = 8;
    sentPackets = malloc(windowSize*sizeof(struct pkt));
}
//Called by the simulator with data passed from the application layer to your transport layer
//containing data that should be sent to B. It is the job of your protocol to ensure that
//the data in such a message is delivered in-order
void A_output(struct msg message) {

  if(nextSeqnum < base + windowsize) {
    //Convert message to packet
    pkt sendPacket;

    //printf("Converting message to packet\n");
    sendPacket.seqnum = nextSeqnum;
    sendPacket.acknum = nextSeqnum;
    sendPacket.length = message.length;
    //printf("Packet length %d\n", sendPacket.length);
    memcpy(sendPacket.payload, message.data, sendPacket.length);
    sendPacket.checksum = calcCheckSum(sendPacket);

    sentPackets[nextSeqnum] = sendPacket;
    //Send packet to layer 3
    //printf("Sending packet to layer 3\n");
    tolayer3_A(sentPackets[nextSeqnum]);

    if(base == nextSeqnum) {
      //printf("Starting timer\n");
      starttimer_A(1.0);
    }
    nextSeqnum++;
  }
  else {
    
    //Refuse data
  }

}

//Called whenever a packet is sent from B to A. Packet may be corrupted
void A_input(struct pkt packet) {
  pkt rcvPacket;
  unsigned char corrupted = 0;
  for(int i = 0; i < sentPackets.size(); ++i) {
    if(sentPackets[i].seqnum == packet.seqnum && sentPackets[i].checksum == packet.checksum) {
      rcvPacket = sentPackets[i];
    }
    corrupted++;
  }
  //Not corrupted
  if(corrupted < sentPackets - 1) {
    base = rcvPacket.acknum + 1;
  if(packet.seqnum == expectedSeqnum && packet.checksum == calcCheckSum(packet)) {
    //Not corrupted
    base = packet.acknum + 1;
    if(base == nextSeqnum)
      stoptimer_A();
    else
      starttimer_A(1.0);
  }
  else {
    
  }

}
}

//Routine to control retransmission of packets
void A_timerinterrupt(){
  starttimer_A(1.0);
  for(int i = base; i < nextSeqnum; i++) {
    tolayer3_A(sendPacket[i]);
  }
}


/**** B ENTITY ****/
void B_init() {
  expectedAcknum_B = 0;
  expectedSeqnum_B = 0;
  cumulativeAck = 0;
}
//Packet received from A possibly corrupted
void B_input(struct pkt packet) {

  pkt sendPacket;
  msg appMsg;
  if(/*packet isn't corrupt*/ && packet.seqnum == expectedSeqnum) {
    appMsg.length = packet.length;
    for(int i = 0; i < appMsg.length; ++i) {
      appMsg.data[i] = packet.payload[i];
    }
    memcpy(packet.payload, appMsg.data,);
    memcpy(appMsg.data, packet.payload, appMsg.length);
    tolayer5_B(appMsg);

    sendPacket.seqnum = expectedSeqNum;
    sendPacket.acknum = packet.acknum;
    sendPacket.checksum = packet.checksum;
    sendPacket.length = packet.length;
    memcpy(sendPacket.payload, packet.payload, sendPacket.length);

    tolayer3_B(sendPacket);

    expectedSeqNum++;
  }
  //printf("Packet received\n");
  //check if packet is corrupted or ACK is wrong or timed out
  if(packet.checksum != calcCheckSum(packet) || expectedAcknum != packet.acknum) {

      //if so send back the packet and retart the timer
      tolayer3_B(packet);
  }
  //check if duplicate packet
  else if(lastRcvPacket.acknum == packet.acknum) {
      tolayer3_B(packet);
  }
  //Otherwise send to next state and stop the timer
  else {
    expectedAcknum++;
    msg message;
    message.length = packet.length;
    for(int i = 0; i< message.length; i++) {
      message.data[i] = packet.payload[i];
    }
    tolayer5_B(message);
  }

  lastRcvPacket = packet;

}

void B_timerinterrupt() {
   timeoutB = 1;
}
