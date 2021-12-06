#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 6543
#define SOCKET_ERROR (-1)

int validate(int exp, const char *message);

int main()
{
  int sock, addr_size;
  struct sockaddr_in addr;
  
  validate(sock = socket(AF_INET, SOCK_STREAM, 0), "Falha ao criar socket");
  
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(SERVER_PORT);

  validate(connect(sock, (struct sockaddr*) &addr, sizeof(addr)), "Falha ao conectar");

  puts("Jogo da forca :D");

  while (1) {
    char buff[12];

    validate(recv(sock, buff, 12, 0), "[-]Erro ao ler");
    printf("Palavra: %s\n", buff);
    
    if (buff == "Ganhou" || buff == "Perdeu") {
      break;
    }

    puts("Digite uma letra:");
    char ch = getchar();
    write(sock, &ch, 1);
  }

  close(sock);
  return 0;
}

int validate(int exp, const char *message) {
  if (exp == SOCKET_ERROR) {
    perror(message);
    exit(1);
  }
  return exp;
}