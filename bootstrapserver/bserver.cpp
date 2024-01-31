#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <vector>

using namespace std;
const int backLog = 3;
const int maxDataSize = 1043;
vector<string> fileserverinfo;

int registration(string s);
int checkvideoservicetype(vector<string> fileserverinfo)
{
	for(int i=0;i<fileserverinfo.size();i++)
	{
      		if(!strcmp(fileserverinfo[i].c_str(),"video"))
      			{
      			return i;
      			}
      	}
      	return 0;		
}
int checktxtservicetype(vector<string> fileserverinfo)
{
	for(int i=0;i<fileserverinfo.size();i++)
      	{	if(!strcmp(fileserverinfo[i].c_str(),"text"))
      			{
      			return i;
      			}
      	}
      	return 0;		
      			
}
int checkimgservicetype(vector<string> fileserverinfo)
{
	for(int i=0;i<fileserverinfo.size();i++)
	{
      		if(!strcmp(fileserverinfo[i].c_str(),"image"))
      			{
      			return i;
      			}
      	}
      	return 0;
}
int checkpdfservicetype(vector<string> fileserverinfo)
{
	for(int i=0;i<fileserverinfo.size();i++)
      	{	if(!strcmp(fileserverinfo[i].c_str(),"pdf"))
      			{
      			return i;
      			}
      			
      	}
      	return 0;
}
int main()
{
   string res;
   uint16_t serverPort = 3002;
   string serverIpAddr = "127.0.0.1";


   int serverSocketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   
   if(!serverSocketFd)
   {
      cout<<"Error creating socket"<<endl;
      exit(1);
   }

   struct sockaddr_in serverSockAddressInfo;
   serverSockAddressInfo.sin_family = PF_INET;
   serverSockAddressInfo.sin_port = htons(serverPort);
   inet_pton(PF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));
   memset(&(serverSockAddressInfo.sin_zero), '\0', 8);
   printf("Server listening on IP %x:PORT %d \n",serverSockAddressInfo.sin_addr.s_addr, serverPort);

   int ret = bind(serverSocketFd, (struct sockaddr *)&serverSockAddressInfo, sizeof(struct sockaddr));
   if(ret<0)
   {
      cout<<"Error binding socket"<<endl;
      close(serverSocketFd);
      exit(1);
   }

 

   socklen_t sinSize = sizeof(struct sockaddr_in);
   int flags = 0;
   int dataRecvd = 0, dataSent = 0;
   struct sockaddr_in clientAddressInfo;
   char rcvDataBuf[maxDataSize], sendDataBuf[maxDataSize];
   string sendDataStr,rcvDataStr;
   memset(&rcvDataBuf, 0, maxDataSize);
 
 int result;
 int i;
while(1)
{
 dataRecvd = recvfrom(serverSocketFd, &rcvDataBuf, maxDataSize, flags, (struct sockaddr *)&clientAddressInfo, &sinSize);
 if(!strcmp(rcvDataBuf, "File Server"))
   {
   int flag=0;
   while(1)
   {
      memset(&rcvDataBuf, 0, maxDataSize);

      dataRecvd = recvfrom(serverSocketFd, &rcvDataBuf, maxDataSize, flags, (struct sockaddr *)&clientAddressInfo, &sinSize);
      rcvDataStr = rcvDataBuf;
      
      if(strcmp(rcvDataBuf, " ")!=0)
      result = registration(rcvDataStr);

      if(!strcmp(rcvDataBuf, "bye"))
      {
         break;
      }

      memset(&sendDataBuf, 0, maxDataSize);
      
      if(result == 1 && flag ==0)
      {
      cout<<"File server registering......"<<endl;
      strcpy(sendDataBuf, "MSGTYPE: REGISTRATION");
      flag =1;
      }
      
      dataSent = sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
      if(!strcmp(sendDataBuf, "bye"))
      {
        
         break;
      }
   }
}
   //cout<<"All done closing server socket now"<<endl;
   //close(serverSocketFd);
else if(!strcmp(rcvDataBuf, "Client"))
{
cout<<"Client Arrived"<<endl;
while(1)
{
memset(&rcvDataBuf, 0, maxDataSize);
int rcs = recvfrom(serverSocketFd, &rcvDataBuf, maxDataSize, flags, (struct sockaddr *)&clientAddressInfo, &sinSize);

if(rcs>0)
{
if(!strcmp(rcvDataBuf, "video"))
{
cout<<"Video Server info requested"<<endl;
if(checkvideoservicetype(fileserverinfo)>0)
{
	int i = checkvideoservicetype(fileserverinfo);
	cout<<"Location is "<<i<<endl;
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+1].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	strcpy(sendDataBuf, fileserverinfo[i+2].c_str());
	
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+3].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, "bye");
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	 break;
	
	
}
}
else if(!strcmp(rcvDataBuf, "text"))
{
cout<<"text Server info requested"<<endl;
if(checktxtservicetype(fileserverinfo)>0)
{
	int i = checktxtservicetype(fileserverinfo);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+1].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+2].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+3].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, "bye");
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	 break;
	
	
}
}
else if(!strcmp(rcvDataBuf, "image"))
{
cout<<"Image Server info requested"<<endl;
if(checkimgservicetype(fileserverinfo)>0)
{
	int i = checkimgservicetype(fileserverinfo);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+1].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+2].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+3].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, "bye");
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	 break;
	
	
}
}
else if(!strcmp(rcvDataBuf, "pdf"))
{
cout<<"Pdf Server info requested"<<endl;
if(checkpdfservicetype(fileserverinfo)>0)
{
	int i = checkpdfservicetype(fileserverinfo);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+1].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+2].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, fileserverinfo[i+3].c_str());
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	memset(&sendDataBuf, 0, maxDataSize);
	strcpy(sendDataBuf, "bye");
	sendto(serverSocketFd, &sendDataBuf, strlen(sendDataBuf), flags, (struct sockaddr *)&clientAddressInfo, sinSize);
	 break;
	
	
	
}
}
else if(!strcmp(rcvDataBuf, "bye"))
      {
        // close(newClientFd);
         break;
      }

}
}
}
}
cout<<"All done closing server socket now"<<endl;
close(serverSocketFd);
}
int registration(string s) 
{
	
        fileserverinfo.push_back(s);
        return 1;
}
