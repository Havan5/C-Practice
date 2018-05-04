// Havan Patel(hp397)
//Charmian Goh(csg102)
#ifndef SORTER_H
#define SORTER_H

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

//struct to store data in heap
movie_data *list;

char *trim(char *str);
void sorters(char* userInput, int i, FILE* path, char* sorted);

#endif