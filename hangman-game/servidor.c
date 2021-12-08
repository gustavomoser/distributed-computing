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
#define NUM_THREADS 5

char *words[8] = {"computador\0", "paralela\0", "distribuida\0", "concorrencia\0", 
  "servidor\0", "cliente\0", "ciencia\0", "computacao\0" };
char *body_pieces[5] = {"cabeca\0", "bracos\0", "tronco\0", "pernas\0", "pes\0"};

int validate(int exp, const char *message);
void* handle_connection(void* client_socket);

int main() {
  int server_socket, client_socket, client_addr_size;
  struct sockaddr_in server_addr, client_addr;

  validate(server_socket = socket(AF_INET, SOCK_STREAM, 0), "[-]Falha ao criar socket");

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVER_PORT);

  validate(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)), "[-]Bind falhou");
  
  listen(server_socket, 5);
  printf("[+]Servidor disponível na porta %d\n", SERVER_PORT);

  pthread_t thread[5];

  for (int i = 0; i < NUM_THREADS; i++) {
    client_addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size);
    validate(client_socket, "[-]Falha ao aceitar conexão");
    printf("[+]Conexao em %d estabelecida!\n", i);

    int* cli_ptr = malloc(sizeof(int));
    *cli_ptr = client_socket;

    printf("[+]Executando thread %d\n", i);
    pthread_create(&thread[i], NULL, handle_connection, cli_ptr);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(thread[i], NULL);
  }

  return 0;
}

void* handle_connection(void* cli_sock) {
  int client_socket = *((int*)cli_sock);
  free(cli_sock);

  printf("[+]Escolhendo palavra...\n");
  srandom(time(NULL));
  char* word = words[random() % 8];
  int word_size = strlen(word);
  
  char underscored[word_size];

  for (int i = 0; i < word_size; i++) {
    underscored[i] = '0';
  }
  
  printf("[+]Palavra escolhida: %s\n", word);

  int cvtd = htonl(word_size);
  send(client_socket, &cvtd, sizeof(cvtd), 0);
  
  printf("[+]Tamanho da palavra: %d\n", word_size);

  char read_ch;
  int game_piece_count = 0;
  char game_pieces[1024];
  int zeros_remaining = word_size;
  int found = 0;
  int ganhou = 0;

  while (1) {
    int v = 0;
    send(client_socket, &underscored, word_size, 0);
    validate(recv(client_socket, &v, 1, 0), "[-]Erro em recv\n");
    
    send(client_socket, &game_pieces, sizeof(char) * 1024, 0);
    validate(recv(client_socket, &v, 1, 0), "[-]Erro em recv\n");
    
    validate(recv(client_socket, &read_ch, sizeof(char), 0), "[-]Erro em recv\n");

    found = 0;
    int used_letter = 0;
    for (int i = 0; i < word_size; i++) {
      char pos = word[i];
      if (pos == read_ch) {
        if (underscored[i] != '0') {
          used_letter++;
        } else {
          found++;
          underscored[i] = pos;
          zeros_remaining--;
        }
      }
    }

    if (found <= 0 && used_letter == 0) {
      game_piece_count++;

      memset(game_pieces, 0, sizeof(game_pieces));
      for(int i = 0; i < game_piece_count; i++) {
        if (i == 0) {
          strcat(game_pieces, body_pieces[i]);
        } else {
          char separator[3] = ", ";
          strcat(game_pieces, separator);
          strcat(game_pieces, body_pieces[i]);
        }
      }
      
      if (game_piece_count == NUM_BODY_PIECES) break;
    }

    if (zeros_remaining <= 0) {
      ganhou = 1;
      break;
    }
  }

  if (ganhou == 1) {
    write(client_socket, "ganhou", sizeof(char) * 7);
  } else {
    write(client_socket, "perdeu", sizeof(char) * 7);
  }

  int v = 0;
  send(client_socket, &game_pieces, sizeof(char) * 1024, 0);
  validate(recv(client_socket, &v, 1, 0), "[-]Erro em recv\n");

  printf("[+]Fechando conexão\n");
  close(client_socket);
  return NULL;
}

int validate(int exp, const char *message) {
  if (exp <= SOCKET_ERROR) {
    perror(message);
    exit(1);
  }
  return exp;
}