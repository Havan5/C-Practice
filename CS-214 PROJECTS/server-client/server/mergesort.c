// Havan Patel(hp397)
//Charmian Goh(csg102)
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "sorter.h"

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


char* Add_Sorted_Word_In_Header(const char *s, const char *old_Word, const char *new_Word){
    char *result;
    int i, cnt = 0;
    int new_Wordlen = strlen(new_Word);
    int old_Wordlen = strlen(old_Word);
 
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++){
        if (strstr(&s[i], old_Word) == &s[i]){
            cnt++;
            // Jumping to index after the old word.
            i += old_Wordlen - 1;
        }
    }
    // Making new string of enough length
    result = (char *)malloc(i + cnt * (new_Wordlen - old_Wordlen) + 1);
    i = 0;
    while (*s){
        // compare the substring with the result
        if (strstr(s, old_Word) == s){
            strcpy(&result[i], new_Word);
            i += new_Wordlen;
            s += old_Wordlen;
        }else
            result[i++] = *s++;
    }
    result[i] = '\0';
    return result;
}

void PrintArray(int n, movie_data *array, char* sorted){
    FILE* path = fopen("sorted.csv", "w");
    char str[] = {"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes"};
    char append[1200];
    strcpy(append, "Sorted: ");
    strcat(append, sorted);
    char *result = Add_Sorted_Word_In_Header(str, sorted, append);
    fprintf(path, "%s\n", result);
    //fflush(stdout);
    int i;
    for(i = 0; i < n; i++){
          fprintf(path,  
                "%s, %s, %lf,%lf, %lf, %lf, %s, %lf, %lf, %s, %s, %s,%lf, %lf, %s, %lf, %s, %s,%lf, %s, %s,%s, %lf, %lf, %lf, %f, %f, %lf\n", 
                (array +i)->color, (array +i)->director_name, (array +i)->num_critic_for_reviews,    
                (array +i)->duration, (array +i)->director_facebook_likes, (array +i)->actor_3_facebook_likes,   
                (array +i)->actor_2_name, (array +i)->actor_1_facebook_likes, (array +i)->gross, 
                (array +i)->genres, (array +i)->actor_1_name, (array +i)->movie_title,
                (array +i)->num_voted_users, (array +i)->cast_total_facebook_likes, (array +i)->actor_3_name,    
                (array +i)->facenumber_in_poster, (array +i)->plot_keywords, (array +i)->movie_imdb_link,    
                (array +i)->num_user_for_reviews, (array +i)->language, (array +i)->country, 
                (array +i)->content_rating, (array +i)->budget, (array +i)->title_year,
                (array +i)->actor_2_facebook_likes, (array +i)->imdb_score, (array +i)->aspect_ratio,
                (array +i)->movie_facebook_likes);
    }
    free(result);
    fclose(path);
    printf("Done Sorting...\n");
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

void sorters(char* userInput, int i, char* sorted){
    //basially this if-else get standard input from user and compares the value on which value or item to sort on
            /*
                this function takes 3 arguments
                1.the whole strcut array
                2.the first index is for left most side
                3.the length of struct - 1, used for the right most sides
            */
    if(strcmp(userInput,"color") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"director_name") == 0){
            MergeSort(list, 0,i-1,"string", "director_name");     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"num_users_for_reviews") == 0){   
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);        
    }else if(strcmp(userInput,"director_facebook_likes") == 0){ 
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);        
    }else if(strcmp(userInput,"actor_3_facebook_likes") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);        
    }else if(strcmp(userInput,"actor_2_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"genres") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"actor_1_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"movie_title") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"actor_3_name") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"plot_keywords") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"movie_imdb_link") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"language") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"country") == 0){
            MergeSort(list, 0,i-1,"string", userInput);     
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"content_rating") == 0){
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"duration") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"actor_1_facebook_likes") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"gross") == 0){   
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"budget") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"num_voted_users") == 0){ 
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"cast_total_facebook_likes") == 0){   
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"facenumber_in_poster") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"title_year") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"actor_2_facebook_likes") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"imdb_score") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"movie_facebook_likes") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"aspect_ratio") == 0){    
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else if(strcmp(userInput,"num_critic_for_reviews") == 0){  
            MergeSort(list, 0,i-1,"int", userInput);    
            PrintArray(i, list,sorted);
    }else{
        printf("\n Entered Column Field Is Not Correct \n TRY AGAIN\n");
        exit(0);
    }
}