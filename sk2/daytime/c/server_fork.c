/* 
Copyright (c) 2006 Michal Kalewski */
/* Permission is hereby granted, free of charge, to any person obtaining a copy of  */
/* this software and associated documentation files (the "Software"), to deal in  */
/* the Software without restriction, including without limitation the rights to  */
/* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies */
/* of the Software, and to permit persons to whom the Software is furnished to do  */
/* so, subject to the following conditions: */
/* The above copyright notice and this permission notice shall be included in all  */
/* copies or substantial portions of the Software. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE. 
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 1234
#define QUEUE_SIZE 5

void childend(int signo)
{
   pid_t pid;
   pid = wait(NULL);
   printf("\t[end of child process number %d]\n", pid);
}

int main(int argc, char* argv[])
{
   int nSocket, nClientSocket;
   int nBind, nListen;
   int nFoo = 1, nTmp;
   struct sockaddr_in stAddr, stClientAddr;

   signal(SIGCHLD, childend);

   /* address structure */
   memset(&stAddr, 0, sizeof(struct sockaddr));
   stAddr.sin_family = AF_INET;
   stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   stAddr.sin_port = htons(SERVER_PORT);

   /* create a socket */
   nSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (nSocket < 0)
   {
       fprintf(stderr, "%s: Can't create a socket.\n", argv[0]);
       exit(1);
   }
   setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

   /* bind a name to a socket */
   nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
   if (nBind < 0)
   {
       fprintf(stderr, "%s: Can't bind a name to a socket.\n", argv[0]);
       exit(1);
   }
   /* specify queue size */
   nListen = listen(nSocket, QUEUE_SIZE);
   if (nListen < 0)
   {
       fprintf(stderr, "%s: Can't set queue size.\n", argv[0]);
   }

   while(1)
   {
       /* block for connection request */
       nTmp = sizeof(struct sockaddr);
       nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);
       if (nClientSocket < 0)
       {
           fprintf(stderr, "%s: Can't create a connection's socket.\n", argv[0]);
           exit(1);
       }

       /* connection */
       if (fork() == 0)
       {
           printf("%s: [connection from %s]\n",
                  argv[0], inet_ntoa((struct in_addr)stClientAddr.sin_addr));
           time_t now;
           struct tm *local;
           time (&now);
           local = localtime(&now);
           char buffer[50];
           int n;
           n = sprintf(buffer, "%s\n", asctime(local));
           write(nClientSocket, buffer, n);
           close(nClientSocket);
           exit(0);
       }
   }

   close(nSocket);
   return(0);
}
