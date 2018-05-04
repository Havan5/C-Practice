// Havan Patel(hp397)
//Charmian Goh(csg102)
#ifndef SORTER_H
#define SORTER_H

// contains movie datas
typedef struct movie_data{
	double imdb_score;
	double aspect_ratio;
	double num_critic_for_reviews;
	double duration;
	double director_facebook_likes;
	double actor_3_facebook_likes;
	double gross;
	double budget;
	double title_year;
	double actor_2_facebook_likes;
	double actor_1_facebook_likes;
	double movie_facebook_likes;
	double num_voted_users;
	double cast_total_facebook_likes;
	double facenumber_in_poster;
	double num_user_for_reviews;
	char* genres;
	char* actor_1_name;
	char* movie_title;
	char* actor_3_name;
	char* plot_keywords;
	char* movie_imdb_link;
	char* language;
	char* country;
	char* content_rating;
	char* actor_2_name;
	char* color;
	char* director_name;
}movie_data;

//struct to store data in heap
movie_data *list;
char *trim(char *str);
void sorters(char* userInput, int i, char* sorted);
void PrintArray(int n, movie_data *array, char* sorted);
#endif