// Havan Patel(hp397)
//Charmian Goh(csg102)
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "sorter.h"
#define MAX_PIDS 256

volatile pid_t* pids;
char* finalExt;
char* finals;
char* extract;
char *cflag, *dflag, *oflag ;
char* rootDirPath;

void getdatas(char* lines, int i);
void recursiveDirectory(char* rootDirPath);
char* checkUsersInput(char* strs);

const char *getExt(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void subDirProcess(char* rootDirPath) {
  recursiveDirectory(rootDirPath);
}

int checkFirstLine(char* rootDirPath, char* filename) {
    int outcome = -1;
    char* finalEx = (char*)malloc(strlen(rootDirPath)+strlen(filename)+2);
    char *fileLines = (char*)malloc(strlen(rootDirPath)+1500); 
    if(finalEx == NULL || fileLines == NULL){
      printf("Could not allocate memory. Program Failes:");
      fflush(stdout);
      exit(1);
    }
    strcpy(finalEx,rootDirPath);
    strcat(finalEx, "/");
    strcat(finalEx, filename);
    char* temps = finalEx;
    FILE* fp = fopen(temps, "r+");
    int skip = 0;
     if(fp) {
      while (fgets(fileLines, 1500, fp) != NULL) {
        if(skip == 0){
            if(strstr(fileLines, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes") != NULL) {
              outcome = 0;
              skip = 1;
              break;
            } else {
              outcome = -1;
              skip = 1;
              break;
            }
        }
      } 
    } else {
      outcome =  -1;
    }
    fclose(fp);
    return outcome;
}

void csvChildProcess(char* rootDirPath, char* filename) {
  char* temps = rootDirPath;
  FILE* fp = fopen(temps, "r+");
  char *fileLine = (char*)malloc(strlen(rootDirPath)+1010); 
  extract = (char*)malloc(strlen(rootDirPath)+100);
  finals = (char*)malloc(strlen(rootDirPath)+100);
  if(fileLine == NULL || extract == NULL || finals == NULL){
    printf("Could not allocate memory. Program Failes:");
    fflush(stdout);
    exit(1);
  }
  FILE* outputFile;
  char* remove_dot = filename;
  if(oflag != NULL){
    mkdir(oflag, 0755);
    extract = oflag;    
    remove_dot = strtok(remove_dot, ".");
    strcpy(finals, extract);
    strcat(finals, "/");
    strcat(finals, remove_dot);
    strcat(finals, "-sorted-");
    strcat(finals, cflag);
    strcat(finals, ".csv");
    outputFile = fopen(finals, "w+");
  }else{
    extract = strtok(rootDirPath, ".");
    strcpy(finals, extract);
    strcat(finals, "-sorted-");
    strcat(finals, cflag);
    strcat(finals, ".csv");
    outputFile = fopen(finals, "w+");
  }
  int skip = 0;
  int c = 0;

  if(fp) {
    while (fgets(fileLine, 1500, fp) != NULL) {
      if(skip == 0){
          skip = 1;
      }else{
        getdatas(fileLine, c);
        c++;
      }  
  } 
   sorters(cflag, c, outputFile, cflag);
   fclose(fp);
   fclose(outputFile);
  }  
  free(fileLine);
  free(extract);
  free(finals);
  exit(0);
}

void recursiveDirectory(char* rootDirPath) {
  DIR  *d;
  struct dirent *dir;
  d = opendir(rootDirPath);
  if (d == NULL) {
    printf(" \nERROR OPENING DIRECTORY OR DIRECTORY DOES NOT EXIST\nRUN THE PROGRAM AGAIN\n");
    fflush(stdout);
    exit(EXIT_FAILURE);    
    }
  if (d){
    while ((dir = readdir(d)) != NULL ){
        //Finds the sub directory
        if(dir -> d_type == DT_DIR) {
           if (strcmp (dir -> d_name, "..") != 0 && strcmp (dir -> d_name, ".") != 0){
                pid_t pid = fork();
                if(pid > 0){
                    pids[MAX_PIDS-2]++;
                    continue;                  
                }else if (pid < 0){
                      printf("Failed to create child process\n");
                      fflush(stdout);
                      exit(EXIT_FAILURE);   
                }else{
                  printf("%d,", (int) getpid());
                  fflush(stdout);
                  int path_length;
                  char path[PATH_MAX];
                  path_length = snprintf (path, PATH_MAX, "%s/%s", rootDirPath, dir -> d_name);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    fflush(stdout);
                    exit (EXIT_FAILURE);
                }
                  subDirProcess(path);
                  exit(0);
                }
            }                            
        } else if (strcmp(getExt(dir -> d_name), "csv") == 0) {
            if(strstr(dir -> d_name, "sorted") != NULL) {
            } else {
                  pid_t pid = fork();
                  if(pid > 0){
                    pids[MAX_PIDS-2]++;
                    continue;
                  }
                   else if (pid < 0) {
                    printf("Failed to create child process\n");
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                  } else {
                    if (checkFirstLine(rootDirPath, dir -> d_name) == -1){
                      printf("%d,", (int) getpid());
                      fflush(stdout);
                      exit(0);
                    } else{
                        printf("%d,",(int) getpid());
                        fflush(stdout);
                        finalExt = (char*)malloc(strlen(rootDirPath)+strlen(dir -> d_name)+2);
                        strcpy(finalExt,rootDirPath);
                        strcat(finalExt, "/");
                        strcat(finalExt, dir -> d_name);
                        csvChildProcess(finalExt, dir->d_name);
                        exit(0);
                      }
            }
        }
      } 
    }
    int status;
    pid_t track;
    do{
      track = wait(&status);
    }while(track>0);
  }
    closedir(d);
}

int main(int argc, char **argv){
  int c;
  opterr = 0;
  list = (movie_data*)malloc(sizeof(movie_data) * 7100);
  while ((c = getopt (argc, argv, "c:d:o:")) != -1){
    switch (c){
      case 'c': cflag = optarg;
        break;
      case 'd': dflag = optarg;
        break;
      case 'o': oflag = optarg;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'd' || optopt == 'o'){
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          fflush(stdout);
        }else if (isprint(optopt)){
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          fflush(stdout);
        }else{
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
          fflush(stdout);
        }
        return 1;
      default:
        abort();
    }
  }
    if(strcmp(checkUsersInput(cflag), cflag) != 0){
      printf("ENTER CORRECT FIELD PLEASE, PROJECT IS Terminated\n");
      fflush(stdout);
      exit(1);
    }

  pids = mmap(0, MAX_PIDS * sizeof(pid_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if(!pids){
    exit (EXIT_FAILURE);
  }
  memset((void*) pids, 0, MAX_PIDS * sizeof(pid_t));
  pids[MAX_PIDS-1] = getpid();
  pids[MAX_PIDS-2]++;
  printf("Initial PID: %d\nPIDS of all child processes: ", (int) pids[MAX_PIDS-1]);
  fflush(stdout);

  rootDirPath = dflag;
  char cwd[1024];
  if(rootDirPath != NULL){
    recursiveDirectory(rootDirPath);
  }else{
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        recursiveDirectory(cwd);
      }else{
        perror("error opening current directory");
        fflush(stdout);
    }
  }
  printf("\nTotal number of processes: %d\n", (int) pids[MAX_PIDS-2]);
  fflush(stdout);

  free(list);
  return(0);
}

char* checkUsersInput(char* strs){
  char* returning;
    if(strcmp("color", strs) == 0){
      returning = strs;
    }else if(strcmp("num_critic_for_reviews", strs) == 0){
      returning = strs;
    }else if(strcmp("duration", strs) == 0){
      returning = strs;
    }else if(strcmp("director_facebook_likes", strs) == 0){
      returning = strs;
    }else if(strcmp("actor_3_facebook_likes", strs) == 0){
      returning = strs;
    }else if(strcmp("actor_1_facebook_likes", strs) == 0){
      returning = strs;
    }else if(strcmp("gross", strs) == 0){
      returning = strs;
    }else if(strcmp("num_voted_users", strs) == 0){
      returning = strs;
    }else if(strcmp("cast_total_facebook_likes", strs) == 0){
      returning = strs;
    }else if(strcmp("facenumber_in_poster", strs) == 0){
      returning = strs;
    }else if(strcmp("num_user_for_reviews", strs) == 0){
      returning = strs;
    }else if(strcmp("budget", strs) == 0){
      returning = strs;
    }else if(strcmp("title_year", strs) == 0){
      returning = strs;
    }else if(strcmp("actor_2_facebook_likes", strs) == 0){
      returning = strs;
    }else if(strcmp("imdb_score", strs) == 0){
      returning = strs;
    }else if(strcmp("aspect_ratio", strs) == 0){
      returning = strs;
    }else if(strcmp("movie_facebook_likes", strs) == 0){
      returning = strs;
    }else if(strcmp("director_name", strs) == 0){
      returning = strs;
    }else if(strcmp("actor_2_name", strs) == 0){
      returning = strs;
    }else if(strcmp("genres", strs) == 0){
      returning = strs;
    }else if(strcmp("actor_1_name", strs) == 0){
      returning = strs;
    }else if(strcmp("movie_title", strs) == 0){
      returning = strs;
    }else if(strcmp("actor_3_name", strs) == 0){
      returning = strs;
    }else if(strcmp("plot_keywords", strs) == 0){
      returning = strs;
    }else if(strcmp("movie_imdb_link", strs) == 0){
      returning = strs;
    }else if(strcmp("language", strs) == 0){
      returning = strs;
    }else if(strcmp("country", strs) == 0){
      returning = strs;
    }else if(strcmp("content_rating", strs) == 0){
      returning = strs;
    }else{
      returning = "EMPTY";
    }
    return returning;
}

void getdatas(char* lines, int i){
      char* line = lines;
      char *data; 
      char *lineholder;
      double d;

      lineholder = strdup(line);
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->color = data;
      }   

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->director_name = data;
      }

      //check if the double value is empty
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data) == 0){
        (list + i)->num_critic_for_reviews = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->num_critic_for_reviews = d;
      }
              
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->duration= 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->duration= d;
      }
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->director_facebook_likes = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->director_facebook_likes= d;
      }
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->actor_3_facebook_likes = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->actor_3_facebook_likes = d;
      }

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->actor_2_name = data;
      }

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->actor_1_facebook_likes = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->actor_1_facebook_likes= d;
      }

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->gross = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->gross = d;
      }
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->genres = data;
      }

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->actor_1_name = data;
      }
      
      //check if there is a "," inside quotation
      data = strsep(&lineholder,","); 
      if(data == NULL){
      }else if(data[0] == '\"'){
        data[0] = ' ';
        char title[100];
        strcpy(title, data);
        strcat(title, ",");
        data = strsep(&lineholder,"\"");
          strcat(title, data);
          title = trim(title);
        (list + i)->movie_title = title;
      }else{
        data = trim(data);
        (list + i)->movie_title = data;
      };

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->num_voted_users = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->num_voted_users = d;
      }     
  
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->cast_total_facebook_likes = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->cast_total_facebook_likes = d;
      } 
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->actor_3_name = data;
      } 
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->facenumber_in_poster = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->facenumber_in_poster = d;
      } 
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->plot_keywords = data; 
      }
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->movie_imdb_link = data;
      }     
  
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->num_user_for_reviews = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->num_user_for_reviews = d;
      } 
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->language = data;
      }
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->country = data; 
      }
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else{
        data = trim(data);
        (list + i)->content_rating= data;
      }
    
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->budget = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->budget = d;
      } 

      data = strsep(&lineholder,",");
      if(data == NULL) {
      } else if (strlen(data) == 0) {
        (list + i)->title_year = 0;
      } else {
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->title_year = d;
      }

      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->actor_2_facebook_likes = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->actor_2_facebook_likes = d;
      }
      
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->imdb_score = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->imdb_score = d;
      }
      
      
      data = strsep(&lineholder,",");
      if(data == NULL){
      }else if(strlen(data)==0){
        (list + i)->aspect_ratio = 0;
      }else{
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->aspect_ratio = d;
      }
      
      
      data = strsep(&lineholder,",");
      if(data == NULL || data[0] == '\n' ) {
        (list + i)->movie_facebook_likes = -1;
      }else if (strlen(data) == 0) {
        (list + i)->movie_facebook_likes = 0;
      } else {
        data = trim(data);
        sscanf(data, "%lf", &d);
        (list + i)->movie_facebook_likes = d;
      }
}