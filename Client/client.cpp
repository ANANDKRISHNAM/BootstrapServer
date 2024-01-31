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
#include <vector>
#define MAXCNTREQ 5 
using namespace std;
const int backLog = 3;
const int maxDataSize = 1043;
string sendDataStr;
vector<string> fileserverinfo;
int fileserversaving(string s);
int requestingfile(vector<string> fileserverinfo);

int receive_data(int socket, char * filename)
{ 
int buffersize = 0, recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat;

char dataarray[10241];
FILE *data_file;

//Find the size of file.
do{
stat = recv(socket, &size, sizeof(int),0);
}while(stat<0);

cout<<"Packet received"<<endl;
cout<<"Packet size: "<<stat<<endl;
cout<<"File size:"<<size<<endl;

data_file = fopen((const char *)filename,  "w");

if(data_file == NULL) 
{
	printf("Error has occurred. File could not be opened\n");
	return -1; 
}

//Loop while we have not received the entire file yet

struct timeval timeout = {10,0};

fd_set fds;
int buffer_fd, buffer_out;

while(recv_size < size) {


    FD_ZERO(&fds);
    FD_SET(socket,&fds);

    buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout);

    if (buffer_fd < 0)
       cout<<"error: bad file descriptor set"<<endl;

    if (buffer_fd == 0)
    cout<<"error: buffer read timeout expired"<<endl;

    if (buffer_fd > 0)
    {
        do{
		read_size = recv(socket,dataarray, 10241, 0);
          }while(read_size <0);
          
	cout<<"Packet number received: "<<packet_index<<endl;
        cout<<"Packet size: "<<read_size<<endl;


        //Write the currently read data into our file
         write_size = fwrite(dataarray,1,read_size, data_file);
         cout<<"Written file size: "<<write_size<<endl;

             if(read_size !=write_size) {
                  cout<<"error in read write"<<endl;    
                 }
             //Increment the total number of bytes read
             recv_size += read_size;
             packet_index++;
             cout<<"Total received File size:"<<recv_size<<endl;
             cout<<" \n";
    }

}


  fclose(data_file);
  cout<<"File successfully Received!\n"<<endl; 
  return 1;
  }
int main()
{

   uint16_t serverPort ;
   string serverIpAddr ;
   cout<<"Enter the ip address and port number of boot strap server"<<endl;
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
   string rcvDataStr;
   memset(&sendDataBuf, 0, maxDataSize);
   strcpy(sendDataBuf, "Client");
   dataSent = sendto(clientSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&serverSockAddressInfo, sinSize );
   cout<<"Enter the file service you want"<<endl;
   cin >> sendDataStr;
   memset(&sendDataBuf, 0, maxDataSize);
  //cin.clear();
  strcpy(sendDataBuf, sendDataStr.c_str());
  dataSent = sendto(clientSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&serverSockAddressInfo, sinSize );
 int flag = 0;
   while(1)
   {
        
      memset(&rcvDataBuf, 0, maxDataSize);
      dataRecvd = recvfrom(clientSocketFd, &rcvDataBuf[0], maxDataSize, flags, (struct sockaddr *)&serverSockAddressInfo, &sinSize); 
      rcvDataStr = rcvDataBuf;
      if(strcmp(rcvDataStr.c_str(),"")!=0 && strcmp(rcvDataBuf, " ")!=0)
      {
      
      if(!strcmp(rcvDataStr.c_str(), "bye"))
      {
 
         break;
      }
      else
      {
      
      flag = 1;
      fileserversaving(rcvDataStr);
      }
   }
   	

   }
   if(flag ==1)
   {
    requestingfile(fileserverinfo);
    cout<<"Receiving files"<<endl;
   }
   cout<<"All done closing socket now"<<endl;
   close(clientSocketFd);
   return 0;
   }
   
   
   int fileserversaving(string s) 
{
	
        fileserverinfo.push_back(s);
        return 1;
}

int requestingfile(vector<string> s)
{
int i; 
  	int socket_desc;
  	struct sockaddr_in server;
  	string acesstoken;
//Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);

  if (socket_desc == -1) 
  {
  cout<<"Could not create socket"<<endl; 
  }

  memset(&server,0,sizeof(server));
  server.sin_addr.s_addr = inet_addr(s[0].c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(stoi(s[1]));

  //Connect to  server 
  cout<<"Waiting to connect...."<<endl;
  for(i=0;i<MAXCNTREQ; i++)
  {
  if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) 
  {
  sleep(2);
  }
  else
  {
  puts("Connected\n");
  break;
  }
  }
  if(i>=MAXCNTREQ)
  {
  cout<<strerror(errno);
  close(socket_desc);
  puts("Connect Error");
  return 1;
  }
  int status =0;
  acesstoken = s[2];
  char sendDataBuf[maxDataSize];
  memset(&sendDataBuf, 0, maxDataSize);
  strcpy(sendDataBuf, acesstoken.c_str());
  send(socket_desc, &sendDataBuf, strlen(sendDataBuf), 0);
  if(!strcmp("text",sendDataStr.c_str()))
  status = receive_data(socket_desc, (char *)"file1.txt");
  else if(!strcmp("pdf",sendDataStr.c_str()))
  status = receive_data(socket_desc, (char *)"ch0.pdf");
  else if(!strcmp("image",sendDataStr.c_str()))
  status = receive_data(socket_desc, (char *)"img.png");
  else if(!strcmp("video",sendDataStr.c_str()))
  status = receive_data(socket_desc, (char *)"simplevdo.mp4");
  return 0;
}



