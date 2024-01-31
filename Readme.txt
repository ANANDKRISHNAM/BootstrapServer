Assignment 3 - Socket Programming (BootStrapServer, FileServers & File Transfer) 
========================


Build
-----------------------
BootStrapServer: g++ bserver.cpp -o server
Client: g++ client.cpp -o client

Build FileServers(Text, image , pdf and video) in their respective folder
g++  fileserver.cpp -o fserver

Execution
-----------------------
Execute the bootstrap server first using ./server. 
The program, by default, runs on localhost.

Registering File server
 
Execute the FileServer by ./fserver.

Enter 1 to register File server in Bootstrap server
 
Enter the IP address 127.0.0.1 and port number 3002 of Bootstrap sever process in FileServer terminal.

Now, Enter the file service, information to be added in Boostrap registry (servicename servicetype IPaddress Portnumber  serviceaccesstoken ).
	for TextFileServer, Enter "<Anyservername> text 127.0.0.1 5000 abcd"
	for ImageFileServer, Enter "<Anyservername> image 127.0.0.1 5001 1234"
	for PdfFileServer, Enter "<Anyservername> pdf 127.0.0.1 5002 qwerty"
	for VideoFileServer, Enter "<Anyservername> video 127.0.0.1 5003 asdf"

Now, FileServer terminal, You will receive "MSGTYPE: REGISTRN" if it was a succesful registration

Now Enter "bye" in FileServer terminal to close.

Now, Execute the client by ./client in different terminal

Enter the IP address 127.0.0.1  and port number 3002 of Bootstrap sever process in Client terminal.

[Parallely, Execute the FileServer again by ./fserver in different terminal 

Enter 2 to serve the TCP clients.]

Now, Enter the file service you want, that is "text" or "image" or "pdf" or "video" in Client terminal.

[Client will receive the Fileserver info from Bootstrap server.(servicename servicetype IP address Port number  service access token )]

Now, Client gets connected to Fileserver terminal after verifying service access token and File transfer starts,
progress can be seen in client as well as fileserver terminal.

Similarly multiple clients can be opened and connected to Filserver


To confirm if the file is completely transferred, check the file size of both send and received. If the transfer completed
correctly, the file sizes should be same and it can be verified by viweing them.


Note: MAXCLIENTS to be served are set to 5 in all file server programs. 
While File server registering for each type of files, the values Servicetype, IP address, portnumber and accesstoken are hardcoded. Do not give any other values instead.

Plagarism Statement
------------------------
I certify that this assignment/report is my own work, based on my personal study and/or
research and that I have acknowledged all material and sources used in its preparation, whether
they be books, articles, reports, lecture notes, and any other kind of document, electronic or
personal communication. I also certify that this assignment/report has not previously been
submitted for assessment in any other course, except where specific permission has been
granted from all course instructors involved, or at any other time in this course, and that I have
not copied in part or whole or otherwise plagiarized the work of other students and/or persons. I
pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I
understand my responsibility to report honor violations by other students if I become aware of it.
Name of the student : M Anand Krishna
Roll No: CS22MTECH14003

