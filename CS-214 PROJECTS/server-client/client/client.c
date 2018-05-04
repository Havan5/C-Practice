// make empty dirs: for (( i=1; i<=6; i++ )); do mkdir junk$i; done
//make csvs: for i in {1..1024}; do cp movie_metadata.csv "dataset3_$i.csv";done

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <fcntl.h>
#include <signal.h>

void getRecursDirectory(char* rootDirPath);
void* csvThreadProcess(void* filename);

struct directory* filestruct;
struct csv* csvstruct;
pthread_t* threads;
int threadCount=0;
int total_movies = 0;
int sockfd;

struct directory{
  char path[1000];
};
struct csv{
  char csvPath[1000];
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;

const char* getExt(const char* filename){
  const char* dot = strstr(filename, ".");
  if(!dot || dot == filename) {
    return "";
  }
  return dot + 1;
}

void* subDirThreadProcess(void* rootDirPath){
  struct directory* inDirFun = (struct directory*) rootDirPath;
  char dirpath[2000];
  strcpy(dirpath,inDirFun->path);
  getRecursDirectory(dirpath);
  pthread_exit(0);
}

void* csvThreadProcess(void* filename) {
pthread_mutex_lock(&lockA);
  struct csv* inCsvFun = (struct csv*) filename;
  char rootDirPath[1000];
  strcpy(rootDirPath,inCsvFun->csvPath);
  int skip = 0;
    FILE* fp = fopen(rootDirPath, "r");
    char * line = NULL;
    size_t len = 0;
    size_t read;
    char line_len[3];
    int x;
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
      if(skip == 0){
        skip = 1;
      }else{
         int leng = read;
         char buffer[leng];
         sprintf(line_len, "%d", leng);
         strcpy(buffer, line);
         if((x = write(sockfd, line_len, sizeof(line_len))) <= 0)
          {
            printf("ERROR writing lines server!!!\n");
            break;
          }
         int n = write(sockfd, buffer, sizeof(buffer));
         if (n <= 0) {
           perror("write");
         }
         memset(buffer ,0 , sizeof(buffer));  //clear the variable
         memset(line_len ,0 , sizeof(line_len));  //clear the variable
       }
    }
    fclose(fp);
    pthread_mutex_unlock(&lockA);
  pthread_exit(0);
}

void getRecursDirectory(char* rootDirPath){
  char path[1000];
  DIR *d = opendir(rootDirPath);
  struct dirent *dir;
  if(d){
    while((dir = readdir(d)) != NULL){
      //FIND THE SUB DIRECTORIES
      if(dir->d_type == DT_DIR){
        if(strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0){
          snprintf(path, PATH_MAX, "%s/%s", rootDirPath, dir->d_name);
          struct directory fs;
          strcpy(fs.path,path);
          filestruct[threadCount] = fs;
          pthread_mutex_lock(&lock);
            pthread_create(&threads[threadCount], NULL, subDirThreadProcess, &filestruct[threadCount]);
            threadCount +=1;
          pthread_mutex_unlock(&lock);   
        }
      }else if(strcmp(getExt(dir->d_name), "csv") == 0){
        if(strstr(dir->d_name, "sorted") != NULL){
        }else{
          snprintf (path, PATH_MAX, "%s/%s", rootDirPath, dir->d_name); 
          struct csv cs;
          strcpy(cs.csvPath,path);
        pthread_mutex_lock(&lock);
          csvstruct[threadCount] = cs;
          pthread_create(&threads[threadCount], NULL, csvThreadProcess, &csvstruct[threadCount]);
          threadCount +=1;
        pthread_mutex_unlock(&lock);    
        }
      }
    }
  }
  closedir(d);
}

int main(int argc, char **argv){
  char *cflag, *dflag, *oflag, *hflag, *pflag; 
  int c;
  int hasoflag = 0;
  int hasdflag = 0;
  int hashflag = 0;
  int haspflag = 0;
  opterr = 0;
  int k;
  for(k = 0; k < argc; k++ ) {
    if(strcmp(argv[k], "-o") == 0) {
     hasoflag = 1;
     break;
    }if(strcmp(argv[k], "-d") == 0){
      hasdflag = 1;
    }
    if(strcmp(argv[k], "-h") == 0){
      hashflag = 1;
    }
    if(strcmp(argv[k], "-p") == 0){
      haspflag = 1;
    }
  }
  while ((c = getopt (argc, argv, "c:d:h:p:o:")) != -1){
      switch (c){
        case 'c': cflag = optarg;
          break;
        case 'd': dflag = optarg;
          break;
        case 'h': hflag = optarg;
          break;
        case 'p': pflag = optarg;
          break;
        case 'o': oflag = optarg;
          break;
        case '?':
          if (optopt == 'c' ||optopt == 'h' || optopt == 'p' || optopt == 'd' || optopt == 'o'){
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            
          }else if (isprint(optopt)){
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            
          }
          return 1;
        default:
          abort();
      }
    }
    if(cflag == NULL){
      printf("-c field is needed Otherwise it will not run the program\n");
      exit(0);
    }
  char* columnName =(char*)malloc(sizeof(columnName)*20);
  char* outName = (char*)malloc(sizeof(outName)*100);
  char* rootDirPath = (char*)malloc(sizeof(dflag)*100);
  char* hostName = (char*)malloc(sizeof(hflag)*100);
  char* portNumber = (char*)malloc(sizeof(pflag)*5); 
  strcpy(outName,oflag);  
  strcpy(columnName,cflag);
  strcpy(rootDirPath, dflag);
  if(haspflag == 1){
    strcpy(portNumber, pflag);
  }else{
    printf("need -p flag\n");
    exit(0);
  }
  if(hashflag == 1){
    strcpy(hostName, hflag);   
  }else{
    printf("need -h flag\n");
    exit(0);
  }
  int PORT = atoi(portNumber);

  threads= (pthread_t*) malloc(sizeof(pthread_t)*10000);    
  filestruct = (struct directory*)malloc(sizeof(struct directory)*10000);
  csvstruct = (struct csv*)malloc(sizeof(struct csv)*10000);
    
  // We need port number
  // We need a sockaddr_in struct for 
  // creating server socket
  struct sockaddr_in address;
  // We need a hostent struct pointer to 
  // hold server info from gethostbyname()
  struct hostent *server;
  server = gethostbyname(hostName);

  // replace hostent pointer with your pointer name
  if (server == NULL)
  {
    // you can also call h_error() instead
    fprintf(stderr, "ERROR, NO SUCH HOST\n");
    exit(0);
  }

  // Now we need to initialize sockaddr_in struct
  // before we do initialization, it's good to set buffer zeros
  // replace corresponding parts
  memset(&address, 0, sizeof(address));
   address.sin_family = AF_INET;
   address.sin_port = htons(PORT);
  // replace corresponding parts
  // we do binary copy to initialize sockaddr_in address part
  // from hostent pointer
   bcopy((char*)server->h_addr,(char*)&address.sin_addr.s_addr, server->h_length);

  /****** STEP 1: Create Socket ******/
  // call socket(AF_INET, SOCK_STREAM, 0) to create socket  
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
   {
     //perror() report error message
     perror("sock");
     //exit your program
     exit(EXIT_FAILURE);
   }

  // call connect(socketfd, (struct sockaddr*)&..., sizeof(...))
   if (connect(sockfd, (struct sockaddr*) &address, sizeof(address)) < 0)
   {
     // perror() report error message
     perror("connect");
     exit(EXIT_FAILURE);
  }

  char sendCoulumnName[256];
  strcpy(sendCoulumnName, cflag);
  write(sockfd, sendCoulumnName, sizeof(sendCoulumnName));
  memset(sendCoulumnName, 0 , sizeof(sendCoulumnName));


    char cwd[1024];
    if(hasdflag == 0){
      if(getcwd(cwd, sizeof(cwd)) != NULL){
        getRecursDirectory(cwd);
      }else{
        printf("Error opening current directory\n");
        exit(0);
      }
    } 
    if(hasdflag == 1){
      getRecursDirectory(rootDirPath);
    }
  
  //printf("Initial PID: %u\n",(int)pthread_self());
  int count;
  int countT;
  //printf("TIDS Of All Child Threads: ");
  for(count=0;count<threadCount;count++){
    //printf("%u,",(int)*(threads+count));
    pthread_join(threads[count],NULL);
    countT++;
  }  

  sleep(2);
  printf("sending dump request to server......\n");
  char eof[3] = "555";
  if((write(sockfd, eof, sizeof(eof))) == 0){
    printf("ERROR writing to server!!!\n");
  }
  memset(eof, 0, sizeof(eof));

  char receiveFileName[BUFSIZ];
  long long file_length;
  char *file_path;
  ssize_t read_return;
  int filefd;
  char buffer1[BUFSIZ];
  while (recv(sockfd, receiveFileName, sizeof(receiveFileName), 0)){
    printf("Receiving sorted file from server....\n");
      file_length = strtoll(receiveFileName,&file_path,10);

      if (*file_path != ',') {
              printf("syntax error in request -- '%s'\n", receiveFileName);
              exit(EXIT_FAILURE);
          }
          file_path += 1;
          //printf("is the file name received? ?   =>  %s [%lld bytes]\n", file_path,file_length);
      if(hasoflag == 1){
        strcat(outName, "/");
        strcat(outName, file_path);
        filefd = open(outName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (filefd == -1) {
          perror("open");
          printf("---Provided output directory path is incorrect. Provide correct -o flag---\n");
          exit(EXIT_FAILURE);
        }
      }else{
        filefd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (filefd == -1) {
          perror("open");
          exit(EXIT_FAILURE);
        }
      }
      
      for (;  file_length > 0;  file_length -= read_return) {
              read_return = BUFSIZ;
              if (read_return > file_length)
                  read_return = file_length;

              read_return = read(sockfd, buffer1, read_return);
              if (read_return == -1) {
                  perror("read here2?");
                  exit(EXIT_FAILURE);
              }
              if (read_return == 0)
                  break;

              if (write(filefd, buffer1, read_return) == -1) {
                  perror("write");
                  exit(EXIT_FAILURE);
              }
          }    
      close(filefd);
      break; 
  }  
  //printf("\nTotal number of threads: %d\n", countT);
  close(sockfd);
  free(columnName);
  free(outName);  
  free(rootDirPath); 
  free(filestruct);
  free(csvstruct);
  free(hostName);
  free(portNumber);
  free(threads);
  printf("received the sorted output.....check the output\n");
  return 0;
}

 