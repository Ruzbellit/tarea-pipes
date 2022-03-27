#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#define READ  0
#define WRITE 1

int main() {

  pid_t pid;
  int   fd[2];

  if (pipe(fd) == -1) {
	perror("Creating pipe");
	exit(EXIT_FAILURE);
  }

  switch(pid = fork()) {

  case 0:
	// The child process
	// Close the pipe write descriptor.
	close(fd[WRITE]);
	// Redirect STDIN to read from the pipe.
	dup2(fd[READ], STDIN_FILENO);

	char bufferEntrada[4];
	read(fd[READ], bufferEntrada, strlen(bufferEntrada));
	// convert string [buf] to number
	int number = strtol(bufferEntrada, NULL, 10);

	// El hijo imprimira un mensaje si es mayor o menor que 500
	if(number < 500){
		printf("%i : menor que 500\n", number);
	} else {
		printf("%i : mayor o igual a 500\n", number);
	}

  default:
	// The parent process will execute ls.
	// Close the pipe read descriptor.
	close(fd[READ]);
	// Redirect STDOUT to write to the pipe.
	dup2(fd[WRITE], STDOUT_FILENO);
	//El proceso padre generará un número entero aleatorio entre 1 y 1000
	srand(time(NULL));
	int upper = 1000;
	int lower = 1;
	int random_number = (rand() % (upper - lower)) + lower;
	char bufferSalida[4];
	// convert number to string [buf]
	snprintf(bufferSalida, sizeof(bufferSalida), "%d", random_number);
	write(fd[WRITE], bufferSalida, strlen(bufferSalida));
  }
}

