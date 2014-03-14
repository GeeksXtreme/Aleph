#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int main(int argc, char *argv[])
{
  char json_body[4000];
  int socket_desc, new_socket, c, read_size;
  int yes = 1;
  char* message, client_message[4000];
  struct sockaddr_in server, client;

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
    return 1;
  }
  puts("bind done");

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

  // Reply to the client
  message = "This was a triumph!\n";
  write(new_socket, message, strlen(message));

  while( (read_size = recv(new_socket, client_message, 4000, 0)) > 0 )
  {
    /* close(new_socket); */
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
    printf("the json paylod: %s", json_body);
    close(new_socket);
  }

  return 0;
}
