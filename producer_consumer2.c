/* 
@Program: Producer & Consumer
@About: Solve the producer and consumer problem with out using sychronization techniques.

@Solution: My solution is to use a first in first out buffer. To do this I force the consumer to sleep if either there is no products in the buffer or if it is at the same index of the last product put into the buffer. To keep the loop going I check when both the producer and consumer get to the end of the buffer and reset to starting conditions without changing the products processed this way. 

@Author: Chris Hyde
@Date April 13, 2018
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define bufferLength 10


//Global variables
static int buffer[bufferLength];
int inIndex = 0, outIndex = 0;
pid_t conID, proID; 


//Put and Get Functions

void put(int i){
	//Put the producer to sleep when it hits buffer limit
	if(inIndex == bufferLength) sleep(5);

	// Place theProduct in the buffer at index of inIndex 
	buffer[inIndex] = i;

	//Increment inIndex for next product placement 
	inIndex = inIndex + 1;

	//wake up consumer if there is 1 or more products in the buffer.
	if(inIndex > 0) wakeup(conID);
}

int get(){
	//put the consumer to sleep if there is no product in buffer
	if(inIndex == 0) sleep(5);	

	//Get the product at index outIndex and set it to i
	int i = buffer[outIndex];
	
	//Icnrement outIndex for getting next product 
	outIndex = outIndex + 1;

	//If both inIndex and outIndex equal 10 reset the process and wakeup the producer. 
	if(inIndex == bufferLength && outIndex == bufferLength){
	inIndex = 0;
	outIndex = 0;
	wakeup(proID);
	}

	//Fail safe incase outIndex trys to get ahead of inIndex to garanty inIndex processes next
	if(outIndex == inIndex) sleep(5);
	
	//return the value of the product
	return i; 
	
}

//The Given Functions for the Assignment.

int theProduct;

int produce(){
	theProduct++;
	return theProduct % 100;
}

void consume(int i){
	printf("%i\n", i);
}



void producer(){
	int i;
	while(1){
		i = produce();
		put(i);
	}
}

void consumer(){
	int i;
	while(1){
		i = get();
		consume(i);
	}
}

void *processControl(){
	
	//Start 2 new process
	pid_t pid = fork();

	//set the process with id 0 to producer the other to consumer. 
	if(pid == 0){ // Child Process
		proID = getpid();
		producer();	
	}else{ // Parent Process
		conID = getpid();
		consumer();
	}
	

}


//Main Function
int main(){
	pthread_t pc;
	pthread_create(&pc, 0, processControl, 0);
	pthread_join(pc, 0);
	
	return 0; 
}

