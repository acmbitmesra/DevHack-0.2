/*
ITS a basic Server Application that uses Multithreading to handle multiple connections
Ensure that the build environment links to the Winsock Library file Ws2_32.lib. Applications that use Winsock must be linked with the Ws2_32.lib library file
*/

/*addrinfo is defined in windows XP and Windows Server 2003
THe direct.h header file contains header file contains definitions for file tranfer into the program loading fiels as binaries
The Winsock2.h header file contains most of the Winsock functions, structures, and definitions.
The Ws2tcpip.h header file contains definitions introduced in the WinSock 2 Protocol-Specific Annex document for TCP/IP that includes newer functions and structures used to retrieve IP addresses
*/
#define _WIN32_WINNT 0x501
#include<stdio.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<direct.h>
#include<pthread.h>

#define DEFAULT_PORT "8888"
#define BUFSIZE 8096
#define LOG_BUFSIZE (BUFSIZE<<1)
struct
{
    char   *ext;
    char   *filetype;
}
extensions[] =
{
    {"ico", "image/ico"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"css", "text/css"},
    {"js", "text/js"},
    {0, 0}
};

//BASIC HTTP Request handler. can handle only GET requests.
void *web(void* cl_Socket)
{
    SOCKET clientSocket = (SOCKET)cl_Socket;
    FILE   *filePointer;
    int     j, buflen, len;
    long    idx, nbytes;
    char   *file_type;
    int z=0,x=0;

    //Infinitely listening to request for a socket unless any error occur or the client requests to close the socket
while(1){

     char buffer[BUFSIZE + 1]="";    // buffer to receive and send out messages.

    /* read the web browser request through the TCP connection */
    nbytes = recv(clientSocket, buffer, BUFSIZE, 0);

    /* check and see if we do get a request from the browser, */
    if (nbytes > 0)
    {
        //terminate the input string.
        buffer[nbytes] = 0;  /* conclude the buffer */

    /* remove CF and LF characters and replacing them with '*' */
    for (idx = 0; idx < nbytes; idx++)
    {
        if (buffer[idx] == '\r' || buffer[idx] == '\n')
        {
            buffer[idx] = '*';
        }
    }
    //check that the request is GET
    if (strncmp(buffer, "GET ", 4) && strncmp(buffer, "get ", 4))
    {
        printf("Only simple GET method is supported\n");
    }
    else{

    /* null-terminate after the second space to ignore extra stuff */
    for (idx = 4; idx < BUFSIZE; idx++)
    {
        if (buffer[idx] == ' ')
        {
            /* string is "GET URL " +lots of other stuff */
            buffer[idx] = 0;
            break;
        }
    }

    /* check for illegal parent directory use .. */
    for (j = 0; j < idx - 1; j++)
    {
        if (buffer[j] == '.' && buffer[j+1] == '.')
        {
            printf("Parent directory (..) path names not supported\n");
            z=1;
        }
    }
if(z!=1){
    /* convert no filename to index file */
    if (!strncmp(&buffer[0], "GET /\0", 6) || !strncmp(&buffer[0], "get /\0", 6))
    {
        strncpy(buffer, "GET /index.html", BUFSIZE);
    }
    printf("%s\n",buffer);

    /* work out the file type and check if we support it */
    buflen = strlen(buffer);
    file_type = (char *) 0;
    for (idx = 0; extensions[idx].ext != 0; idx++)
    {
        len = strlen(extensions[idx].ext);
        if (!strncmp(&buffer[buflen - len], extensions[idx].ext, len))
        {
            file_type = extensions[idx].filetype;
            break;
        }
    }
    if (file_type == 0)
    {
        printf("file type not supported\n");
        //sprintf-Instead of printing on console, it store output on char buffer which are specified in sprintf
        //metaData to be include with the message to comply with the HTTP Protocol of Data Exchange through WEB.
        sprintf(buffer, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: 70\r\n\r\n", file_type);

        //send a message to the client.
        send(clientSocket, buffer, strlen(buffer), 0);
        sprintf(buffer,
                "<HTML><BODY><H2>Web Server: file type not supported </H2></BODY></HTML>\r\n");
            send(clientSocket, buffer, strlen(buffer), 0);
    }
    else{
    /* execute the send command, but first check if the browser */
    /* has requested the default icon file, favicon.ico.        */
    if (!strncmp(buffer+5, "favicon.ico", 12))
    {
        printf("handling favicon request\n");
        if ((filePointer = fopen(&buffer[5], "rb")) == NULL)
        {
        /* Send a null file to satisfy a request for favicon.ico  if favicon is not present*/
        sprintf(buffer, "HTTP/1.1 204 NO CONTENT\r\nContent-Length: 0\r\n\r\n");
        send(clientSocket, buffer, strlen(buffer), 0);
    }
    }
    else
    {
        /* open the file for reading */
        if ((filePointer = fopen(&buffer[5], "rb")) == NULL)
        {
            printf("file not found\n");
            sprintf(buffer, "HTTP/1.1 404 File not found\r\nContent-Type: %s\r\nContent-Length: 67\r\n\r\n", file_type);
            send(clientSocket, buffer, strlen(buffer), 0);
            sprintf(buffer,
                "<HTML><BODY><H2>Web Server: failed to open file</H2></BODY></HTML>\r\n");
            send(clientSocket, buffer, strlen(buffer), 0);

        }
        else{

        //find out the Content length
        fseek(filePointer, 0L, SEEK_END);   //reach to end of file
        int sz = ftell(filePointer);    //filePointer store the byte of the file is on
        rewind(filePointer);    //return to pointer to the start of the file Content

        sprintf(buffer, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", file_type, sz);
        send(clientSocket, buffer, strlen(buffer), 0);

        /* send file in 8KB block - last block may be smaller */
        while ((nbytes = fread(buffer, 1, BUFSIZE, filePointer)) > 0)
        {
            send(clientSocket, buffer, nbytes, 0);
        }
        }
    }
    }
    }
    }
    }
    else if(nbytes==0){
            //x++;
           // if(x==2){
            printf("Connection Closing ....\n");
            closesocket(clientSocket);
            break;
    //}
    }

        else{
            printf("Recv failed %d \n",WSAGetLastError());
            closesocket(clientSocket);
            break;
        }
}
}
int main(){
    //Create a WSADATA object called wsaData.
    WSADATA wsaData;

    int iresult;
    //The WSAStartup function is called to initiate use of WS2_32.dll.
    /*
    MAKEWORD(2,2) parameter of WSAStartup makes a request for version 2.2 of Winsock on the system, and sets the passed version as the highest version of Windows Sockets support that the caller can use.
    */
    iresult=WSAStartup(MAKEWORD(2,2),&wsaData);

    if(iresult!=0)
    {
         printf("WSAStartup failed with error: %d\n", iresult);
        return 1;
    }
    //The addrinfo structure is used by the getaddrinfo function to hold host address information.
    struct addrinfo *result=NULL,hints;
    //ZeroMemory- Fills a block of memory with zeros.
    ZeroMemory(&hints, sizeof(hints));

    // hint-->A pointer to an addrinfo structure that provides hints about the type of socket the caller supports.
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    //A null Terminated ANSI pointer to the string containing the port number at which  the server is to listen to requests.
    char *portno=(char*)malloc(7);
    printf("Enter the PORT NO to host the server\n");
    scanf("%6s",portno);

    // Resolve the server address and port
    iresult=getaddrinfo(NULL,portno,&hints,&result);//The IP address portion of the socket address structure is set to INADDR_ANY for IPv4 addresses
    if(iresult!=0)
    {
        //if any error occur then terminates use of the Winsock2 DLL (Ws2_32.dll). and exit...
        printf("getaddrinfo failed with error: %d\n", iresult);
        WSACleanup();
        return 1;
    }
    SOCKET listenSocket =INVALID_SOCKET;
     // Create a SOCKET for connecting to server
    listenSocket= socket(result->ai_family, result->ai_socktype,result->ai_protocol);

    if(listenSocket==INVALID_SOCKET)
    {
        //WSAGetLastError sends back the last socket error recived by the system
        printf("socket failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);//frees address information that the getaddrinfo function dynamically allocates in addrinfo structures.
        WSACleanup();//termites the use of Winsock Api
        return 1;
    }
     // Setup the TCP listening socket
    iresult=bind(listenSocket,result->ai_addr,(int)result->ai_addrlen);

    if(iresult==SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);


    // Sets the Socket to listening mode.
    if(listen(listenSocket,SOMAXCONN)==SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("\n=========================================================\n");
    printf("           SERVER STARTED \n");
    printf("===========================================================\n\n");

    SOCKET clientSocket =INVALID_SOCKET;
    struct sockaddr_in from;
    int fromlen;
    //Infinitely listening for client connections.
    while((clientSocket=accept(listenSocket,(SOCKADDR*)&from,(int*)&fromlen)))//Accept a client socket
{

     if(clientSocket==INVALID_SOCKET)
    {
        printf("accept failed with error: %d\n", WSAGetLastError());//print out the error
        closesocket(listenSocket);//close listening socket
        WSACleanup();//terminate the use of Winsock API
        return 1;
    }
       printf("New connection ip is : %s , port : %d \n" , inet_ntoa(from.sin_addr) , ntohs(from.sin_port));//prinst out the IP_address of the User and PORT assigned to it on the server side.
       pthread_t thread_id;
       //create a thread for a newly connected client and pass its request to the worker thread which infinitely receives webpage file request and sens the file until the client terminated the connections
      pthread_create(&thread_id, NULL,web, (void*)clientSocket);
}
}

