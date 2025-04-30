#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "emulator.h"
#include "sr.h"

/* ******************************************************************
   Go Back N protocol.  Adapted from J.F.Kurose
   ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.2  

   Network properties:
   - one way network delay averages five time units (longer if there
   are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
   or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
   (although some can be lost).

   Modifications: 
   - removed bidirectional GBN code and other code not used by prac. 
   - fixed C style to adhere to current programming style
   - added GBN implementation
**********************************************************************/

#define RTT  16.0       /* round trip time.  MUST BE SET TO 16.0 when submitting assignment */
#define WINDOWSIZE 6    /* the maximum number of buffered unacked packet */
#define SEQSPACE 12      /* the min sequence space for SR must be at least windowsize * 2 */
#define NOTINUSE (-1)   /* used to fill header fields that are not being used */

/* generic procedure to compute the checksum of a packet.  Used by both sender and receiver  
   the simulator will overwrite part of your packet with 'z's.  It will not overwrite your 
   original checksum.  This procedure must generate a different checksum to the original if
   the packet is corrupted.
*/

/* Used to check contents for corruption. Corrupted contents will have different checksum between sender and receiver */
int ComputeChecksum(struct pkt packet)  
{
  int checksum = 0;
  int i;

  checksum = packet.seqnum;
  checksum += packet.acknum;
  for ( i=0; i<20; i++ ) 
    checksum += (int)(packet.payload[i]);

  return checksum;
}

bool IsCorrupted(struct pkt packet)
{
  if (packet.checksum == ComputeChecksum(packet))
    return (false);
  else
    return (true);
}


/********* Sender (A) variables and functions ************/

static struct pkt buffer[WINDOWSIZE];  /* array for storing packets waiting for ACK */
static int windowfirst, windowlast;    /* array indexes of the first/last packet awaiting ACK */
static int windowcount;                /* the number of packets currently awaiting an ACK */
static int A_nextseqnum;               /* the next sequence number to be used by the sender */

/* called from layer 5 (application layer), passed the message to be sent to other side */
void A_output(struct msg message)
{
  struct pkt sendpkt;
  int i;

  /* if not blocked waiting on ACK */
  if ( windowcount < WINDOWSIZE) {
    if (TRACE > 1)
      printf("----A: New message arrives, send window is not full, send new messge to layer3!\n");

    /* create packet */
    sendpkt.seqnum = A_nextseqnum;
    sendpkt.acknum = NOTINUSE;
    for ( i=0; i<20 ; i++ ) 
      sendpkt.payload[i] = message.data[i];
    sendpkt.checksum = ComputeChecksum(sendpkt); 

    /* put packet in window buffer */
    /* windowlast will always be 0 for alternating bit; but not for GoBackN */
    windowlast = (windowlast + 1) % WINDOWSIZE; 
    buffer[windowlast] = sendpkt;
    windowcount++;

    /* send out packet */
    if (TRACE > 0)
      printf("Sending packet %d to layer 3\n", sendpkt.seqnum);
    tolayer3 (A, sendpkt);

    /* start timer if first packet in window */
    if (windowcount == 1)
      starttimer(A,RTT);

    /* get next sequence number, wrap back to 0 */
    A_nextseqnum = (A_nextseqnum + 1) % SEQSPACE;  
  }
  /* if blocked,  window is full */
  else {
    if (TRACE > 0)
      printf("----A: New message arrives, send window is full\n");
    window_full++;
  }
}


/* called from layer 3, when a packet arrives for layer 4 
   In this practical this will always be an ACK as B never sends data.
*/
void A_input(struct pkt packet)
{
  int i;
  bool dupCheck = false;

  /*These var might be used for later if I need something to keep track of ACKed packets*/
  /*int ACKed;*/
  /* bool ACKed[SEQSPACE]; */

  /* if received ACK is not corrupted */ 
  if (!IsCorrupted(packet)) {
    if (TRACE > 0)
      printf("----A: uncorrupted ACK %d is received\n",packet.acknum);
    total_ACKs_received++;

    /* check if new ACK or duplicate */
    for(i=0 ; i<6 ; i++) {
      if(packet.acknum == buffer[i].seqnum) {
        dupCheck = true;
        break;
      }
    }

    if (windowcount != 0 && dupCheck) { /* If there are packets waiting to be ACK */
          int seqfirst = buffer[windowfirst].seqnum;
          int seqlast = buffer[windowlast].seqnum;
          /* check case when seqnum has and hasn't wrapped */
          if (((seqfirst <= seqlast) && (packet.acknum >= seqfirst && packet.acknum <= seqlast)) ||
              ((seqfirst > seqlast) && (packet.acknum >= seqfirst && packet.acknum <= seqlast))) {  /* Check within ACKed window. This takes into account that the window go around */

            /* packet is a new ACK */
            if (TRACE > 0)
              printf("----A: ACK %d is not a duplicate\n",packet.acknum);
            new_ACKs++;

            /* Individual ACK for incoming packets */
            /* slide window by the number of packets ACKed */
            windowfirst = (windowfirst + 1) % WINDOWSIZE;
            /* delete the acked packets from window buffer */
            windowcount--;
          



	    /* start timer again if there are still more unacked packets in window */
            stoptimer(A);
            if (windowcount > 0)
              starttimer(A, RTT);

          }
        }
        else
          if (TRACE > 0)
        printf ("----A: duplicate ACK received, do nothing!\n");
  }
  else 
    if (TRACE > 0)
      printf ("----A: corrupted ACK is received, do nothing!\n");
}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{
  int i;

  if (TRACE > 0)
    printf("----A: time out,resend packets!\n");

  for(i=0; i<windowcount; i++) {

    if (TRACE > 0)
      printf ("---A: resending packet %d\n", (buffer[(windowfirst+i) % WINDOWSIZE]).seqnum);

    tolayer3(A,buffer[(windowfirst+i) % WINDOWSIZE]);
    packets_resent++;
    if (i==0) starttimer(A,RTT);
  }
}       



/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(void)
{
  /* initialise A's window, buffer and sequence number */
  A_nextseqnum = 0;  /* A starts with seq num 0, do not change this */
  windowfirst = 0;
  windowlast = -1;   /* windowlast is where the last packet sent is stored.  
		     new packets are placed in winlast + 1 
		     so initially this is set to -1
		   */
  windowcount = 0;
}



/********* Receiver (B)  variables and procedures ************/

static int expectedseqnum; /* the sequence number expected next by the receiver */
static int B_nextseqnum;   /* the sequence number for the next packets sent by B */
/* Make an array the size of SEQSPACE to check to received elements
use round number to ensure that we will not mistake element of new round
to old one */
static int ACKed[SEQSPACE];

/*Buffer to store the received out-of-order packets*/
static int B_buffer[WINDOWSIZE];
static int roundNum;

/*Used to check if the input num is within the boundaries. 
Seperated to a function for better readability*/
bool windCheck(int inputSeqNum , int windBegin , int windEnd);

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
  struct pkt sendpkt;
  int i;
  int winEndNum;

  if(expectedseqnum == 0) {
    roundNum++;
  }

  /* DO nothing and return if packet is duplicate*/
  if(ACKed[packet.seqnum] != roundNum) {
    if(TRACE > 0) {
      printf("----B: packet %d is dupplicate or unexpected packet number, do nothing" , packet.seqnum);
    }
    return;
  }

  /*Mark end of buffer position*/
  winEndNum = (expectedseqnum + WINDOWSIZE) % SEQSPACE;
  
  /* if not corrupted and received packet is in order */
  if  ( (!IsCorrupted(packet))  && (packet.seqnum == expectedseqnum) ) {
    if (TRACE > 0)
      printf("----B: packet %d is correctly received, send ACK!\n",packet.seqnum);
    packets_received++;

    /*Log change to ACKed array and roundNum*/
    ACKed[packet.seqnum]++;

    /* Deliver the packet*/
    tolayer5(B, packet.payload);

    /* Increment the next expected seqnum */
    expectedseqnum = (expectedseqnum + 1) % WINDOWSIZE;

    /*Deliver the rest of the buffer if any is left*/
    i = packet.seqnum;
    while(ACKed[expectedseqnum] == roundNum) {
      tolayer5(B, buffer[i].payload);
      i++;
    }
    /* send an ACK for the received packet */
    sendpkt.acknum = packet.seqnum;

    /* update state variables */
    expectedseqnum = (expectedseqnum + 1) % SEQSPACE; 

    /*Process packets that come in out of order and in window*/
  } else if((!IsCorrupted(packet)) && (windCheck(packet.seqnum , expectedseqnum , winEndNum))) {

    if(TRACE > 0) {
      printf("----B: packet %d is received as out of order in receiving window, buffering packet" , packet.seqnum);
    }
    packets_received++;

    /*log the change to buffer*/
    ACKed[packet.seqnum]++;

    /*Buffer packets to window*/
    /*Check a2_breakdown.txt the B_buffer part (line 126) for breakdown*/
    B_buffer;

    /*Send ACK*/
    sendpkt.acknum = packet.seqnum;
  }
  else {
    /* packet is corrupted or out of order resend last ACK */
    if (TRACE > 0) 
      printf("----B: packet corrupted or not expected sequence number, resend ACK!\n");
    if (expectedseqnum == 0)
      sendpkt.acknum = SEQSPACE - 1;
    else
      sendpkt.acknum = expectedseqnum - 1;
  }

  
  /*Will get back to these parts later if there's an issue*/
  /* create packet */
  sendpkt.seqnum = B_nextseqnum;
  B_nextseqnum = (B_nextseqnum + 1) % 2;
    
  /* we don't have any data to send.  fill payload with 0's */
  for ( i=0; i<20 ; i++ ) 
    sendpkt.payload[i] = '0';  

  /* computer checksum */
  sendpkt.checksum = ComputeChecksum(sendpkt); 

  /* send out packet */
  tolayer3 (B, sendpkt);
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(void)
{
  int i;

  expectedseqnum = 0;
  B_nextseqnum = 1;
  roundNum = 0;

  /*Fill ACKed with 0 to start round*/
  for(i=0 ; i<SEQSPACE ; i++) {
    ACKed[i] = 0;
  }
}

/******************************************************************************
 * The following functions need be completed only for bi-directional messages *
 *****************************************************************************/

/* Note that with simplex transfer from a-to-B, there is no B_output() */
void B_output(struct msg message)  
{
}

/* called when B's timer goes off */
void B_timerinterrupt(void)
{
}

/*The extra functions needed to implement code*/
bool windCheck(int inputSeqNum , int windBegin , int windEnd) {
  if(windBegin <= windEnd) {
    if(inputSeqNum <= windEnd && inputSeqNum >= windBegin) {
      return true;
    }

  } else if(windBegin > windEnd) {
    if(inputSeqNum >= windEnd && inputSeqNum <= windBegin) {
      return true;
    }
  }

  return false;
}

