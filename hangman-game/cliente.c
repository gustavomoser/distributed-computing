#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SERVER_PORT 6543
#define SOCKET_ERROR (-1)

int validate(int exp, const char *message);

int main()
{
  int sock, addr_size;
  struct sockaddr_in addr;
  
  validate(sock = socket(AF_INET, SOCK_STREAM, 0), "Falha ao criar socket");
  
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(SERVER_PORT);

  validate(connect(sock, (struct sockaddr*) &addr, sizeof(addr)), "Falha ao conectar");

  puts("Jogo da forca :D");

  char ch;

  while (1) {
    char buff[20];
    int i = 1;

    int buffr = 0;
    validate(buffr = recv(sock, buff, 12, 0), "[-]Erro ao ler");
    send(sock, &i, 1, 0);

    sprintf(buff, "%.*s", buffr, buff);

    char pieces[1024];
    int piecer = 0;
    validate(piecer = recv(sock, pieces, sizeof(char) * 1024, 0), "[-]Erro ao ler");
    send(sock, &i, 1, 0);

    sprintf(pieces, "%.*s", piecer, pieces);

    if (strcmp("ganhou", buff) == 0|| strcmp("perdeu", buff) == 0) {
      printf("Partes do corpo marcadas na partida: %.*s\n", piecer, pieces);  
      printf("%s\n", buff);
      break;
    }

    printf("Partes do corpo: %.*s\n", piecer, pieces);
    printf("Palavra: %.*s\n", buffr, buff);
    printf("Digite uma letra: ");

    do
      ch = getchar();
    while (isspace(ch));

    send(sock, &ch, sizeof(char), 0);
    printf("\n");
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