#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <bits/stdc++.h>
using namespace std; 

int main() 
{ 
	string input_text;
	// ftok to generate unique key 
	key_t key = ftok("shmfile",65); 

	// shmget returns an identifier in shmid 
	int shmid = shmget(key,1024,0666|IPC_CREAT); 

	// shmat to attach to shared memory 
	char *str = (char*) shmat(shmid,(void*)0,0); //nista nas ne sprecava da deljenu memoriju posmatramo kao bafer odnosno niz struktura item (po primeru iz skripte)

	cout<<"Write Data : "; 
	//gets(str); 
	getline(std::cin, input_text);
	strcpy(str, input_text.c_str());

	printf("Data written in memory: %s\n",str); 
	
	//detach from shared memory 
	shmdt(str); 

	return 0; 
} 
