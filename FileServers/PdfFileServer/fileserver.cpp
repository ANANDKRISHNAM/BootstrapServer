#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define MAXCLIENTS 5

// Child process id
pid_t childpid;

using namespace std;
const int backLog = 3;
const int maxDataSize = 1043;
int reginBS();
int servingclient();
int senddata(int socket, const char * filedata){

   FILE *files;
   int size, read_size, stat, packet_index;
   char send_buffer[10240], read_buffer[256];
   packet_index = 1;

   files = fopen(filedata, "r");
   cout<<"Getting file Size"<<endl;

   if(files == NULL) 
   {
	cout<<"Error Opening File"<<endl;
   } 

   fseek(files, 0, SEEK_END);
   size = ftell(files);
   fseek(files, 0, SEEK_SET);
   cout<<"Total file size: "<< size <<endl;
   
   //Send File Size
   
   cout<<"Sending file Size\n";
   send(socket, (void *)&size, sizeof(int), 0);

   //Send File as Byte Array
   
   cout<<"Sending File as Byte Array\n";

   while(!feof(files)) 
   {
   //Read from the file into our send buffer
      	read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, files);
      //Send data through our socket 
      do{
        stat = send(socket, send_buffer, read_size, 0);  
      }while (stat < 0);

      
      cout<<"Packet Number: \n"<< packet_index <<endl;
      cout<<"Packet Size Sent: \n"<< read_size <<endl;    
      
      packet_index++;  

      //Empty out our send buffer
      bzero(send_buffer, sizeof(send_buffer));
     }
     return 0;
    }

int main()
{
int service = 0;
cout<<"Enter 1 to register in Bootstrap server or 2 to serve TCP clients "<<endl;
cin>>service;
if(service==1)
reginBS();
else if(service==2)
servingclient();
}
int reginBS()
{

   uint16_t serverPort ;
   string serverIpAddr ;
   cout<<"Enter the ip address and port number of server"<<endl;
   cin>>serverIpAddr;
   cin>>serverPort;

   int clientSocketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(!clientSocketFd)
   {
      cout<<"Error creating socket"<<endl;
      exit(1);
   }

   struct sockaddr_in serverSockAddressInfo;
   serverSockAddressInfo.sin_family = PF_INET;
   serverSockAddressInfo.sin_port = htons(serverPort);
   
   inet_pton(AF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));

   memset(&(serverSockAddressInfo.sin_zero), '\0', 8);

   socklen_t sinSize = sizeof(struct sockaddr_in);
   int flags = 0;
   int dataRecvd = 0, dataSent = 0;
   struct sockaddr_in clientAddressInfo;
   char rcvDataBuf[maxDataSize], sendDataBuf[maxDataSize];
   string sendDataStr, rcvDataStr;

   memset(&sendDataBuf, 0, maxDataSize);
   strcpy(sendDataBuf, "File Server");
   dataSent = sendto(clientSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&serverSockAddressInfo, sinSize );
   cout<<"Enter the file service informations to be added"<<endl;

   while(1)
   {
      
      cin >> sendDataStr;
      memset(&sendDataBuf, 0, maxDataSize);
      cin.clear();
      strcpy(sendDataBuf, sendDataStr.c_str());
      dataSent = sendto(clientSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&serverSockAddressInfo, sinSize );
     	sendDataStr = sendDataBuf;
      if(!strcmp(sendDataStr.c_str(), "bye"))
      {
         break;
      }

      memset(&rcvDataBuf, 0, maxDataSize);
      dataRecvd = recvfrom(clientSocketFd, &rcvDataBuf[0], maxDataSize, flags, (struct sockaddr *)&serverSockAddressInfo, &sinSize); 
      rcvDataStr = rcvDataBuf;
      if(strcmp(rcvDataStr.c_str(),"")!=0)
      cout<< rcvDataStr <<endl;
      if(!strcmp(rcvDataStr.c_str(), "bye"))
      {
         break;
      }
   }

 
   cout<<"All done closing socket now"<<endl;
   close(clientSocketFd);
   return 0;
}

int servingclient()
{

int socket_desc , new_socket , c, read_size,buffer = 0;
      struct sockaddr_in server , client;
      char *readin;

      //Create socket
      socket_desc = socket(AF_INET , SOCK_STREAM , 0);
      if (socket_desc == -1)
      {
         cout<<"Could not create socket";
      }

      //Prepare the sockaddr_in structure
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;
      server.sin_port = htons( 5002 );

      //Bind
     if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
     {
       puts("bind failed");
       return 1;
     }
     puts("bind done");

     //Listen
     
     	if(listen(socket_desc, MAXCLIENTS) == -1)
	{
	cout<<"Failed to listen\n";
	return -1;
}	

      puts("Waiting for incoming connections...");
      int clientcount = 0;
      while(1)
 {
      
      c = sizeof(struct sockaddr_in);
      
      if((new_socket = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&c))&&(clientcount<MAXCLIENTS))
      {
      
	puts("Connection accepted");
	
	if ((childpid = fork()) == 0) 
        {
        char rcvDataBuf[maxDataSize];
   	memset(&rcvDataBuf, 0, maxDataSize);
        close(socket_desc);
	if(recv(new_socket, &rcvDataBuf, maxDataSize, 0)>0)
	{
	if(!strcmp(rcvDataBuf,"qwerty"))
	senddata(new_socket, "ch0.pdf");
	else
	cout<<"Invalid token"<<endl;
	exit(0);
    	}		
   }
        clientcount++;
   	close(new_socket);
    	fflush(stdout);
    	while(clientcount)
    	{
    	childpid = waitpid((pid_t)-1, NULL, WNOHANG);
    	if(childpid<0)
    	{
    	cout<< "Die with error"<<endl;
    	exit(1);
    	}
    	else if(childpid == 0)
    	{
    	break;
    	}
    	else
    	clientcount--;
    	}
	}

if(new_socket < 0)
  perror("Accept Failed");
	}

 fflush(stdout);
    return 0;
}


