/*******************************************************************************
* Title                 :   P2P Project
* Filename              :   peer.c
* Author                :   Thomas Pazhaidam, Mohib Khan, Edwiwn Chen
* Origin Date           :   11/11/2023
* Version               :   0.0.1
* Notes                 :   None
*******************************************************************************/
/** \file peer.c
 * \brief This module contains functionality of peers for a peer to peer application
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat. h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define BUFFLEN 100
#define PACKETSIZE 101
/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct PDU {
    char type;
    char data[BUFFLEN]
};
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
enum MsgType {
    REGISTER = 114, DOWNLOAD = 100, SEARCH = 115, DEREGISTER = 116, DATA = 99,
    ONLINE = 111, ACK = 97, ERR = 101
};
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Request user to specify peer name from terminal
*******************************************************************************/
void GetPeerName(char *PeerName)
{
    int outLen = 0;
    memset(&PeerName, 0, sizeof(PeerName));
    printf("Specify peer name (max 10 characters): \n");
    outLen = read(0, PeerName, sizeof(peerName));
    PeerName[outLen-1] = 0;
}

/******************************************************************************
* Request list of online content on server
*******************************************************************************/
void RequestOnlineContent(int Socket)
{
    struct PDU msg, resp;
    memset(&msg, 0, sizeof(msg));
    memset(&resp, 0, sizeof(resp));
    msg.type = ONLINE;

    write(Socket, &msg, sizeof(msg));
    read(Socket, &resp, sizeof(resp));

    printf("Content available: \n");
    printf("%s\n", resp.data);
}

/******************************************************************************
* Initialize TCP Connection and returns tcp socket and server response
*******************************************************************************/
int InitializeTCPServer(int UdpSocket, char* PeerName, char* Filename, struct PDU Response)
{
    // Make tcp connection
	int 	tcp_socket, alen;
	struct	sockaddr_in server;
	struct 	PDU msg;
	char 	tcpAddr[15], tcpPort[6], send_data[100];

    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "Can't create a socket\n");
    exit(1);
	}

    alen = sizeof(struct sockaddr_in);
	getsockname(tcp_socket, (struct sockaddr*)&server, &alen);

    sprintf(tcpAddr, "%d", server.sin_addr.s_addr);
    sprintf(tcpPort, "%d", server.sin_port);

    memset(&msg, 0, sizeof(msg));

    msg.type = REGISTER;

    char data[BUFFLEN] = {0};
    strcpy(data, PeerName);
    strcat(data, ";");
    strcat(data, Filename);
    strcat(data, ";");
    strcat(data, tcpAddr);
    strcat(data, ";");
    strcat(data, tcpPort);
    strcpy(msg.data, data);

    write(UdpSocket, &msg, sizeof(msg));
    read(UdpSocket, &Response, sizeof(Response));

    return tcp_socket;
}

/******************************************************************************
* Register new content
*******************************************************************************/
void RegisterContent(int Socket, char* PeerName)
{
    struct PDU msg, resp;
    memset(&resp, 0, sizeof(resp));

    printf("File name to register: ");

    char Filename[25] = {0};
    
    int outLen = 0;
    outLen = read(0, Filename, sizeof(Filename));
    input[outLen-1] = 0;

    struct stat info;
    if(stat(Filename, &info) != 0)
    {
        printf("Unable to find file. \n");
        return;
    }

    int tcpSocket = InitializeTCPServer(Socket, PeerName, Filename, &resp);

    if(resp.type == (char)ERR)
    {
        printf("Can't register data. \n");
    }
    else if(resp.type == (char)ACK)
    {
       memset(&resp, 0, sizeof(resp));
       memset(&msg, 0, sizeof(msg));

       listen(tcpSocket, 5);

       struct sockaddr_in client;
       int sd;
       switch(fork())
       {
        case 0:
            sd = accept(tcpSocket, (struct sockaddr *)&client, sizeof(client));
            read(sd, resp, sizeof(resp));
            if(resp.type == (char)DOWNLOAD)
            {
                FILE *file;
                file = fopen(Filename, "rb");
                msg.type = (char)DATA;
                while(fgets(msg.data, BUFFLEN, file)>0){
                    write(sd, &msg, sizeof(msg));
                }
                fclose(file);
                close(sd);
            }
            break;
        case -1:
            fprintf(stderr, "Unable to create child process.\n");
            break;
       }
    }
}

/******************************************************************************
* Download Content
*******************************************************************************/

/******************************************************************************
* main
*******************************************************************************/
void main ( int argc, char **argv )
{
	char	*host = "localhost";
	int	port = 3000;
	char	now[100];		/* 32-bit integer to hold time	*/ 
	struct hostent	*phe;	/* pointer to host information entry	*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, n, type;	/* socket descriptor and socket type	*/

	switch (argc) {
	case 1:
		break;
	case 2:
		host = argv[1];
	case 3:
		host = argv[1];
		port = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "usage: UDPtime [host [port]]\n");
		exit(1);
	}

	memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;                                                                
        sin.sin_port = htons(port);
                                                                                        
    /* Map host name to IP address, allowing for dotted decimal */
        if ( phe = gethostbyname(host) ){
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        }
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		fprintf(stderr, "Can't get host entry \n");
                                                                                
    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
		fprintf(stderr, "Can't create socket \n");
	
                                                                                
    /* Connect the socket */
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "Can't connect to %s \n", host);

    char peerName[10] = {0};
    GetPeerName(peerName);

    while(1)
    {
        char input[10] = {0};

        printf("Enter the letter associated with the action you would like to complete: \n");
        printf("Register content (r). \n");
        printf("List content (o). \n");
        printf("Download content (d). \n");
        printf("Search for content (s).\n");
        printf("Deregister content (t).\n");
        printf("Quit (q).\n");
        printf("\nMake a selection: ");
        read(0, input, 2);

        switch((int) input[0])
        {
            case REGISTER:
                RegisterContent(s, peerName);
                break;
            case ONLINE:
                RequestOnlineContent(s);
                break;
            case DOWNLOAD:
                
                break;
            case SEARCH:

                break;
            case DEREGISTER:

                break;
            case 113:

                break;
        }
    }
    close(s);
    exit(0);
}
