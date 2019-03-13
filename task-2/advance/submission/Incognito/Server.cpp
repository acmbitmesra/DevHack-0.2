#include <bits/stdc++.h>
#include <string.h>
#include <sys/types.h>
#include <winsock2.h>
#include <unistd.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <windows.h>
#include<direct.h>
#include <pthread.h>

const int winsockVersion = 2;
using namespace std;

//#pragma comment(lib,"libws2_32.a") //Winsock Library

string output2(const string full_path,long *file_length)
{
  *file_length=0;
    ifstream input;
	input.open(full_path.c_str(),ios::binary);
	if(input){
	    string str;
	    while(!input.eof()){
          string data;
          getline(input,data);
          str = str + "\n" + data;
          //str = lexical_cast<string>(str)+lexical_cast<string>("\n")+lexical_cast<string>(data);
    }
	*file_length=str.size();
	input.close();
  return str;}
  else
    return "Not Found/Accessible";
  }
  string output(const string full_path,long *file_length)
{
	*file_length=0;
	ifstream input;
	input.open(full_path.c_str());
    if(!input) {
    string str="<html>\n<body>\njames_bond_can_find_anything_not_me.\n</body>\n</html>";
	*file_length=str.size();
    return str;
    }
	if(input){
	    string str_file;
	    while(!input.eof()){
          string data;
          getline(input,data);
          //str_file = lexical_cast<string>(str_file)+lexical_cast<string>("\n")+lexical_cast<string>(data);
          str_file = str_file + "\n" + data;
    }
	*file_length=str_file.size();
	input.close();
    return str_file;}
}
void send_data (SOCKET sock,string str)
{	 int n;
	 n = send(sock,str.c_str(),str.size(),0);
     if (n < 0) std::cerr << strerror(errno) << std::endl;
	 //error("ERROR writing to socket");
}
string read_data (SOCKET sock)
{   string req_type;
   int n,a=4,b=5;
   char buffer[30000];
   //bzero(buffer,30000);
	 memset(buffer,0,sizeof(buffer));
	 n = recv(sock,buffer,29999,0);
     string request;
	 request=buffer;
	 cout<<request;
	 if (n < 0){
	 cout<<"ERROR reading from socket";
	 goto end1;}
     memset(buffer,0,sizeof(buffer));
     req_type=request.substr(0,3);
	 //cout<<req_type<<"*****************\n";
	 if(req_type=="GET")
	 {
		  int i;
		  for(i=4;i<request.size();i++)
		  if(request[i]==' ')
		  break;
			string file=request.substr(4,i-4);
			if(file=="/")
			{file="index.html";
			return file;}
			file=request.substr(5,i-5);
			return file;
	 }
	//end1:
	else
        return "0";
	     end1:
	     a=b;
	 return "0";
}
void *dostuff (void *socket)
{	SOCKET sock=SOCKET(socket);
    while(1){
    int start;
	string content_with_header,file_content,extension,end_line,content_type,file=read_data(sock);
	//cout<<file<<"**************\n";
	long file_length=0;
	if(file=="0")
	{cout<<"File not found...!!!\n"<<endl;
	goto end; }
	int dot=0;
	for(start=0;start<file.size();start++)
	if(file[start]=='.')
	{dot=1;break;}
	 if(dot)
     extension=file.substr(start+1,file.size()-start-1);
	//cout<<extension<<"************\n";
	content_with_header;content_type="text/html";
	//cout<<file_content<<endl;
	 end_line="\n\n";
	 file_content=output(file,&file_length);
	if(extension=="jpg")
	{content_type="image/jpg";
	file_content=output2(file,&file_length);
    end_line="\n";}
	else if(extension=="png")
	{content_type="image/png";
	file_content=output2(file,&file_length);
    end_line="\n";}
	else if(extension=="jpeg")
	{content_type="image/jpeg";
	file_content=output2(file,&file_length);
	 end_line="\n";}
	 else if(extension=="css")
	 {content_type="stylesheet";}
	 else if(extension=="js")
	 {content_type="script";}
	  stringstream int_to_string; int_to_string << file_length; string file_size = int_to_string.str();
	content_with_header = "HTTP/1.1 200 OK\nserver: linuxbox\nContent-Type: " + content_type;
	content_with_header = content_with_header + "\nContent-Length: " + file_size + end_line + file_content;
	//cout<<content_with_header;
	send_data(sock,content_with_header);

}
end:
    closesocket(sock);
}
int main()
{
		 WSADATA wsadata;
		 const char yes='1';
		 if (WSAStartup(MAKEWORD(winsockVersion,0),&wsadata) == 0){
        cout<<"-WSAStartup initialized..." << endl;
		 }
     int sockfd, portno, pid;
     SOCKET newsockfd;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     cout<<"Enter the port : ";
     cin>>portno;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        perror("ERROR opening socket");
        if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
                cout<<"-Couldnt clear blocked port." << endl;
                perror("setsockopt");
                exit(1);
            }
     memset((char *) &serv_addr, sizeof(serv_addr),0);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              perror("ERROR on binding");
     listen(sockfd,5);
     int a=4,b=5,*soc;
     while (1) {
                 clilen = sizeof(cli_addr);
                 try{
         newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         *soc=newsockfd;
         if (newsockfd < 0)
             throw("ERROR on accept");
                 }
                 catch(string str)
                 {  a=b;
                     cout<<str;
                 }
       if(a!=b){
            pthread_t newthread;
           pthread_create(&newthread, NULL,dostuff, (void*)newsockfd);
            }
       }
     close(sockfd);
     return 0;
}
