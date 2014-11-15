// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "mailbox.h"
#include "whale.h"

#include "unistd.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}
Mailbox *ma = new Mailbox("Mail");


void
Sender(int param)
{
	//printf("sender %d PID %d \n", param, getpid());

//	printf("Send mail? \n");
	ma->Send(param);
}
void
Reciever(int param)
{
	//printf("reciever PID %d \n", getpid());

	int *a =0;
	ma->Receive(a);
//	printf("Recieved mail %d \n", a);
}


//----------------------------------------------------------------------
// LockTest1
//----------------------------------------------------------------------

Lock *locktest1 = NULL;
Lock *locktestDel = NULL;

void
LockThread1(int param)
{
    locktestDel = new Lock("LockTestDel");  //TEST 3
    delete locktestDel;   
    ASSERT(locktestDel);

    printf("Thread:0\n");
    locktest1->Acquire();
    printf("Thread:1\n");
    currentThread->Yield();
    printf("Thread:2\n");
    locktest1->Release();
    printf("Thread:3\n");
}

void
LockThread2(int param)
{
    printf("L2:0\n");
    locktest1->Acquire();
    printf("L2:1\n");
    currentThread->Yield();
    printf("L2:2\n");
    locktest1->Release();
    printf("L2:3\n");
}

void
LockTest1()
{
    DEBUG('t', "Entering LockTest1");

    locktest1 = new Lock("LockTest1");

    Thread *t = new Thread("one");
    t->Fork(LockThread1, 0);
    t = new Thread("two");
    t->Fork(LockThread2, 0);
}

////
//Mail Test
/////


void
MailTestRecThenSend()
{
	printf("Multi rec then multi send START\n");
	Thread *b = new Thread("Reciever");
	Thread *b1 = new Thread("Reciever1");
	Thread *b2 = new Thread("Reciever2");
	Thread *b3 = new Thread("Reciever3");
	
	b->Fork(Reciever, 0);
	b1->Fork(Reciever, 1);
	b2->Fork(Reciever, 2);
	b3->Fork(Reciever, 3);
	
	Thread *a = new Thread("Send");
	Thread *a1 = new Thread("Send1");
	Thread *a2 = new Thread("Send2");
	Thread *a3 = new Thread("Send3");
	
	a->Fork(Sender, 1);
	a1->Fork(Sender, 2);
	a2->Fork(Sender, 3);
	a3->Fork(Sender, 4);
	
	printf("Multi rec then multi send END\n");
}

void
MailTestRandomSendRec()
{
	printf("Random send rec START\n");
	Thread *b = new Thread("Reciever");
	Thread *b1 = new Thread("Reciever1");
	Thread *b2 = new Thread("Reciever2");
	Thread *b3 = new Thread("Reciever3");
	
	Thread *a = new Thread("Send");
	Thread *a1 = new Thread("Send1");
	Thread *a2 = new Thread("Send2");
	Thread *a3 = new Thread("Send3");
	
	b->Fork(Reciever, 0);
	a->Fork(Sender, 1);
	a1->Fork(Sender, 2);
	b1->Fork(Reciever, 1);
	b2->Fork(Reciever, 2);
	a2->Fork(Sender, 3);
	b3->Fork(Reciever, 3);
	a3->Fork(Sender, 4);
	
	printf("Random send rec END\n");
}

void
MailTestMoreSend()
{
	printf("Mail more send START\n");
	
	Thread *a = new Thread("Send");
	Thread *a1 = new Thread("Send1");
	Thread *a2 = new Thread("Send2");
	Thread *a3 = new Thread("Send3");
	
	a->Fork(Sender, 1);
	a1->Fork(Sender, 2);
	a2->Fork(Sender, 3);
	a3->Fork(Sender, 4);
	
	Thread *b = new Thread("Reciever");
	Thread *b1 = new Thread("Reciever1");
	Thread *b2 = new Thread("Reciever2");
	
	
	b->Fork(Reciever, 0);
	b1->Fork(Reciever, 1);
	b2->Fork(Reciever, 2);
	
	printf("Mail more send END\n");
}

void
MailTestMoreReceive()
{
	printf("Mail more receive START\n");
	Thread *a = new Thread("Send");
	Thread *a1 = new Thread("Send1");
	Thread *a2 = new Thread("Send2");
	
	a->Fork(Sender, 1);
	a1->Fork(Sender, 2);
	a2->Fork(Sender, 3);
	
	Thread *b = new Thread("Reciever");
	Thread *b1 = new Thread("Reciever1");
	Thread *b2 = new Thread("Reciever2");
	Thread *b3 = new Thread("Reciever3");
	
	b->Fork(Reciever, 0);
	b1->Fork(Reciever, 1);
	b2->Fork(Reciever, 2);
	b3->Fork(Reciever, 3);
	
	printf("Mail more receive END\n");
}


void
MailTest()
{
	printf("MAilTesT\n");
//	Lock *as = new Lock("AAA");
	
	MailTestMoreReceive();
	MailTestMoreSend();
	MailTestRecThenSend();
	MailTestRandomSendRec();

	Thread *t = new Thread("Sender");
	t->Fork(Sender, 5);


	Thread *t3 = new Thread("Sender1");

	t3->Fork(Sender, 7);	
	Thread *t4 = new Thread("Reciever");

	t4->Fork(Reciever, 0);
	
	///
	printf("Multi send then multi receive\n");
	Thread *a = new Thread("Send");
	Thread *a1 = new Thread("Send1");
	Thread *a2 = new Thread("Send2");
	Thread *a3 = new Thread("Send3");
	
	a->Fork(Sender, 1);
	a1->Fork(Sender, 2);
	a2->Fork(Sender, 3);
	a3->Fork(Sender, 4);
	
	Thread *b = new Thread("Reciever");
	Thread *b1 = new Thread("Reciever1");
	Thread *b2 = new Thread("Reciever2");
	Thread *b3 = new Thread("Reciever3");
	
	b->Fork(Reciever, 0);
	b1->Fork(Reciever, 1);
	b2->Fork(Reciever, 2);
	b3->Fork(Reciever, 3);
	

}


//Mail Test End


//Priority Test

void LowPriorityFirst(int param)
{
	printf("Low priority first test \n");
	Thread *t1 = new Thread("100");
	Thread *t2 = new Thread("80");
	Thread *t3 = new Thread("60");
	t1->setPriority(100);
	t2->setPriority(80);
	t3->setPriority(60);
	t3->Fork(SimpleThread, 60);
	t2->Fork(SimpleThread, 80);
	t1->Fork(SimpleThread, 100);
	printf("High priority first test \n");
}
void HighPriorityFirst(int param)
{
	printf("High priority first test \n");
	Thread *t1 = new Thread("100");
	Thread *t2 = new Thread("80");
	Thread *t3 = new Thread("60");
	t1->setPriority(100);
	t2->setPriority(80);
	t3->setPriority(60);
	t1->Fork(SimpleThread, 100);
	t2->Fork(SimpleThread, 80);
	t3->Fork(SimpleThread, 60);
	printf("High priority first test \n");
}
void HighPriorityYield(int param)
{
	printf("High priority yield Start \n");
	Thread *t1 = new Thread("100");
	Thread *t2 = new Thread("80");
	Thread *t3 = new Thread("60");
	t1->setPriority(100);
	t2->setPriority(80);
	t3->setPriority(60);
	t1->Fork(SimpleThread, 100);
	t1->Yield();
	t2->Fork(SimpleThread, 80);
	t3->Fork(SimpleThread, 60);
	printf("High priority yield end \n");
}


void
PriorityTest()
{
	Thread *a = new Thread("One");
	Thread *a1 = new Thread("Two");
	Thread *a2 = new Thread("Three");
	a->Fork(LowPriorityFirst, 0);
	a1->Fork(HighPriorityFirst, 1);
	a2->Fork(HighPriorityYield, 2);
}




//Part 5 whale

Whale *w = new Whale("Whale");

void
WhaleMale(int param)
{
	w->Male();
}

void
WhaleFemale(int param)
{
	w->Female();
}
void
WhaleMatchmaker(int param)
{
	w->Matchmaker();
}

void WhaleTest1(){

    Thread *t1 = new Thread("Male1");
    Thread *t2 = new Thread("Male2");
    Thread *t3 = new Thread("Male3");
    Thread *t4 = new Thread("Matchmaker1");
    Thread *t5 = new Thread("Female1");
    Thread *t6 = new Thread("Matchmaker2");
    Thread *t7 = new Thread("Female2");

    t1->Fork(WhaleMale,1);
    t2->Fork(WhaleMale,-1);
    t3->Fork(WhaleMale,2);
    t4->Fork(WhaleMatchmaker,3);
    t5->Fork(WhaleFemale,4);
    t6->Fork(WhaleMatchmaker,5);
    t7->Fork(WhaleFemale,-1);
    currentThread->Yield();
	 
     printf("Test Over !!\n");
 
}

void WhaleTestMaleFemaleMM(){

	printf("Test WhaleTestMaleFemaleMM START !!\n");
    Thread *t1 = new Thread("Male1");
    Thread *t2 = new Thread("Male2");
    Thread *t3 = new Thread("Male3");
    Thread *t4 = new Thread("Matchmaker1");
    Thread *t5 = new Thread("Female1");
    Thread *t6 = new Thread("Matchmaker2");
    Thread *t7 = new Thread("Female2");

    t1->Fork(WhaleMale,1);
    t2->Fork(WhaleMale,-1);
    t3->Fork(WhaleMale,2);
    t5->Fork(WhaleFemale,4);
	t7->Fork(WhaleFemale,-1);
    t6->Fork(WhaleMatchmaker,5);
	t4->Fork(WhaleMatchmaker,3);
    
    currentThread->Yield();
	
	//One extra male stays pending
	 
     printf("Test WhaleTestMaleFemaleMM Over !!\n");
 
}

void WhaleTestFemaleMaleMM(){

	printf("Test WhaleTestFemaleMaleMM START !!\n");
    Thread *t1 = new Thread("Male1");
    Thread *t2 = new Thread("Male2");
    Thread *t3 = new Thread("Male3");
    Thread *t4 = new Thread("Matchmaker1");
    Thread *t5 = new Thread("Female1");
    Thread *t6 = new Thread("Matchmaker2");
    Thread *t7 = new Thread("Female2");

    t5->Fork(WhaleFemale,4);
	t7->Fork(WhaleFemale,-1);
    t1->Fork(WhaleMale,1);
    t2->Fork(WhaleMale,-1);
    t3->Fork(WhaleMale,2);
    t6->Fork(WhaleMatchmaker,5);
	t4->Fork(WhaleMatchmaker,3);
    
    currentThread->Yield();
	
	//One extra male stays pending
	 
     printf("Test WhaleTestFemaleMaleMM Over !!\n");
 
}

void WhaleTestMMFemaleMale(){

	printf("Test WhaleTestMMFemaleMale START !!\n");
    Thread *t1 = new Thread("Male1");
    Thread *t2 = new Thread("Male2");
    Thread *t3 = new Thread("Male3");
    Thread *t4 = new Thread("Matchmaker1");
    Thread *t5 = new Thread("Female1");
    Thread *t6 = new Thread("Matchmaker2");
    Thread *t7 = new Thread("Female2");

    t6->Fork(WhaleMatchmaker,5);
	t4->Fork(WhaleMatchmaker,3);
    t5->Fork(WhaleFemale,4);
	t7->Fork(WhaleFemale,-1);
    t1->Fork(WhaleMale,1);
    t2->Fork(WhaleMale,-1);
    t3->Fork(WhaleMale,2);
    
    currentThread->Yield();
	
	//One extra male stays pending
	 
     printf("Test WhaleTestMMFemaleMale Over !!\n");
 
}



//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
Joiner(Thread *joinee)
{
  currentThread->Yield();
  currentThread->Yield();

  printf("Waiting for the Joinee to finish executing.\n");

  currentThread->Yield();
  currentThread->Yield();

  // Note that, in this program, the "joinee" has not finished
  // when the "joiner" calls Join().  You will also need to handle
  // and test for the case when the "joinee" _has_ finished when
  // the "joiner" calls Join().

  joinee->Join();

  currentThread->Yield();
  currentThread->Yield();

  printf("Joinee has finished executing, we can continue.\n");

  currentThread->Yield();
  currentThread->Yield();
}

void
Joinee()
{
  int i;

  for (i = 0; i < 5; i++) {
    printf("Smell the roses.\n");
    currentThread->Yield();
  }

  currentThread->Yield();
  printf("Done smelling the roses!\n");
  currentThread->Yield();
}

void
ForkerThread()
{
  Thread *joiner = new Thread("joiner", 0);  // will not be joined
  Thread *joinee = new Thread("joinee", 1);  // WILL be joined

  // fork off the two threads and let them do their business
  joiner->Fork((VoidFunctionPtr) Joiner, (int) joinee);
  joinee->Fork((VoidFunctionPtr) Joinee, 0);

  // this thread is done and can go on its merry way
  printf("Forked off the joiner and joiner threads.\n");
}





/* Testing case where Joinee has finished before Joiner*/
void Joinee1() {
	printf("Just 1 line in this code yo and we done\n");
	currentThread->Yield();
}
void Joiner1(Thread *t) {
	for(int i =0; i<5; i++) {
		printf(" thread joiner1 looped %d times \n", i);
		currentThread->Yield();
	}
	printf("Calling Join, Joinee would be done much before this!\n");
	t->Join();
	printf("We Done!");
	currentThread->Yield();
	printf("We Done!");
	currentThread->Yield();
}
void Test32() {
	Thread *joiner1 = new Thread("joiner1", 0);
	Thread *joinee = new Thread("joinee", 1);
	joiner1->Fork((VoidFunctionPtr) Joiner, (int) joinee);
	joinee->Fork((VoidFunctionPtr) Joinee, 0);
}
/* Testing case where Thread t tries to join itself*/
void Test33() {
	Thread* joiner = new Thread("joiner", 1);
	printf("A function is about to fork itself\n");
	joiner->Fork((VoidFunctionPtr) Joiner, (int) joiner);
}
/*Testing case where join is called more than once*/
void Joinee2() {
	printf("Just 1 line in this code yo and we done\n");
	currentThread->Yield();
}
void Joiner2(Thread *t) {
	for(int i =0; i<5; i++) {
		printf("thread joiner1 looped %d times \n", i);
		currentThread->Yield();
	}
	printf("Calling Join, Joinee would be done much before this!\n");
	t->Join();
	printf("Calling join once again after this");
	t->Join();
	currentThread->Yield();
	printf("We Done!");
	currentThread->Yield();
}
void Test34() {
	Thread *joiner1 = new Thread("joiner1", 0);//not be joined
	Thread *joinee = new Thread("joinee", 1);//will be joined
	joiner1->Fork((VoidFunctionPtr) Joiner, (int) joinee);
	joinee->Fork((VoidFunctionPtr) Joinee, 0);
}
void Test35() {
	Thread *joiner1 = new Thread("joiner1", 0);
	Thread *joinee = new Thread("joinee", 0);
	printf("Thread should not be allowed to fork\n");
	joiner1->Fork((VoidFunctionPtr) Joiner, (int) joinee);
}








void
ThreadTest()
{
    switch (testnum) {
    case 1:
        ThreadTest1();
        break;
    case 2:
	MailTest();
	break;
    case 3:
	PriorityTest();
	break;
    case 41:
	WhaleTest1();
	break;
	case 42:
	WhaleTestMaleFemaleMM();
	break;
	case 43:
	WhaleTestFemaleMaleMM();
	break;
	case 44:
	WhaleTestMMFemaleMale();
	break;
    case 5:
	ForkerThread();
	break;
    case 6:
	LockTest1();
	break;
    case 7:
	Test32();
	Test33();
	Test34();
	break;
    case 8:
	Test33();
	break;
    case 9:
	Test35();
	break;
    default:
        printf("No test specified.\n");
        break;
    }
}

