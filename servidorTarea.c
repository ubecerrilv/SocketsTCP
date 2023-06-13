#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10

/*
Codigo realizado por Ulises Becerril Valdés
V1.0		04/06/2023
*/

int main(int argc, char*argv[]){
	int s, b, l, fd, sa, bytes, on =1;
	char buf[BUF_SIZE], respuesta[BUF_SIZE];
	struct sockaddr_in channel;
	
	memset(&channel, 0, sizeof(channel));
	channel.sin_family=AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);
	
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s<0) fatal("Falla en socket");
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	
	b = bind(s, (struct sockaddr*)&channel, sizeof(channel));
	if(b<0) fatal ("Falla en bind");
	l = listen(s, QUEUE_SIZE);
	if (l<0) fatal("Falla en listen");
	
	
	//CONECCION ESTABLECIDA, ESPERA LA PETICION
	printf("Chat con cliente, esperando cliente...\n");
	sa = accept(s,0,0);
	if(sa<0)fatal("Falla en accept");
		printf("Cliente conectado!\n");
		
	while(1){
		//MOSTRAR EL MENSAJE DEL CLIENTE
		read(sa, buf, BUF_SIZE);
		printf("Cliente: %s", buf);
		
		//PEDIR AL USUARIO DEL SERVIDOR QUE ESCRIBA EL MENSAJE A MANDAR
		printf("Tu: ");
		fgets(respuesta, BUF_SIZE, stdin);
		send(sa, respuesta, strlen(respuesta),0);
		
		//LIMPIAR LA BUFFER
		memset(buf, 0, BUF_SIZE);
	}//FIN WHILE
	
		//CERRAR EL SOCKET
		close(sa);
}//FIN MAIN

fatal(char *string){
	printf("%s\n", string);
	exit(1);
}//FIN FATAL
