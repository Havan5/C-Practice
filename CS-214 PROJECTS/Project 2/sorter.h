// Havan Patel(hp397)
//Charmian Goh(csg102)
#ifndef SORTER_H
#define SORTER_H

// contains movie datas
typedef struct movie_data{
	float imdb_score;
	float aspect_ratio;
	int num_critic_for_reviews;
	int duration;
	int director_facebook_likes;
	int actor_3_facebook_likes;
	int gross;
	int budget;
	int title_year;
	int actor_2_facebook_likes;
	int actor_1_facebook_likes;
	int movie_facebook_likes;
	int num_voted_users;
	int cast_total_facebook_likes;
	int facenumber_in_poster;
	int num_user_for_reviews;
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
void trim(char* str);
void sorters(char* userInput, int i, FILE* path, char* sorted);
#endif