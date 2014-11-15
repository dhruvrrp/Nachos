#ifndef MAILBOX_H
#define MAILBOX_H

#include "synch.h"
#include "thread.h"
#include "list.h"

class Mailbox {

  public:
        Mailbox(char *debugName);               // constructor
        ~Mailbox();                     // default destructor
        
        char *getName() {return name;}; // get name, used for debugging
        
        void Send(int mesage);          // send message
        void Receive(int *message);     // receive message

  private:
        char *name;                     // name used for debugging
        Lock *recv;        
        Lock *snd;   
        Condition *meet;
        List *msgBuffer;           // message buffer
};

#endif
