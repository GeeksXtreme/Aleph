#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int create_listener();
int accept_requests(int, char*);
char *parse_request(char*, char[]);
void handle_connection(char*, int);

int main(int argc, char *argv[])
{
  int socket, socket_desc;
  char* message;

  int port = 8888;

  message = "This was a triumph!\n";
  socket_desc = create_listener(port);
  socket = accept_requests(socket_desc, message);
  if (socket < 0)
  {
    exit(1);
  }

  // Reply to the client

  return 0;
}

int create_listener(int port)
{
  struct sockaddr_in server;
  int socket_desc;
  int yes = 1;
  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }

  // Prepate the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( port );

  // Set Bind options
  if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &yes,
          sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
  }

  // Bind
  if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0 )
  {
    puts("bind failed");
    fflush(stdout);
    return -1;
  }
  puts("bind done");
  fflush(stdout);

  // Listen
  listen(socket_desc, 3);

  return socket_desc;
}

int accept_requests(int socket_desc, char* message)
{
  struct sockaddr_in client;
  int new_socket, c;

  // Accept an incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  while(1)
  {
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if(new_socket < 0)
    {
      perror("accept failed");
      continue;
    }
    puts("connection accepted\n");

    if(!fork())
    {
      handle_connection(message, new_socket);
    }
    close(new_socket);
  }
  return new_socket;
}

void handle_connection(char* connect_message, int socket)
{
  char* json_body;
  char client_message[4000];
  int read_size;

  write(socket, connect_message, strlen(connect_message));

  while( (read_size = recv(socket, client_message, 4000, 0)) > 0 )
  {
    printf("read_size: %i \n", read_size);

    char body[read_size];
    json_body = parse_request(client_message, body);
    printf("the json paylod: %s \n", json_body);

    close(socket);
    exit(0);
  }
  fflush(stdout);
}

char *parse_request(char* client_message, char json_body[])
{
  int message_len = strlen(client_message);
  int found_body = 0;
  int json_body_idx = 0;
  for(int i = 0; i < message_len; i++)
  {
    if(!found_body) {
      if(client_message[i] == '\r' && client_message[i+1] == '\n' && client_message[i+2] == '\r' && client_message[i+3] == '\n'){
        i += 3;
        found_body = 1;
      }
    } else {
      json_body[json_body_idx] = client_message[i];
      json_body_idx++;
    }
  }
  return json_body;
}

