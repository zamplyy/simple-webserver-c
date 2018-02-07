#include <sys/types.h>                      //SERVER KOD
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SERVER_PORT 8080                   //ORELEVANT men client och server måste ha matcha
#define BUF_SIZE 4096                       // BLOCK TRANSER SIZE
#define QUEUE_SIZE 10
void fatal();

int main(int argc, char *argv[]) {

    int s, b, l, fd, sa, bytes, on = 1;
    char buf[BUF_SIZE] = "";                    // BUFFER FÖR UTGÅENDE FILER
    struct sockaddr_in channel;             // Håller ip adress

    // Bygg adress struktur för att binda till socket
    memset(&channel, 0, sizeof(channel));   //Zero kanal

    channel.sin_family = AF_INET;
    channel.sin_addr.s_addr = htonl(INADDR_ANY);
    channel.sin_port = htons(SERVER_PORT);

    //Passive open. Wait for connection
    s = socket(AF_INET, SOCK_STREAM, 0); // SKAPA SOCKET
    if(s < 0) fatal("socket failed");
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

    b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
    if (b < 0) fatal("bind failed");

    l = listen(s, QUEUE_SIZE);              //SPECIFY QUEUE size
    if (l < 0) fatal("listen failed");

    //SOCKET is now setup and bound. Wait for connection and process it
    while(1){
        sa = accept(s, 0, 0); // BLock for connection request
        if (sa < 0) fatal("accept failed");
        char str[BUF_SIZE] = "";
        read(sa, buf, BUF_SIZE); //READ file name from socket
        //printf("%s\n", );
        //här borde vi få filnamnet som det vill ha
        //eller get requesten. tror jag

        fwrite(buf, BUF_SIZE, 1, stdout);

        strcpy(str, buf);

        char *token;
        const char s[2] = "/ ";
        char result[20] = "a";
        char felsida[20] = "404error.html";
        char meddelande[BUF_SIZE] = "";

        token = strtok(str, s);
        while( token != NULL ) {

          token = strtok(NULL, s);
          strcpy(result,token);
          break;
        }


        //Get and return the file
        fd = open(result, O_RDONLY); //open the file to be sent back
        if (fd < 0) {
          strcat(meddelande, "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\n\r\n");
          fd = open(felsida, O_RDONLY);
        }else{
          strcat(meddelande, "HTTP/1.1 200 OK\r\n\r\n");

        }


        while(1){
            bytes = read(fd, buf, BUF_SIZE); //read from file
            if (bytes <= 0) break;          //check for EOF


            //write meddd först
            //puts(meddelande);
            write(sa,meddelande,strlen(meddelande));
            //fwrite(buf, BUF_SIZE, 1, stdout);
            write(sa,buf,bytes);            //write bytes to socket
        }
        close(fd);          //close file
        close(sa);          //close connection
    }
}

void fatal(char * string){
    printf("%s\n", string);
    exit(1);
}
