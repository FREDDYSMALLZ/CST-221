/*

@Name: Semaphore Solution for Sycronized Document Access

@About: A solution to a senario in which multiple threads try to access and modify the funds in a bank account. 

@Author: Chris Hyde
@Date: April 13, 2018

*/


#include <stdio.h>
#include <pthread.h>

//Define a Semaphore
typedef int semaphore;
semaphore lock = 0;

// Sudo Bank account info

int accountNumber = 576009;
double balance = 500;

//Actions of two ATM's attempting to acces bank account 576009

void *atm1(){
	//loop to properly show semaphore usage
	for(int i = 1; i <10; i++){
		//trigger the lock for this atm usage
		if (accountNumber == 576009 && lock != 1) {
		lock = 1;
		//Deposite $50 into the account
		printf("ATM 1: Account %d has a balance of $%f\n", accountNumber, balance);
		balance = balance + 50; 
		printf("You have deposited $50 into your account %d. Your balance is now $%f\n", accountNumber, balance);
		//Finish transaction and release lock
		printf("Thank you for Banking DWB\n"); 
		lock = 0;
		}else{ printf("ATM 1: The Account %d cannot be accessed right now try again later\n", accountNumber);} 
	}	
}

void *atm2(){
	//loop to properly show semaphore usage
	for(int i = 1; i <10; i++){
		//trigger the lock for this atm usage
		if (accountNumber == 576009 && lock != 1) {
		lock = 1;
		//Deposite $50 into the account
		printf("ATM 2: Account %d has a balance of $%f\n", accountNumber, balance);
		balance = balance + 50; 
		printf("You have deposited $50 into your account %d. Your balance is now $%f\n", accountNumber, balance);
		//Finish transaction and release lock
		printf("Thank you for Banking DWB\n"); 
		lock = 0;
		}else{ printf("ATM 2: The Account %d cannot be accessed right now try again later\n", accountNumber);} 
	}	
}


int main(){
	//Create the two threads variables
	pthread_t userBob, userJan;
	pthread_create(&userBob, 0, atm1, 0);
	pthread_create(&userJan, 0, atm2, 0);
	
	//Let the user know the threads are going be created and started
	printf("Starting threads\n");
	pthread_join(userBob, 0);
	pthread_join(userJan, 0);

	//report the new state of the bank funds
	printf("After the two threads the balance is $%f", balance);

	//exit the program
	return 0;

}



