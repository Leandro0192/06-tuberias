/*
 * Ejercicio 2 de TP PIPE
 * 
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 

#define DATA "INFORMACION IMPORTANTE" // Define variales globales
#define BUFF_SIZE 80			

int main (){

   int ipc[2], proc;			//Define descriptores, variable para usar fork() 
   int leido, success;
   char buff[BUFF_SIZE] = {0};

   if (pipe(ipc) == -1)		//si cuando se llama a pipe retorna -1 (error), se sale del programa
      exit(-1);

   switch (fork()){ 			//llama de fork() e implementación de switch para evaluar distintos casos
      
      case 0:				//Caso 0 es lo que va a realizar el proceso hijo
         close(ipc[0]);		//cierra descriptor de lectura en la tubería
         strncpy(buff, DATA, sizeof(DATA)); //copia en la variable "buff" lo que se declaro en DATA 
         sleep(10);
         write(ipc[1], buff, sizeof(DATA)); // Escribre en la tubería "IFORMACION IMPORTANTE"
         exit(0);
         
      default:    //Proceso Padre
      	close(ipc[1]); //cierro descriptor de escritura 
         printf("Leyendo tuberia... \n");
         leido = read(ipc[0], buff, sizeof(buff));  //lee de la tuberia
         if(leido < 1){				//error si no hay nada escrido
            write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
         }else {

            write (STDOUT_FILENO, "Leido de la tuberia ", sizeof("\nLeido de la tuberia")); //
            write (STDOUT_FILENO, buff, leido-1);			//escribe por pantalla lo que leyo de la tubería
            printf(", por el proceso padre, pid %d \n", getpid());	//identifica al proceso padre
         }
         //wait(NULL);  //espera que termine el proceso hijo, aunque no deberìa ser necesario

         exit(0);
   }
}
