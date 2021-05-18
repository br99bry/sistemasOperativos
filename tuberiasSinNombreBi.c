#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MAX 256
int main () {
    int tuberia_em_re[2];
    int tuberia_re_em[2];
    //int pipe(int fildes [2]);
    //int tuberia [2];
    int pid;
    char mensaje [MAX];
 /*creación de la tuberia sin nombre.*/
    if (pipe (tuberia_em_re)== -1 || pipe (tuberia_re_em) == -1) {
        perror("pipe");
        exit(-1);
    }
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(-1);
    } else if (pid == 0) {
        /*Codigo del proceso hijo.*/
        /*El proceso hijo (receptor) se encarga de leer el mensaje de la tuberia
        y presentarlos en pantalla. El ciclo de lectura y precentacion terminan
        al leer el mensaje "FIN\n".*/
        close(tuberia_re_em [0]);
        close(tuberia_em_re [1]);
        while (read (tuberia_em_re [0], mensaje, MAX) > 0  && strcmp(mensaje, "FIN\n") !=0 ) {
            printf("PROCESO RECEPTOR. MENSAJE: %s\n", mensaje);
            /*Enviamos al proceso emisor un mensaje para indicar que estamos listos para recibir otro mensaje.*/
            strcpy(mensaje, "LISTO");
            write (tuberia_re_em [1], mensaje, strlen(mensaje)+1);
        }
        close(tuberia_em_re [0]);//
        close(tuberia_re_em [1]);//
        exit(0);
 } else {
        /*Codigo del proceso padre.*/
        /*El proceso padre (emisor) se carga de leer mensajes de la entrada estandar y, acto seguido,
        escribirlos en la tuberia para que los reciba el priceso hijo.El ciclo de lectura de la entrada
        estandar y escribir en la tuberia terminara cuando se introduzca el mensaje "FIN\n"*/
        close(tuberia_re_em [1]);
        close(tuberia_em_re [0]);
printf ("PROCESO EMISOR. MENSAJE: ");
        while ( fgets(mensaje, sizeof(mensaje), stdin)!= NULL && write (tuberia_em_re [1], mensaje, strlen(mensaje) + 1) > 0 && strcmp(mensaje, "FIN\n") !=0){
            /*Nos ponemos a esperar el mensaje "LISTO" procedente del proceso receptor*/
            do {
                read(tuberia_re_em [0], mensaje, MAX);
            } while(strcmp(mensaje, "LISTO") !=0);
 printf ("PROCESO EMISOR. MENSAJE: ");
        }

        close(tuberia_em_re [1]); //
        close(tuberia_re_em [0]); //

        exit(0);
    }
}