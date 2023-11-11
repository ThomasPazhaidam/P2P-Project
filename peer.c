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

    write(Socket, &msg, PACKETSIZE);
    read(Socket, &resp, PACKETSIZE);

    printf("Content available: \n");
    printf("%s\n", resp.data);
}

/******************************************************************************
* Register new content
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
