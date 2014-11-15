#ifndef WHALE_H
#define WHALE_H

#include "synch.h"
#include "thread.h"
#include "list.h"

class Whale {

  public:
        Whale(char *debugName);               // constructor
        ~Whale();                     // default destructor
        
        char *getName() {return name;}; // get name, used for debugging
        
        void Male();      
        void Female();     
		void Matchmaker();

  private:
        char *name;                     // name used for debugging
        Semaphore *m;        
        Semaphore *mm;
		Semaphore *f;
		Lock *ml;
		Lock *fl;
		Lock *mml;
        int male;
		int female;
		int matchmaker;
};

#endif
