//source:geeksforgeeks
// client implementation 
// libraries used
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
// printing error msg
void print_error(const char *message);

// original implememtation
int main(int argc, char *argv[])
{
    int sockfd, port_number, rw;
    struct sockaddr_in server_address;
    struct hostent *server;

    char message[256];
    // if condition not satisfied it exit immediately
    if (argc < 5) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    port_number = atoi(argv[4]);
    //default prameter for tcp network
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if file descriptoe socket is negative then socket is not connected.
    if (sockfd < 0) 
        print_error("unable to open socket");
    server = gethostbyname(argv[2]);
    // if  server is not avaliable 
    if (server == NULL) {
        fprintf(stderr,"unable to identify host\n");
        exit(0);
    }
    bzero((char *) &server_address, sizeof(server_address));  // bzero used to erase the data
    // default parameters for using TCP.
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&server_address.sin_addr.s_addr,
         server->h_length);
    server_address.sin_port = htons(port_number); // bcopy copies n bytes


    server_address.sin_addr.s_addr
        = inet_addr("127.0.0.1");

   ///raises an error if it is unable to unable to connect
    if (connect(sockfd,(struct sockaddr *) &server_address,sizeof(server_address)) < 0) 
        print_error("unable connect");

while(1) 
{
    printf("client: enter your command ");
    bzero(message,256);
    fgets(message,255,stdin); // reading the message
    rw = write(sockfd,message,strlen(message));
    // unable to write
    if (rw < 0) 
         print_error("unable to write a socket");
    bzero(message,256);
    rw = read(sockfd,message,255);
    //unable to read
    if (rw < 0) 
         print_error("unable to read a socket");
    printf("%s\n",message);
}
    //close(sockfd);
    return 0;
}
// function to print error mesage and exit 
void print_error(const char *message)
{
    perror(message);
    exit(0);
}

int port_no = 9999;
char* ip_addr = "127.0.0.1";

void parameters(int argc, char** argv){
    char* prog;

    prog = *argv;
    while (++argv, --argc > 0)
        if (**argv == '-')
            if(*++ * argv =='i') {
                if((*++ * argv) == 'p'){
                    --argc;
                    ip_addr = (*++argv);
                    printf("Ip address: %s\n", ip_addr);
                }                
                
            if(*++ * argv =='p') {
                --argc;
                port_no = atoi(*++argv);
                printf("Port number: %d\n\n", port_no);
            }
            else{
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog);
            }
                
            }

}