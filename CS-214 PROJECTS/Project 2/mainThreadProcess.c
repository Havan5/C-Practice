// make empty dirs: for (( i=1; i<=6; i++ )); do mkdir junk$i; done
//make csvs: for i in {1..1024}; do cp movie_metadata.csv "dataset3_$i.csv";done

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sorter.h"

void getRecursDirectory(char* rootDirPath);
void* csvThreadProcess(void* filename);
void getdatas(char* lines, int i);
void sort(char* rootDirPath);

struct directory* filestruct;
struct csv* csvstruct;
pthread_t* threads;
int threadCount=0;
int total_movies = 0;

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
  pthread_exit(NULL);
}

void* csvThreadProcess(void* filename) {
  struct csv* inCsvFun = (struct csv*) filename;
  char rootDirPath[1000];
  strcpy(rootDirPath,inCsvFun->csvPath);
  sort(rootDirPath);
  pthread_exit(NULL);
}

void sort(char* rootDirPath){ 
    char* line = (char*)malloc(strlen(rootDirPath)*1000);  
    double d;
    int skip = 0;
    int temp_total_movies = 0;
    //movie_data data1;
    movie_data* temp_data = (movie_data*) malloc(sizeof(movie_data) * (1024*5000));

    char* temps = rootDirPath;
    FILE* fp = fopen(temps, "r");
    if(fp) {
      while (fgets(line, 2000, fp) != NULL) {
        if(skip == 0){
            skip = 1;
        }else{
            char* addComma = (char*)malloc(strlen(rootDirPath)*1000);
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

            strcpy(addComma, line);
            addComma[strlen(line)-1] = ','; 
            char *tempLine = (char*)malloc(strlen(rootDirPath) * 1000);
            char* tempStr;
            strcpy(tempLine , addComma);
            free(addComma);
            int k;
            for (k = 0; k < 28; k++) {
              int commaIndexis = 0;
              const char *ptr = strchr(tempLine, ',');
              if(ptr){
                commaIndexis = ptr - tempLine;
              }
              tempStr = (char*) malloc(strlen(rootDirPath) * 200);
              memset(tempStr, ' ', commaIndexis);
              if(commaIndexis != 0) {
                  int i;
                  int count = 0;
                  for(i = 0; i < commaIndexis; i++) {
                    count++;
                    tempStr[i] = tempLine[i];
                  }
                  tempStr[commaIndexis] = '\0';
                  trim(tempStr);
                  if(k == 0){
                      temp_data[temp_total_movies].color = tempStr;
                    }else if(k == 1){
                      temp_data[temp_total_movies].director_name = tempStr;
                    }else if(k == 2){
                      temp_data[temp_total_movies].num_critic_for_reviews = atoi(tempStr);
                    }else if(k == 3){
                      temp_data[temp_total_movies].duration = atoi(tempStr);
                    }else if(k == 4){
                      temp_data[temp_total_movies].director_facebook_likes = atoi(tempStr);
                    }else if(k == 5){
                      temp_data[temp_total_movies].actor_3_facebook_likes = atoi(tempStr);
                    }else if(k == 6){
                      temp_data[temp_total_movies].actor_2_name = tempStr;
                    }else if(k == 7){
                      temp_data[temp_total_movies].actor_1_facebook_likes = atoi(tempStr);
                    }else if(k == 8){
                      temp_data[temp_total_movies].gross = atoi(tempStr);
                    }else if(k == 9){
                      temp_data[temp_total_movies].genres = tempStr;
                    }else if(k == 10){
                      temp_data[temp_total_movies].actor_1_name = tempStr;
                    }else if(k == 11){
                      temp_data[temp_total_movies].movie_title = tempStr;
                    }else if(k == 12){
                      temp_data[temp_total_movies].num_voted_users = atoi(tempStr);
                    }else if(k == 13){
                      temp_data[temp_total_movies].cast_total_facebook_likes = atoi(tempStr);
                    }else if(k == 14){
                      temp_data[temp_total_movies].actor_3_name = tempStr;
                    }else if(k == 15){
                      temp_data[temp_total_movies].facenumber_in_poster = atoi(tempStr);
                    }else if(k == 16){
                      temp_data[temp_total_movies].plot_keywords = tempStr;
                    }else if(k == 17){
                      temp_data[temp_total_movies].movie_imdb_link = tempStr;
                    }else if(k == 18){
                      temp_data[temp_total_movies].num_user_for_reviews = atoi(tempStr);
                    }else if(k == 19){
                      temp_data[temp_total_movies].language = tempStr;
                    }else if(k == 20){
                      temp_data[temp_total_movies].country = tempStr;
                    }else if(k == 21){
                      temp_data[temp_total_movies].content_rating = tempStr;
                    }else if(k == 22){
                      temp_data[temp_total_movies].budget = atoi(tempStr);
                    }else if(k == 23){
                      temp_data[temp_total_movies].title_year = atoi(tempStr);
                    }else if(k == 24){
                      temp_data[temp_total_movies].actor_2_facebook_likes = atoi(tempStr);
                    }else if(k == 25){
                      sscanf(tempStr, "%lf", &d);
                      temp_data[temp_total_movies].imdb_score = d;
                    }else if(k == 26){
                      sscanf(tempStr, "%lf", &d);
                      temp_data[temp_total_movies].aspect_ratio = d;
                    }else if(k == 27){
                      temp_data[temp_total_movies].movie_facebook_likes = atoi(tempStr);
                    }
                }else{
                  if(k == 0){
                    temp_data[temp_total_movies].color = " ";
                  }else if(k == 1){
                    temp_data[temp_total_movies].director_name = " ";
                  }else if(k == 2){
                    temp_data[temp_total_movies].num_critic_for_reviews = 0;
                  }else if(k == 3){
                    temp_data[temp_total_movies].duration = 0;
                  }else if(k == 4){
                    temp_data[temp_total_movies].director_facebook_likes = 0;
                  }else if(k == 5){
                    temp_data[temp_total_movies].actor_3_facebook_likes = 0;
                  }else if(k == 6){
                    temp_data[temp_total_movies].actor_2_name = " ";
                  }else if(k == 7){
                    temp_data[temp_total_movies].actor_1_facebook_likes = 0;
                  }else if(k == 8){
                    temp_data[temp_total_movies].gross = 0;
                  }else if(k == 9){
                    temp_data[temp_total_movies].genres = " ";
                  }else if(k == 10){
                    temp_data[temp_total_movies].actor_1_name = " ";
                  }else if(k == 11){
                    temp_data[temp_total_movies].movie_title = " ";
                  }else if(k == 12){
                    temp_data[temp_total_movies].num_voted_users = 0;
                  }else if(k == 13){
                    temp_data[temp_total_movies].cast_total_facebook_likes = 0;
                  }else if(k == 14){
                    temp_data[temp_total_movies].actor_3_name = " ";
                  }else if(k == 15){
                    temp_data[temp_total_movies].facenumber_in_poster = 0;
                  }else if(k == 16){
                    temp_data[temp_total_movies].plot_keywords = " ";
                  }else if(k == 17){
                    temp_data[temp_total_movies].movie_imdb_link = " ";
                  }else if(k == 18){
                    temp_data[temp_total_movies].num_user_for_reviews = 0;
                  }else if(k == 19){
                    temp_data[temp_total_movies].language = " ";
                  }else if(k == 20){
                    temp_data[temp_total_movies].country = " ";
                  }else if(k == 21){
                    temp_data[temp_total_movies].content_rating = " ";
                  }else if(k == 22){
                    temp_data[temp_total_movies].budget = 0;
                  }else if(k == 23){
                    temp_data[temp_total_movies].title_year = 0;
                  }else if(k == 24){
                    temp_data[temp_total_movies].actor_2_facebook_likes = 0;
                  }else if(k == 25){
                    temp_data[temp_total_movies].imdb_score = 0;
                  }else if(k == 26){
                    temp_data[temp_total_movies].aspect_ratio = 0;
                  }else if(k == 27){
                    temp_data[temp_total_movies].movie_facebook_likes = 0;
                  }
              }

              //Remove the first word from tempLine
              char getRest[strlen(line)];
              strcpy(getRest, tempLine+commaIndexis+1);
              strcpy(tempLine,getRest);
            }
            //temp_data[temp_total_movies]. = data1;
            temp_total_movies++;
            free(tempStr);
            free(tempLine);
        } 
      } 
    }
  pthread_mutex_lock(&lockA);
      int v;
      for (v = 0; v < temp_total_movies ; v++){
        list[total_movies] = temp_data[v];
        total_movies++ ;
      }
      free(temp_data);
      free(line);
      fclose(fp);
  pthread_mutex_unlock(&lockA);
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
  char *cflag, *dflag, *oflag; 
  int c;
  int hasoflag = 0;
  int hasdflag = 0;
  opterr = 0;
  int k;
  for(k = 0; k < argc; k++ ) {
    if(strcmp(argv[k], "-o") == 0) {
     hasoflag = 1;
     break;
    }if(strcmp(argv[k], "-d") == 0){
      hasdflag = 1;
    }
  }
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
  list = (movie_data*)malloc(sizeof(movie_data) * (1024*20000));
  char* columnName =(char*)malloc(sizeof(columnName)*100);
  char* outName = (char*)malloc(sizeof(outName)*100);
  char* rootDirPath = (char*)malloc(sizeof(dflag)*100); 
  strcpy(outName,oflag);  
  strcpy(columnName,cflag);
  strcpy(rootDirPath, dflag);

  threads= (pthread_t*) malloc(sizeof(pthread_t)*10000);    
  filestruct = (struct directory*)malloc(sizeof(struct directory)*10000);
  csvstruct = (struct csv*)malloc(sizeof(struct csv)*10000);
    
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
  
  printf("Initial PID: %u\n",(int)pthread_self());
  
  int count;
  int countT;
  printf("TIDS Of All Child Threads: ");
  for(count=0;count<threadCount;count++){
    printf("%u,",(int)*(threads+count));
    pthread_join(threads[count],NULL);
    countT++;
  }  
  printf("\nTotal number of threads: %d\n", countT);
    

    FILE* fp2;
    char filePath[1024];
    if(hasoflag == 1){
      char filePathFileName[12] = "/sorted.csv";
      strcpy(filePath, oflag);
      strcat(filePath, filePathFileName);
       fp2 = fopen(filePath, "w");
       if(fp2 == NULL){
        printf("\n----Could not find output directory----,\n----Enter Correct output directory----\n");
        exit(0);
      }
      sorters(cflag, total_movies, fp2, cflag);
    }else{
      if(hasdflag == 1){
          char filePathFileName[12] = "/sorted.csv";
          strcpy(filePath, dflag);
          strcat(filePath, filePathFileName);
          fp2 = fopen(filePath, "w");
          sorters(cflag, total_movies, fp2, cflag);
      }else{
        char filePathFileName[12] = "/sorted.csv";
          strcpy(filePath, cwd);
          strcat(filePath, filePathFileName);
          fp2 = fopen(filePath, "w");
          sorters(cflag, total_movies, fp2, cflag);
      }
    }
  free(list);
  free(columnName);
  free(outName);  
  free(rootDirPath); 
  free(filestruct);
  free(csvstruct);
  free(threads);
  return 0;
}

 