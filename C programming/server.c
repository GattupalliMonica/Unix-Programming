   //source: geeksforgeeks
// server implementation using fork 
//libraries used
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void print_error(const char *message);
int parameters(int argc, char** argv);
int port_no;
char* Strat;
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, port_number; // file descriptor and new file descriptor sockts
     socklen_t client_length;
     char message[256];
     parameters(argc, argv);
     pid_t process_id; // fork creats many child process
     struct sockaddr_in server_address, client_address; //represents cleint and server address
     int rw; 

     // check if port is defined properl or not
     if (argc <= 1) {
         fprintf(stderr,"port unidentified\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating TCP sockets
     
     // if file descriptoe socket is negative then socket is not connected.
     if (sockfd < 0) 
        print_error("unable to open socket");
     bzero((char *) &server_address, sizeof(server_address));  // bzero used to erase the data
     port_number = atoi(argv[2]);

     // default parameters for using TCP.
     server_address.sin_family = AF_INET; 
     server_address.sin_addr.s_addr = INADDR_ANY;
     server_address.sin_port = htons(port_number);
    
     server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

     //raises an error if it is unable to unable to run
     if (bind(sockfd, (struct sockaddr *) &server_address,
              sizeof(server_address)) < 0) 
              print_error("binding unsuccessfully");
     listen(sockfd,5);  // prepared server to accept the client
     client_length = sizeof(client_address); // storing the size of address
     while (1) // runs infinitly untill it is killed
    {
     newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &client_length); // accepting the requried parameters

     //if above parameters is not accepted.
     if (newsockfd < 0) 
          print_error("not accepted");

    // checking fork with process id's
     if((process_id = fork()) == 0){
			close(sockfd);
            sockfd = -1;
    while(1){
     bzero(message,256);
     //n = read(newsockfd,message,255);
     rw = read(newsockfd,message,255);

    //array to store the client command
    char a[]= "./"; 
    // add ./ to the command to make it executable
    strcat(a,message); 

    //error in reading socket
     if (rw < 0) 
     {
        print_error("unable to read socket");
        break;
     }
     printf("client command %s\n",a);
     rw = write(newsockfd,"message received successfully",18);

     // execute the command sent by client
     system(a);
     //errr in writing socket
     if (rw < 0) 
     {
        print_error("unable to write socket");
        }
    }
     close(newsockfd);
    
     // To avoid bad file descriptor
     newsockfd = -1;
    exit(EXIT_SUCCESS);
}
    }
     close(sockfd);
     return 0; 
}

// function to print error mesage and exit 
void print_error(const char *message)
{
    perror(message);
    exit(1);
}


int parameters(int argc, char** argv)
{
    char* prog;

    prog = *argv;
    while (++argv, --argc > 0)
        if (**argv == '-')
            if(*++ * argv== 'p') {            
                --argc;
                port_no = atoi(*++argv);
                printf("Server is connected to the Port: %d\n", port_no);
                printf("Listening for the clients.....\n\n");
            }
            else if(*++ * argv== 'h') { 
                printf("\nHELP: try matinv -u \n\n");
                exit(0);
            }
            else if(*++ * argv== 's') { 
                --argc;
                Strat = (*++argv);
                printf(" NOTE: for this implementation , only fork strategy is used\n");
            }
            else{
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog); 
            }
}
