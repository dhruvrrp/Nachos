#include "copyright.h"
#include "synch.h"
#include "system.h"
#include "mailbox.h"
#include "stdio.h"


#include "unistd.h"


Mailbox::Mailbox(char *debugName) 
{
        name = debugName;
        recv = new Lock("Rcv");
        snd = new Lock("Send");
        meet = new Condition("meet");
        msgBuffer = new List();
}
Mailbox::~Mailbox()
{
        delete recv;
        delete snd;
        delete meet;
        delete msgBuffer;
}
void
Mailbox::Send(int message)
{
	//printf("Send start %d\n", message);
	snd->Acquire();
//	//printf("Thread ID: %d \n", getpid());
//	//printf("Value of lock %d \n", &snd);
	msgBuffer->Append((void *)message);
//	while(msgBuffer != NULL)
//	{
	//	//printf("Still here?\n");
		meet->Signal(recv);

		meet->Wait(snd);
	//	//printf("Wheree is the core dumpinggg \n");
//	}
	//printf("After while send \n");
        // Waiting for message to be received for recv
		//meet->Wait(recv)
	snd->Release();
	   // Adding message to buffer    
}

void
Mailbox::Receive(int * message)
{
	//printf("Recieve \n");
	recv->Acquire();
//	meet->Wait(recv);
	do
	{
	message = (int *)msgBuffer->Remove();
	
//		//printf("recieve while %d \n", message);
	if(message == NULL)
		meet->Wait(recv);
//	//printf("Recieve awake%d\n", message);
	}while(message == NULL);

	//printf("before signal \n");
	meet->Signal(snd);
	//printf("Start? \n");
//	message = (int *)msgBuffer->Remove();
	//printf("Message recieved = %d\n", message);
	recv->Release();
	//printf("End of recieve \n");
}
