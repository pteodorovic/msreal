#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(){
	char write_msg[BUFFER_SIZE] = "Hello from here!";
	char read_msg[BUFFER_SIZE];

	int fd[2];
	pid_t pid;
	if(pipe(fd) == -1){
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	pid = fork();//od ovog mesta nastavljaju da se izvrsavaju dva programa
	if (pid < 0){
		fprintf(stderr,"Fork failed");
		return 1;
	}
	if (pid > 0){//roditeljski proces gde je pid zapravo process id od dete procesa
		close(fd[READ_END]);
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		printf("write %s in parent process\n",write_msg);
		close(fd[WRITE_END]);
	}
	else{//dete proces koji je kreiran pomocu fork
		close(fd[WRITE_END]);
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("read %s in child process\n", read_msg);
		close(fd[READ_END]);
	}
	return 0;
}


