
// C Program for Message Queue (Reader Process) 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
  
// structure for message queue 
struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[100]; 
} message; 
  
int main(int argc, char** argv) 
{ 
    key_t key; 
    int msgid; 

    int destroy_queue = atoi(argv[2]);
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
  
    // msgrcv to receive message 
    //msgrcv(msgid, &message, sizeof(message), 1, 0); 
    msgrcv(msgid, &message, sizeof(message.mesg_text), atoi(argv[1]), 0); 
  
    // display the message 
    printf("Data Received is : %s \n",  
                    message.mesg_text); 
  
    if(destroy_queue){
	    // to destroy the message queue 
	    printf("Queue destroyed\n");
	    msgctl(msgid, IPC_RMID, NULL); 
    }
  
    return 0; 
} 
