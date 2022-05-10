/*
 * Ejercicio 3 de TP PIPE
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 

#define FRASE_A "INFORMACION IMPORTANTE A"
#define FRASE_B "INFORMACION IMPORTANTE B"
#define BUFF_SIZE 80

void pipe_sign_handler(int a){   //Define un manejador de Señal 
   
   write (STDOUT_FILENO, "\n Problema con pipeline.\n ", sizeof("\n Problema con pipeline.\n")); //Termina el proceso si hay alguna clase de error
}

int main (){

   int ipc[2], proc;
   int leido, success;
   char buff[BUFF_SIZE] = {0};

   signal(SIGPIPE, pipe_sign_handler);   //Llamando al manejador
   
   pipe(ipc);			//Llamando de la tubería

   switch (fork()){   		//Proceso Padre crea un hijo
      
      case 0:			//Proceso hijo
      close(ipc[0]);      	//cierra descriptor de lectura
      strncpy(buff, FRASE_A, sizeof(FRASE_A));  //Coopia informacìon al buff
      write(ipc[1], buff, sizeof(FRASE_A));	//Escribe en la tubería
      exit(0);
      break;
      
      default:
      switch (fork()){  //Proceso Padre llama crea otro hijo
            
         case 0:     //Proceso del otro hijo
            close(ipc[0]);     //cierra descriptor de lectura           
            strncpy(buff, FRASE_B, sizeof(FRASE_B));  //copia información al buff
            write(ipc[1], buff,    sizeof(FRASE_B));	//escribe en la tubería
            exit(0);      
         break;
         default: //Proceso Padre sigue...
            close(ipc[1]);   //Cierra descripor de escritura
            int i;
            for(i=0; i<2; i++){  
               leido = read(ipc[0], buff, sizeof(buff));  //Realiza dos lecturas del mismo tamaño
               if(leido < 1){
                  write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia")); //error si no hay nada escrito
               }else {
                  write (STDOUT_FILENO, "Leido de la tuberia ", sizeof("\nLeido de la tuberia"));
                  write (STDOUT_FILENO, buff, leido-1);  //Muestra por pantalla lo que leyò
                  printf(", por el proceso padre, pid %d \n", getpid());
               }
            }

            close(ipc[0]);
            wait(NULL);
            wait(NULL);
            exit(0);
         break;
      }
   }   
}
