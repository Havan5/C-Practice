#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "sorter.h"
#include <assert.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "sorter.h"
 
pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char* portNumb;
int port;

//the thread function
void *connection_handler();
 
int main(int argc , char *argv[]){
    char *pflag;
    int c;
    portNumb = (char*) malloc(sizeof(char*) + 5);
     while ((c = getopt (argc, argv, "p:")) != -1){
      switch (c){
        case 'p': pflag = optarg;
            break;
        case '?':
            if (optopt == 'p'){
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            }else if (isprint(optopt)){
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            }
         return 1;
        default:
        abort();
      }
    }
    strcpy(portNumb, pflag);
    port = atoi(portNumb);
	pthread_t thread_id;         
        if( pthread_create( &thread_id , NULL ,  connection_handler, NULL) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        pthread_join( thread_id , NULL);
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(){
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 0;
    }
    //puts("bind done");
     
    //Listen
    listen(socket_desc , 3);     
     
    //Accept and incoming connection
    //puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    int i = 0;
    int b = 0;   
    int max = 5000; 
    while((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*)&c)) )
    {
        pthread_mutex_lock(&lockA);
        char clntName[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET,&client.sin_addr.s_addr,clntName,sizeof(clntName))!= 0){
           printf("Received Connection From IP ADDRESS <%s>\n",clntName);  
        } else {
           printf("Unable to get address\n"); 
        }

        //Get the socket descriptor
        int sock = client_sock;
        int read_size, size;
        char recvnum[3];   
        char recvColumnName[256];
        movie_data data1;   

        if(b == 0){
            b = 1;
            int s = recv(sock, recvColumnName, sizeof(recvColumnName), 0);
            if(s < 0){
                printf("COLUMN NOT RECEIVED\n");
            }
            list = (movie_data*) malloc(sizeof(movie_data) * max + 5044);  
        }

        //Receive a message from client
        printf("Reading files from client\n");
        while((size = (recv(sock, recvnum, sizeof(recvnum), 0))) > 0){
            if(i >= max){
                max = max * 2;
               list = realloc(list, sizeof(movie_data) * (max) + 5044);
            }
            if(atoi(recvnum) == 555){
                printf("Dump Request Received from client\n");
                break;
            }
            int nb = atoi(recvnum);
            char revbuf[nb]; // Receiver buffer
            read_size = recv(sock , revbuf , sizeof(revbuf) , 0);
            if(read_size < 0){
                printf("ERROR READING\n");
                break;
            }
                memset(recvnum, 0, sizeof(recvnum));
                //revbuf[nb] = '\0';
                char* line = revbuf;
                char* data; 
                char* lineholder;
                double d;
                int hasQuotation = 0;
                int leftQIndexis; //Get Left quote index      
                    const char *ptr0 = strchr(line, '\"');
                    if(ptr0){
                      leftQIndexis = ptr0 - line;
                      hasQuotation = 1;
                    } else {
                      leftQIndexis = -1;
                      hasQuotation = 0;
                    }
                    
                    int rightQIndexis; //get right quote index
                    if(hasQuotation == 1) {
                      line[leftQIndexis] = ' ';
                      const char *ptr1 = strchr(line, '\"');
                      if(ptr1) {
                          rightQIndexis = ptr1 - line;
                      } else {
                        rightQIndexis = -1;
                      }
                      line[rightQIndexis] = ' ';
                      int n;
                      for(n = 1 + leftQIndexis; n < rightQIndexis; n++) {
                        if(line[n] == ',') {
                          line[n] = ' ';
                        }     
                      }
                    }
                lineholder = strdup(line);
                data = strsep(&lineholder,",");
                if(data == NULL){
                }else{
                    data = trim(data);
                    data1.color = data;
                }   
                data = strsep(&lineholder,",");
                if(data == NULL){
                }else{
                    data = trim(data);
                    data1.director_name = data;
                }
                //check if the double value is empty
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data) == 0){
                    data1.num_critic_for_reviews = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.num_critic_for_reviews = d;
                  }
                          
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.duration= 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.duration= d;
                  }
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.director_facebook_likes = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.director_facebook_likes= d;
                  }
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.actor_3_facebook_likes = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.actor_3_facebook_likes = d;
                  }

                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.actor_2_name = data;
                  }

                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.actor_1_facebook_likes = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.actor_1_facebook_likes= d;
                  }

                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.gross = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.gross = d;
                  }
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.genres = data;
                  }

                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.actor_1_name = data;
                  }
                  
                  //check if there is a "," inside quotation
                  data = strsep(&lineholder,","); 
                  if(data == NULL){
                  }else if(data[0] == '\"'){
                    data[0] = ' ';
                    char title[100];
                    data = trim(data);
                    strcpy(title, data);
                    strcat(title, ",");
                    data = strsep(&lineholder,"\"");
                    strcat(title, data);
                    data1.movie_title = title;
                  }else{
                    data = trim(data);
                    data1.movie_title = data;
                  };

                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.num_voted_users = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.num_voted_users = d;
                  }     
              
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.cast_total_facebook_likes = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.cast_total_facebook_likes = d;
                  } 
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.actor_3_name = data;
                  } 
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.facenumber_in_poster = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.facenumber_in_poster = d;
                  } 
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.plot_keywords = data; 
                  }
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.movie_imdb_link = data;
                  }     
              
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.num_user_for_reviews = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.num_user_for_reviews = d;
                  } 
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.language = data;
                  }
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.country = data; 
                  }
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else{
                    data = trim(data);
                    data1.content_rating= data;
                  }
                
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.budget = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.budget = d;
                  } 

                  data = strsep(&lineholder,",");
                  if(data == NULL) {
                  } else if (strlen(data) == 0) {
                    data1.title_year = 0;
                  } else {
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.title_year = d;
                  }

                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.actor_2_facebook_likes = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.actor_2_facebook_likes = d;
                  }
                  
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.imdb_score = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.imdb_score = d;
                  }
                  
                  
                  data = strsep(&lineholder,",");
                  if(data == NULL){
                  }else if(strlen(data)==0){
                    data1.aspect_ratio = 0;
                  }else{
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.aspect_ratio = d;
                  }
                  
                  
                  data = strsep(&lineholder,",");
                  if (strlen(data) == 0) {
                    data1.movie_facebook_likes = 0;
                  } else {
                    data = trim(data);
                    sscanf(data, "%lf", &d);
                    data1.movie_facebook_likes = d;
                  }

                memset(revbuf ,0 , sizeof(revbuf));  //clear the variable
                //printf("%d\n",(i+1) );
                list[i] = data1;
                i++;
        }
        if(size == 0){
            puts("Client disconnected");
            exit(0);
            //fflush(stdout);
        }else if(size == -1){
            perror("recv failed");
        }
            printf("Sorting the data.....\n");
            sorters(recvColumnName, i, recvColumnName);
            //send the file name [DONE]  Allsorted-columnName.csv
             char fileName[256] = "Allsorted-<";
             strcat(fileName,recvColumnName);
             strcat(fileName,">.csv");
             //printf("new file name %s\n",fileName);
             memset(recvColumnName, 0, sizeof(recvColumnName));
            //send file to clinet : 1. variables
            int filefd;
            struct stat st;
            char remote_file[BUFSIZ];
            ssize_t read_return;
            char buffer[BUFSIZ];
              
            //open file
            filefd = open("sorted.csv",O_RDONLY);
            if (filefd == -1) {
                perror("open send file");
                // exit(EXIT_FAILURE);  
            }
                        
               //fstat for size
            if (fstat(filefd,&st) < 0) {
                perror("stat send file");
                // exit(EXIT_FAILURE);
                close(filefd);         
            }
            memset(remote_file, 0, sizeof(remote_file));
            sprintf(remote_file, "%lld,%s",
            (long long) st.st_size,fileName);
                  
               //size works****   
              //printf("size of file from server to client %s\n", remote_file);   
              //this send is for file size    
            send(sock, remote_file, sizeof(remote_file), 0);           
            //send the actual data
            while (1) {
                read_return = read(filefd, buffer, BUFSIZ);
                if (read_return == 0){
                    break;
                }
                if (read_return == -1) {
                    perror("read here?");
                    exit(EXIT_FAILURE);
                    break;
                }
                if (write(sock, buffer, read_return) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                    break;
                }
            }
            
        close(filefd);
        printf("Sorted File has been sent to client\n");
        // remove("sorted.csv");
        i = 0;
        b = 0;
        max = 5000;
        free(list);
        pthread_mutex_unlock(&lockA);
    }
        if (client_sock < 0)
        {
            perror("accept failed");
            return 0;
        }


    pthread_exit(0);
} 