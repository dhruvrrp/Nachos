#include "copyright.h"
#include "system.h"
#include "whale.h"
#include "stdio.h"


#include "unistd.h"



Whale::Whale(char *debugName) 
{
		////printf("Start Constructor %d\n",getpid());
        name = debugName;
		m = new Semaphore("Male",0);
		mm = new Semaphore("MatchMaker",0);
		f = new Semaphore("female",0);
		ml = new Lock("Malelock");
		fl = new Lock("Fealelock");
		mml = new Lock("Matchmakerlock");
		male=0;
		female=0;
		matchmaker=0;
        
}
Whale::~Whale()
{
		////printf("Start Des\n");
       delete name;
	   delete m;
	   delete mm;
	   delete f;
}
void
Whale::Male()
{
	ml->Acquire();
	////printf("Start Male%d\n",getpid());

//	//printf("male: %d female %d mm %d\n",male,female,matchmaker);
	male++;
//	//printf("male: %d female %d mm %d\n",male,female,matchmaker);
//	m->P();
	////printf("male: %d female %d mm %d\n",male,female,matchmaker);
	if (male >0 && female >0 && matchmaker >0)
	{
//		m->V();
		f->V();
		mm->V();
		male--;
		female--;
		matchmaker--;
	}
	else
		m->P();
//	m->V();
	ml->Release();
	////printf("%d end Male\n",getpid());
}

void
Whale::Female()
{
	fl->Acquire();
	////printf("Start Female%d\n",getpid());

	female++;
//	f->P();
	////printf("male: %d female %d mm %d\n",male,female,matchmaker);
	if (male >0 && female >0 && matchmaker >0)
	{
		m->V();
//		f->V();
		mm->V();
		male--;
		female--;
		matchmaker--;
	//	//printf("Inside if of female \n");
	}
	else
		f->P();
//	f->V();
	fl->Release();
	////printf("%d end Female\n",getpid());
}

void
Whale::Matchmaker()
{
	mml->Acquire();
	////printf("Start Match%d\n",getpid());

	matchmaker++;
//	mm->P();
	////printf("male: %d female %d mm %d\n",male,female,matchmaker);
	if (male >0 && female >0 && matchmaker >0)
	{
		m->V();
		f->V();
	//	mm->V();
		male--;
		female--;
		matchmaker--;
	}
	else
		mm->P();
//	mm->V();
	mml->Release();
	////printf("%d end Match\n",getpid());
}

/*
Whale::Whale(char *debugName) 
{
		//printf("%d start Cons\n",getpid());
        name = debugName;
		m = new Lock("Male");
		mm = new Lock("MatchMaker");
		f = new Lock("female");
		match = new Condition("match");
		//printf("%d end Cons\n",getpid());
        
}
Whale::~Whale()
{
		//printf("%d start Des\n",getpid());
       delete name;
	   delete m;
	   delete mm;
	   delete f;
	   delete match;
	   //printf("%d end Des\n",getpid());
}
void
Whale::Male()
{
	//printf("%d start Male\n",getpid());
	m->Acquire();
	//while((!f->isHeldFunc()ByCurrentThread())&&(!mm->isHeldFunc()ByCurrentThread()))
	//while(!(f->isHeldFunc() && mm->isHeldFunc())); //isHeldFunc() is private create a getter fucntion 
	match->Wait(f);
	match->Wait(mm);
	
	match->Signal(m);
	
	m->Release();
	
	//printf("%d end Male\n",getpid());
}

void
Whale::Female()
{
	//printf("%d start Female\n",getpid());
	
	f->Acquire();
	
	//while(!(m->isHeldFunc() && mm->isHeldFunc()));
	match->Wait(m);
	match->Wait(mm);
	
	match->Signal(f);
	
	f->Release();
	
	
	f->Acquire();
	match->Wait(f);
	
	match->Signal(m);
	match->Signal(mm);
	
	f->Release();
	*
	//printf("%d end Female\n",getpid());
}

void
Whale::Matchmaker()
{

	mm->Acquire();
	
	//while(!(m->isHeldFunc() && f->isHeldFunc())); 
	match->Wait(f);
	match->Wait(m);
	
	match->Signal(mm);
	
	mm->Release();
	*
	//printf("%d start Match\n",getpid());
	mm->Acquire();
	match->Wait(mm);
	
	match->Signal(f);
	match->Signal(m);
	
	mm->Release();
	*
	//printf("%d end Match\n",getpid());
}
*/
