
// C Program for Message Queue (Writer Process) 
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
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    //message.mesg_type = 1; 
    message.mesg_type = atoi(argv[1]); 
  
    printf("Write Data : "); 
    fgets(message.mesg_text, sizeof(message.mesg_text),stdin); 
  
    // msgsnd to send message 
    msgsnd(msgid, &message, sizeof(message.mesg_text), 0); 
  
    // display the message 
    printf("Data sent to queue: %s \n", message.mesg_text); 
  
    return 0; 
} 
