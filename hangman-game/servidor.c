#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>

#define SERVER_PORT 6543
#define SOCKET_ERROR (-1)
#define NUM_BODY_PIECES 5

char *words[8] = {"computador", "paralela", "distribuida", "concorrencia", "servidor", "cliente", "ciencia", "computacao" };
char *body_pieces[5] = {"Cabeça", "Braços", "Tronco", "Pernas", "Pés"};

int validate(int exp, const char *message);
void handle_connection(int client_socket);
void init_underscored(char* underscored, char* word, int size);

int main() {
  int server_socket, client_socket, client_addr_size;
  struct sockaddr_in server_addr, client_addr;

  validate(server_socket = socket(AF_INET, SOCK_STREAM, 0), "[-]Falha ao criar socket");

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  validate(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)), "[-]Bind falhou");
  
  listen(server_socket, 5);
  printf("[+]Servidor disponível na porta %d\n", SERVER_PORT);

  while (1) {
    client_addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size);
    validate(client_socket, "[-]Falha ao aceitar conexão");
    printf("[+]Servidor conectado!\n");

    handle_connection(client_socket);
  }

  return 0;
}

void handle_connection(int client_socket) {
  printf("[+]Escolhendo palavra...\n");
  srandom(time(NULL));
  char* word = words[random() % 8];
  
  printf("[+]Palavra escolhida: %s\n", word);
  int word_len = strlen(word);
  char underscored[word_len];
  init_underscored(underscored, word, word_len);
  
  int game_piece_count = 0;
  char* game_pieces;
  char ch;

  while (1) {
    write(client_socket, underscored, strlen(underscored));
    read(client_socket, &ch, 1);

    printf("[+] Caractere lido: %s", ch);

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
          char separator[3] = ", ";
          strcat(game_pieces, separator);
          strcat(game_pieces, body_pieces[i]);
        } else {
          strcat(game_pieces, body_pieces[i]);
        }
      }
      write(client_socket, game_pieces, strlen(game_pieces));
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
  
  free(word);

  printf("Fechando conexão");
  close(client_socket);
}

void init_underscored(char* underscored, char* word, int size) {
  for (int i = 0; i < size; i++) {
    if (word[i] == ' ') {
      underscored[i] = ' ';  
    } else {
      underscored[i] = '_';
    }
  }
  underscored[size + 1] = '\0';
}

int validate(int exp, const char *message) {
  if (exp == SOCKET_ERROR) {
    perror(message);
    exit(1);
  }
  return exp;
}