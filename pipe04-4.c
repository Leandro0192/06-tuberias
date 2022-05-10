/*
 * Ejercicio 4 de TP PIPE
*/

/*Así como esta no funcionaría porque el descriptor de lectura 
está cerrado para el proceso hijo, porque fue heredado del padre,
Solucionado: cerrar el descriptor de lectura dentro de default así no lo hereda el hijo*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 

#define DATA "INFORMACION IMPORTANTE"
#define BUFF_SIZE 80

void pipe_sign_handler(int a){ // Definición del manjenador
   
   write (STDOUT_FILENO, "\n Problema con pipeline.\n ", sizeof("\n Problema con pipeline.\n"));
   exit(-1);
}

int main (){

   int ipc[2], proc;
   int leido, success;
   char buff[BUFF_SIZE] = {0};

   signal(SIGPIPE, pipe_sign_handler); // Llamado del manejador
   
   if (pipe(ipc) == -1) //Comprueba si hay error al crear la tubería
      exit(-1);
   
   
   
   switch (fork()){    //Crea proceso hijo
      
      case 0:  //Proceso hijo
         close(ipc[1]); //cierra descriptor de escritura en el proceso hijo
         printf("Leyendo tuberia... \n");
         leido = read(ipc[0], buff, sizeof(buff));  //Lee lo que hay en la tuberiía
         if(leido < 1){ //detecta si la tuberìa no tiene ningún caracter
            write(STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
         }else {
            write(STDOUT_FILENO, "Leido de la tuberia ", sizeof("\nLeido de la tuberia"));
            write(STDOUT_FILENO, buff, leido-1); // Escribe en pantalla lo que tenía la tubería
            printf(", por el proceso hijo, pid %d \n", getpid());  //especifíca el proceso del hijo
         }
         exit(0);
         
      default: 					//Proceso Padre
      	 close(ipc[0]); 				//cierra descriptor de lectura de la tubería en el proceso padre
         strncpy(buff, DATA, sizeof(DATA));		//copia DATA en buff 
         write(ipc[1], buff, sizeof(DATA));		// escribe en la tubería el contenido de buff 
      
         wait(NULL);      			//espera que el proceso hijo concluya, es decir espera que lea de la tubería
         
         exit(0);      
   }
}


