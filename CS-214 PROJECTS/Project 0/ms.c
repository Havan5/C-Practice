//Havan Patel
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// contains movie datas
typedef struct movie_data{
	double num_critic_for_reviews;
	double duration;
	double director_facebook_likes;
	double actor_3_facebook_likes;
	double actor_1_facebook_likes;
	double gross;
	double num_voted_users;
	double cast_total_facebook_likes;
	double facenumber_in_poster;
	double num_user_for_reviews;
	double budget;
	double title_year;
	double actor_2_facebook_likes;
	double imdb_score;
	double aspect_ratio;
	double movie_facebook_likes;
	char* color;
	char* director_name;
	char* actor_2_name;
	char* genres;
	char* actor_1_name;
	char* movie_title;
	char* actor_3_name;
	char* plot_keywords;
	char* movie_imdb_link;
	char* language;
	char* country;
	char* content_rating;
}movie_data;
movie_data *list;
void subDirProcess(char* rootDirPath);


char *trim(char *str){
  char *end;
  // Trim leading space
  while(isspace((unsigned char)*str)) str++;
  if(*str == 0)  // All spaces?
    return str;
  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  // Write new null terminator
  *(end+1) = 0;
  return str;
}

void PrintArray(int n, movie_data *array){
    int i;
    for(i=0; i < n; i++){
            char s[50];
            if(((array + i)->movie_facebook_likes) < 0) {
                memset(s, 0, 50);
            } else {
                sprintf(s,"%f", (array + i)->movie_facebook_likes);
            }
            fprintf(stdout,  
            "%s, %s, %lf,%lf, %lf, %lf,%s, %lf, %lf,%s, %s, %s,%lf, %lf, %s,%lf, %s, %s,%lf, %s, %s,%s, %lf, %lf,%lf, %lf, %lf,%s\n", 
            (array + i)->color, (array + i)->director_name, (array + i)->num_critic_for_reviews,    
            (array + i)->duration, (array + i)->director_facebook_likes, (array + i)->actor_3_facebook_likes,   
            (array + i)->actor_2_name, (array + i)->actor_1_facebook_likes, (array + i)->gross, 
            (array + i)->genres, (array + i)->actor_1_name, (array + i)->movie_title,
            (array + i)->num_voted_users, (array + i)->cast_total_facebook_likes, (array + i)->actor_3_name,    
            (array + i)->facenumber_in_poster, (array + i)->plot_keywords, (array + i)->movie_imdb_link,    
            (array + i)->num_user_for_reviews, (array + i)->language, (array + i)->country, 
            (array + i)->content_rating, (array + i)->budget, (array + i)->title_year,
            (array + i)->actor_2_facebook_likes, (array + i)->imdb_score, (array + i)->aspect_ratio,
            s);
    }
    putchar('\n');
    fflush(stdout);
}

char* get_S_Values(int index, movie_data* x, char* queryOn) {
    if (strcmp(queryOn, "color") == 0) {
        return x[index].color;
    }else if(strcmp(queryOn, "director_name") == 0){
        return x[index].director_name;
    }else if(strcmp(queryOn, "actor_2_name") == 0){
        return x[index].actor_2_name;
    }else if(strcmp(queryOn, "genres") == 0){
        return x[index].genres;
    }else if(strcmp(queryOn, "actor_1_name") == 0){
        return x[index].actor_1_name;
    }else if(strcmp(queryOn, "movie_title") == 0){
        return x[index].movie_title;
    }else if(strcmp(queryOn, "actor_3_name") == 0){
        return x[index].actor_3_name;
    }else if(strcmp(queryOn, "plot_keywords") == 0){
        return x[index].plot_keywords;
    }else if(strcmp(queryOn, "movie_imdb_link") == 0){
        return x[index].movie_imdb_link;
    }else if(strcmp(queryOn, "language") == 0){
        return x[index].language;
    }else if(strcmp(queryOn, "country") == 0){
        return x[index].country;
    }else if(strcmp(queryOn, "content_rating") == 0){
        return x[index].content_rating;
    }
    return 0;
}

int getValues(int index, movie_data* x, char* queryOn) {
    if(strcmp(queryOn, "num_critic_for_reviews") == 0){
        return x[index].num_critic_for_reviews;
    }else if(strcmp(queryOn, "duration") == 0){
        return x[index].duration;
    }else if(strcmp(queryOn, "director_facebook_likes") == 0){
        return x[index].director_facebook_likes;
    }else if(strcmp(queryOn, "actor_3_facebook_likes") == 0){
        return x[index].actor_3_facebook_likes;
    }else if(strcmp(queryOn, "actor_1_facebook_likes") == 0){
        return x[index].actor_1_facebook_likes;
    }else if(strcmp(queryOn, "gross") == 0){
        return x[index].gross;
    }else if(strcmp(queryOn, "num_voted_users") == 0){
        return x[index].num_voted_users;
    }else if(strcmp(queryOn, "cast_total_facebook_likes") == 0){
        return x[index].cast_total_facebook_likes;
    }else if(strcmp(queryOn, "facenumber_in_poster") == 0){
        return x[index].facenumber_in_poster;
    }else if(strcmp(queryOn, "num_user_for_reviews") == 0){
        return x[index].num_user_for_reviews;
    }else if(strcmp(queryOn, "budget") == 0){
        return x[index].budget;
    }else if(strcmp(queryOn, "title_year") == 0){
        return x[index].title_year;
    }else if(strcmp(queryOn, "actor_2_facebook_likes") == 0){
        return x[index].actor_2_facebook_likes;
    }else if(strcmp(queryOn, "imdb_score") == 0){
        return x[index].imdb_score;
    }else if(strcmp(queryOn, "aspect_ratio") == 0){
        return x[index].aspect_ratio;
    }else if(strcmp(queryOn, "movie_facebook_likes") == 0){
        return x[index].movie_facebook_likes;
    }
    return -1;
}

void Merge(movie_data *arr, int low, int mid, int high, char* tag, char* val){
    //Find sizes of two sub-arrays to be merged
    int index = high - low + 1;
    //creating temparory struct storing all values and then puting it back in original struct
    struct movie_data *temp = (movie_data *)malloc(index * sizeof(movie_data));
    int mergePos = 0;
    int leftIndex = low;//left half
    int rightIndex = mid + 1;//right half
    //copying the data into temp struct 
    while (leftIndex <= mid && rightIndex <= high){   
        //if the values are int perform int mergesort otherwise string mergesort
        if(strcmp(tag, "int") == 0){
            if(getValues(leftIndex, arr, val) <= getValues(rightIndex, arr, val)){
                temp[mergePos++] = arr[leftIndex++];
            }
            else{
                temp[mergePos++] = arr[rightIndex++];
            }
        }else{
            if (strcmp(get_S_Values(leftIndex, arr, val), get_S_Values(rightIndex, arr, val)) <= 0){
                temp[mergePos++] = arr[leftIndex++];
        }else{
                temp[mergePos++] = arr[rightIndex++];
        }
        }
    }
    while (leftIndex <= mid)    {
        temp[mergePos++] = arr[leftIndex++];
    }
    while (rightIndex <= high)    {
        temp[mergePos++] = arr[rightIndex++];
    }
    assert(mergePos == index);

    //copy back to original struct array from temp struct array
    for (mergePos = 0; mergePos < index; ++mergePos){
        arr[low + mergePos] = temp[mergePos];
    }
    free(temp);
}

//function divides structs in halves recursivly
void MergeSort(movie_data *arr, int low, int high, char *tag, char* field){
    //we chaeck if we have struct and are not exceeding the size
    if (low < high){
        int mid = (low + high) / 2;//gives the middle index of struct
        MergeSort(arr, low, mid,tag, field);// left half is recursivly called 
        MergeSort(arr, mid + 1, high,tag, field);//right half is recursivly called
        Merge(arr, low, mid, high,tag, field);//does the merging of values
    }
}

void sorters(char* userInput, int i){
    //basially this if-else get standard input from user and compares the value on which value or item to sort on
    if(strcmp(userInput,"color") == 0){
            /*
                this function takes 3 arguments
                1.the whole strcut array
                2.the first index is for left most side
                3.the length of struct - 1, used for the right most sides
            */
            MergeSort(list, 0,i-1,"string", userInput);     
            //simply print to file when done mergesorting
            PrintArray(i, list);
    }else if(strcmp(userInput,"director_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"num_users_for_reviews") == 0){   
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);        
    }else if(strcmp(userInput,"director_facebook_likes") == 0){ 
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);        
    }else if(strcmp(userInput,"actor_3_facebook_likes") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);        
    }else if(strcmp(userInput,"actor_2_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"genres") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"actor_1_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"movie_title") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"actor_3_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"plot_keywords") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"movie_imdb_link") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"language") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"country") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list);
    }else if(strcmp(userInput,"content_rating") == 0){
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"duration") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"actor_1_facebook_likes") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"gross") == 0){   
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"budget") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"num_voted_users") == 0){ 
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"cast_total_facebook_likes") == 0){   
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"facenumber_in_poster") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"title_year") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"actor_2_facebook_likes") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"imdb_score") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"movie_facebook_likes") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"aspect_ratio") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else if(strcmp(userInput,"num_critic_for_reviews") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list);
    }else{
        printf("Field Enter Is Not Available \n Please Run It Again!" );
    }
}

//main function will be reding file and assign it to specific data to value from struct
int main (int argc, char **argv){
	// size if of struct
	list = (movie_data*)malloc(sizeof(movie_data) * 50000);
	//temporarily stores data
	char *lines = (char*)malloc(1000);
	//reading line (row) index and storing and incrementign the next strcut
	int i =0;
	//skip first line
	int skip = 0;
	//for converting char number to double nu,bers
	double d;

	//read line until hit null termination
	while(fgets(lines, 1000, stdin)!= NULL){
			char* line = trim(lines);
			char *data; //data holder
			char *lineholder;
			//read specific value deliminated by ","
			lineholder = strdup(line);
		if(skip == 0){

			skip = 1;
		}else{
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
                data = trim(data);
				strcpy(title, data);
				strcat(title, ",");
				data = strsep(&lineholder,"\"");
			    strcat(title, data);
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
			
			
			if(data == NULL) {
			}else if (strlen(data) == 0) {
				(list + i)->movie_facebook_likes = 0;
			} else {
				data = trim(data);
				data = trim(data);
				sscanf(data, "%lf", &d);
				(list + i)->movie_facebook_likes = d;
			}
			//printf("Value of data for last column: %d\n", data);
			i++;			
		}
	}
	char* Svalues = argv[2];
 	sorters(Svalues, i);	
	//free the memory alocated by line, and list
	free(lines);
	free(list);
	return 0;
}

