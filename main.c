#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 6666

char bool = 0;

int server_fd = 0; 

struct metadado
{
  size_t len;
  unsigned char *name;
};

int *p2p_clients = NULL;


void
metadados_signal(struct metadado *meta)
{
  unsigned char b = 0;
  while(b != 0x2)
  {
    recv(server_fd, &b, sizeof(unsigned char), 0);
  }
  b = 0;
  unsigned char *buffer = calloc(1, sizeof(unsigned char));
  size_t index = 0;
  char comma = 0;
  while (recv(server_fd, &b, sizeof(unsigned char), 0) >= 0)
  {
    if(b !=',')
    { 
      buffer[index] = b;
      index++;
      buffer = realloc(buffer, (index + 1)* sizeof(unsigned char));
      continue;
    }

    buffer[index] = '\0';
    meta->len = atoi((const char *)buffer);
    memset(buffer, 0, index);
    index = 0;
    buffer = realloc(buffer, (index + 1) * sizeof(unsigned char));
  }
  meta->name = calloc(index, sizeof(unsigned char));
  strncpy((char *)meta->name, (const char *)buffer, index);
}

int
main(int argc, char **argv)
{ 
  if(argc < 2)
  {
    fprintf(stderr, "missing argments ./a.out name_thread\n");
    return  -1;
  }

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if(server_fd == -1)
  {
    fprintf(stderr, "error create socket\n");
    return -1;
  }
  
  struct sockaddr_in sock_adress;
  sock_adress.sin_addr.s_addr = INADDR_ANY;
  sock_adress.sin_family = AF_INET;
  sock_adress.sin_port = htons(PORT);
  socklen_t sock_len = sizeof(sock_adress);
  if(bind(server_fd, (struct sockaddr *)&sock_adress, sock_len) < 0)
  {
    fprintf(stderr, "error create socket\n");
    return -1;
  }



  return 0;
}
  
