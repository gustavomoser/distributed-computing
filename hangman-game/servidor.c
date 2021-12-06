#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>

#define SERVER_PORT 6543
#define SOCKET_ERROR (-1)
#define NUM_BODY_PIECES 5
#define ALL_BODY_PIECES 40

char *words[8] = {"computador", "paralela", "distribuida", "concorrencia", "servidor", "cliente", "ciencia", "computacao" };
char *body_pieces[5] = {"Cabeça", "Braços", "Tronco", "Pernas", "Pés"};

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int validate(int exp, const char *message);
void* handle_connection(void* p_client_socket);

int main(int argc, char **argv) {
  int server_socket, client_socket, addr_size;
  SA_IN server_addr, client_addr;

  validate(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Falha ao criar socket");

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  validate(bind(server_socket, (SA*) &server_addr, sizeof(server_addr)), "Bind falhou");
  validate(listen(server_socket, 1), "Falha na escuta");

  while (1) {
    printf("Aguardando conexões...\n");
    addr_size = sizeof(SA_IN);

    validate(client_socket = accept(server_socket, (SA*) &client_addr, (socklen_t*) &addr_size), "Falha ao aceitar conexão");
    printf("Conectado\n");
    
    pthread_t thread;
    int *p_client = malloc(sizeof(int));
    *p_client = client_socket;

    pthread_create(&thread, NULL, handle_connection, p_client);
  }

  return 0;
}

void* handle_connection(void* p_client_socket) {
  int client_socket = *((int*)p_client_socket);
  free(p_client_socket);

  printf("Escolhendo palavra para iniciar o jogo\n");
  char *word = words[rand() % 9];

  printf("A palavra escolhida é: %s\n", word);
  int word_len = strlen(word);
  char word_len_str = word_len;
  char underscored[word_len];
      
  for (int i = 0; i < word_len; i++) {
    char str = word[i];
    if (isspace(str)) {
      underscored[i] = ' ';  
    } else {
      underscored[i] = '_';
    }
  }
  
  int accept = 0;
  char game_piece_count = 0;
  char game_pieces = malloc(35 * sizeof(char));
  
  char *word_size = "Numero de letras: ";
  strcat(word_size, &word_len_str);
  write(client_socket, word_size, strlen(word_size));

  while (accept == 0) {
    write(client_socket, underscored, strlen(underscored));
    char ch;
    read(client_socket, &ch, 1);

    int found = 0;
    for (int i = 0; i < word_len; i++) {
      char pos = word[i];
      if (pos == ch) {
        found++;
        underscored[i] = pos;
      }
    }

    if (found <= 0) {
      game_piece_count++;
      if (game_piece_count == NUM_BODY_PIECES) {
        write(client_socket, "Perdeu", strlen("Perdeu"));
        break;
      }

      for(int i = 0; i < game_piece_count; i++) {
        if (game_piece_count > 1 && game_piece_count < 5) {
          char *separator = ", ";
          strcat(&game_pieces, separator);
          strcat(&game_pieces, body_pieces[i]);
        } else {
          strcat(&game_pieces, body_pieces[i]);
        }
      }
      write(client_socket, &game_pieces, strlen(&game_pieces));
    }

    int underscore_remaining = 0;
    for (int i = 0; i < word_len; i++) {
      char pos = underscored[i];
      if (pos == '_') {
        underscore_remaining++;
      }
    }

    if (underscore_remaining <= 0) {
      write(client_socket, "Ganhou", strlen("Ganhou"));
      break;
    }
  }

  printf("Fechando conexão");
  close(client_socket);
  return NULL;
}

int validate(int exp, const char *message) {
  if (exp == SOCKET_ERROR) {
    perror(message);
    exit(1);
  }
  return exp;
}