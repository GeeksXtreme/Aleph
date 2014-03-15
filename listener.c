#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int create_listener();
char *parse_request(char*, char[]);
void handle_connection(char*, int);

int main(int argc, char *argv[])
{
  int socket;
  char* message;

  socket = create_listener();
  if (socket < 0)
  {
    exit(1);
  }

  // Reply to the client
  message = "This was a triumph!\n";
  handle_connection(message, socket);

  return 0;
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
    /* close(new_socket); */
    char body[read_size];
    json_body = parse_request(client_message, body);
    printf("the json paylod: %s", json_body);
    close(socket);
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

int create_listener()
{
  struct sockaddr_in server, client;
  int new_socket, c, socket_desc;
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
  server.sin_port = htons( 8888 );

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

  // Accept an incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);
  new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
  if(new_socket < 0)
  {
    perror("accept failed");
  }
  puts("connection accepted");

  return new_socket;
}
