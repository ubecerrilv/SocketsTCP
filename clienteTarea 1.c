#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096

/*
Codigo realizado por Ulises Becerril Valdés
V1.0		04/06/2023
*/

int main(int argc, char**argv){
  int c, s, bytes;
  char buf[BUF_SIZE], mensaje[BUF_SIZE];
  struct hostent *h;
  struct sockaddr_in channel;
  
  h = gethostbyname(argv[1]);
  if(!h) fatal("Falla en gethostbyname");
  
  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(s<0) fatal("Socket");
  memset(&channel, 0, sizeof(channel));
  channel.sin_family = AF_INET;
  memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
  channel.sin_port=htons(SERVER_PORT);
  
  //CONECCION ESTABLECIDA
  c = connect(s, (struct sockaddr*)&channel, sizeof(channel));
  if(c<0) fatal("Falla en conexión");
  
  printf("Chat con el servidor, conectado al servidor %s\n", argv[1]);
  while(1){
		//PIDE EL MENSAJE AL USUARIO Y LO MANDA
		printf("Tu: ");
		fgets(mensaje, BUF_SIZE, stdin);
		send(s, mensaje, strlen(mensaje), 0);
		
		//RECIBE LA RESPUESTA DEL SERVIDOR Y LO IMPRIME
		read(s, buf, BUF_SIZE);
		printf("Servidor: %s", buf);
		
		//LIMPIAR EL BUFFER
		memset(buf,0,BUF_SIZE);
  }//FIN WHILE
  
  //CERRAR EL SOCKET
  close(s);
  
}//FIN MAIN

fatal(char *string){
	printf("%s\n", string);
	exit(1);
}//FIN FATAL
