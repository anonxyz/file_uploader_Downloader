#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#define DEFAULT_BUFFLEN 512


int filesize(char *filename)
{
    printf("\nGetting the size of file %s",filename);
    FILE *fp;
    char ch;
    int size;
    fp=fopen(filename,"r");
    fseek(fp,0,2);
    size=ftell(fp);
    fclose(fp);
    return size;

}



int check_if_file_exist(char *filename)
{
    FILE *file;
    if(file=fopen(filename,"r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
int sending_file(char *filename)
{
    printf("Trying to send the file %s",filename);
    int filecheck;
    printf("\nchecking if file exists %s",filename);
    filecheck=check_if_file_exist(filename);
    if(filecheck==0)
    {
        printf("\nfile doesnt exist sorry");
        return 1;
    }
    else if(filecheck==1){

        printf("\nFile does exist\nStarting the file upload process.....\n");

    }
                        //start the server code

WSADATA wsadata;
int iresult;
int sendbytes;
SOCKET listensocket=INVALID_SOCKET;
SOCKET acceptsocket=INVALID_SOCKET;
struct sockaddr_in from;
int fromlen;
struct addrinfo *result=NULL,hints;
char *sendbuf="hello friend";

iresult=WSAStartup(MAKEWORD(2,2),&wsadata);
if(iresult!=0)
{
    printf("wsa startup failed\n");
    return 1;
}
ZeroMemory(&hints,sizeof(hints));
hints.ai_family=AF_INET;
hints.ai_socktype=SOCK_STREAM;
hints.ai_protocol=IPPROTO_TCP;
hints.ai_flags=AI_PASSIVE;

iresult=getaddrinfo(NULL,"2222",&hints,&result);

if(iresult!=0)
{
    printf("getaddr info error %d\n",iresult);
    WSACleanup();
    return 1;
}

listensocket=socket(result->ai_family,result->ai_socktype,result->ai_protocol);

if(listensocket==INVALID_SOCKET)
{
    printf("socket failed with error %d",WSAGetLastError());
    WSACleanup();
    return 1;
}

iresult=bind(listensocket,result->ai_addr,(int)result->ai_addrlen);
if(iresult==SOCKET_ERROR)
{
    printf("Failed to bind\n");
    WSACleanup();
    return 1;
}
iresult = listen(listensocket, SOMAXCONN);
    if (iresult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

acceptsocket=accept(listensocket,(struct sockaddr*)&from, &fromlen);
if(acceptsocket==INVALID_SOCKET)
{
    printf("Accept socket error %d",WSAGetLastError());
    WSACleanup();
    return 1;
}
//start here and write the file sender using a do while loop and count the bytes send and do it until the file size is reached
//Also dont forget to write the incoming connection.
printf("connection received from %s:%d",inet_ntoa(from.sin_addr),htons(from.sin_port));


int fsize=filesize(filename);
printf("\nFinally Sending file %s with a file size of %d\n",filename,fsize);

FILE *fileptr;
fileptr=fopen(filename,"rb");
char buffer[DEFAULT_BUFFLEN];
int sendresult;
while( (sendresult = fread(buffer, 1, sizeof(buffer), fileptr))>0 ){
        sendresult=send(acceptsocket,buffer,sendresult,0);
        printf("%d bytes sent!\n",sendresult);
    }
fclose(fileptr);
WSACleanup();
}


int receive_file(char *address,char *filename) //connect call
{
    printf("Receving a file from %s which will be saved as %s\n",address,filename);
    WSADATA wsadata;
    int iresult;
    SOCKET connectsocket=INVALID_SOCKET;
    struct addrinfo *result=NULL,*ptr=NULL,hints;
    char receive_data[DEFAULT_BUFFLEN];
    iresult=WSAStartup(MAKEWORD(2,2),&wsadata);
    if(iresult!=0)
    {
        printf("wsa data failed");
        return 1;
    }
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;

    iresult=getaddrinfo(address,"2222",&hints,&result);
    if(iresult!=0)
    {
        printf("getaddrinfo FAILED! because %s\n",gai_strerror(iresult));
        WSACleanup();
        return 1;
    }
    for(ptr=result;ptr!=NULL;ptr=ptr->ai_next)
    {
        connectsocket=socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
        if(connectsocket==INVALID_SOCKET)
        {
            printf("socket failed with error %d",WSAGetLastError());
        }
        iresult=connect(connectsocket,ptr->ai_addr,(int)ptr->ai_addrlen);
        if(iresult==SOCKET_ERROR)
        {
            closesocket(connectsocket);
            connectsocket=INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if(connectsocket==INVALID_SOCKET)
    {
        printf("cannot perform connect\n");
    }
    FILE *file;
    file=fopen(filename,"ab");

    do
    {
        iresult=recv(connectsocket,receive_data,DEFAULT_BUFFLEN,0);
        if(iresult>0)
        {
            printf("received bytes %d\n",iresult);
            fwrite(receive_data,iresult,1,file);
        }
    }while(iresult>0);
    closesocket(connectsocket);
    WSACleanup();

}

void print_help()
{
        system("cls");
        printf("\t\tAN UNENCRYPTED FILE UPLOADER/DOWNLOADER \n\nTo receive a file simply type upload_download.exe frecv address_to_RECEIVE_from filename_to_save_as.\n");
        printf("\n\tEx:-upload_download.exe frecv localhost test.pdf\n");
        printf("\n\nTo send a file simply type upload_download.exe fsend filename_to_send\n");
        printf("\n\tEX:-upload_download.exe fsend test.pdf\n");

}

int main(int argc,char *argv[])
{


    if(argc==3)
    {
        if(strcmp(argv[1],"fsend")==0)
            {
                sending_file(argv[2]);
            }
        else
            {
                print_help();
                printf("\t\t\tWrong command specified");
            }
    }
    else if(argc==4)
    {
        if(strcmp(argv[1],"frecv")==0)
            {
                receive_file(argv[2],argv[3]);
            }
        else{
                print_help();
                printf("\t\t\twrong command specified");
            }
    }
    else{
            print_help();

    }


return 1;
}
