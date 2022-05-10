#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>


int main(void){

int pfd[2];
int nread;
char buf[256];

pipe(pfd); //creo tubería

if (fork()==0){
	close(pfd[0]);	//cierro descriptor de lectura en la tuberìa
	nread=read(0, buf, sizeof buf);
	write(pfd[1], buf, nread);
	//printf("Hijo, escribío en tuberia\n");
	return(0);
	
	}
	
close(pfd[1]);  	//cierro descriptor de escritura en la tuberìa
//printf("Padre, esperando que haya algo en la tuberia para leer...\n");
while((nread = read(pfd[0], buf, sizeof(buf)))> 0){

	write(1, buf, nread);
	
	}
wait(NULL);
return 0;

}
