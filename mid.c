# include <stdlib.h>
# include <sys/wait.h>
# include <stdio.h>
# include <sys/shm.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <string.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 30


int main(){
	int fd1[2];
	int fd2[2];
	char write_msg[BUFFER_SIZE] = "Hello world, this is CS6233\n"; 
	char read_msg[BUFFER_SIZE]; 
	pid_t pid;

	/* create the pipe1 */
	if (pipe(fd1) == 1) {
		fprintf(stderr, "Pipe failed");
		return -1;
	}
	/* create the pipe2 */
	if (pipe(fd2) == 1) {
		fprintf(stderr, "Pipe failed");
		return -1;
	}

	pid = fork();
	if (pid < 0){
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	if (pid == 0){
		close(fd1[READ_END]);
		close(fd2[WRITE_END]);	

		char var1='a';
		write(fd1[WRITE_END],&var1,sizeof(char));
		

		read(fd2[READ_END],&read_msg,BUFFER_SIZE);
		printf("read: %s", read_msg);

		close(fd1[WRITE_END]);
		close(fd2[READ_END]);
		exit(0);

	} else {
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

		char var1='t';
		while(var1!='a'){
			read(fd1[READ_END],&var1,sizeof(char));
		}			
		
		write(fd2[WRITE_END],&write_msg,strlen(write_msg) + 1);
		close(fd1[READ_END]);
		close(fd2[WRITE_END]);
		wait(0);
	}
	return 0;
}
