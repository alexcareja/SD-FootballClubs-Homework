#ifndef FOOTBALL_CLUB_H_D
#define FOOTBALL_CLUB_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// List node which contains information about one football player.
typedef struct Player {
	char *name;					// player's name
	char *position;				// player's game position
	int score;					// player's score
	int injured;				// indicator for injury (1 true, 0 false)
	struct Player *next;		// next list node
	struct Player *prev;		// previous list node
} Player;

// Structure which contains information about one football club.
typedef struct FootballClub {
	char *name;					// club's name
	Player *players;			// list with players in good shape
	Player *injured_players;	// list with injured players
	struct FootballClub *next;	// next list node
} FootballClub;

FootballClub *initialize_clubs(int clubs_no, char **names) {
	if(clubs_no == 0){
		return NULL;
	}
	FootballClub *clubs = (FootballClub *) malloc(sizeof(FootballClub));
	if(clubs == NULL){
		return NULL;
	}
	int i = 1;
	/*Player *players = (Player *) malloc(sizeof(Player));
	if(players == NULL){
		return NULL;
	}
	players->next = NULL;
	players->prev = NULL;
	Player *inj_players = (Player *) malloc(sizeof(Player));
	if(inj_players == NULL){
		return NULL;
	}
	inj_players->next = NULL;
	inj_players->prev = NULL;*/
	strcpy(clubs->name, names[0]);
	clubs->players = NULL;
	clubs->injured_players = NULL;
	clubs->next = NULL;
	for(; i < clubs_no; i++){
		add_club(clubs, names[i]);
	}
	return clubs;
}

FootballClub *add_club(FootballClub *clubs, char *name) {
	FootballClub *club = (FootballClub *) malloc(sizeof(FootballClub));
	if(club == NULL){
		return NULL;
	}
	FootballClub *aux = clubs;
	strcpy(club->name, name);
	club->players = NULL;
	club->injured_players = NULL;
	club->next = NULL;
	while(aux->next != NULL){
		aux = aux->next;
	}
	aux->next = club;
	return clubs;
}

void add_player(FootballClub *clubs, char *club_name, 
				char *player_name, char *position, int score) {
}

void transfer_player(FootballClub *clubs, char *player_name, 
					char *old_club, char *new_club) {
}

void remove_player(FootballClub *clubs, char *club_name, char *player_name) {
}

void update_score(FootballClub *clubs, char *club_name, 
					char *player_name, int score) {
}

void update_game_position(FootballClub *clubs, char *club_name, 
							char *player_name, char *position, int score) {
}

void add_injury(FootballClub *clubs, char *club_name,
				char *player_name, int days_no) {
}

void recover_from_injury(FootballClub *clubs, char *club_name, 
							char *player_name) {
}

// Frees memory for a list of Player.
void destroy_player_list(Player *player) {
}

// Frees memory for a list of FootballClub.
void destroy_club_list(FootballClub *clubs) {
}

// Displays a list of players.
void show_list(FILE *f, Player *players, int free_memory) {
	fprintf(f, "P: ");
	Player *player = players;
	while (player) {
		fprintf(f, "(%s, %s, %d, %c) ", 
			player->name,
			player->position,
			player->score,
			player->injured ? 'Y' : '_');
		player = player->next;
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}

// Displays a list of players in reverse.
void show_list_reverse(FILE *f, Player *players, int free_memory) {
	fprintf(f, "P: ");
	Player *player = players;
	if (player) {
		while (player->next) {
			player = player->next;
		}
		while (player) {
			fprintf(f, "(%s, %s, %d, %c) ", 
				player->name,
				player->position,
				player->score,
				player->injured ? 'Y' : '_');
			player = player->prev;
		}
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}


// Displays information about a football club.
void show_clubs_info(FILE *f, FootballClub *clubs) {
	fprintf(f, "FCs:\n");
	while (clubs) {
		fprintf(f, "%s\n", clubs->name);
		fprintf(f, "\t");
		show_list(f, clubs->players, 0);
		fprintf(f, "\t");
		show_list(f, clubs->injured_players, 0);
		clubs = clubs->next;
	}
}

#endif // FOOTBALL_CLUB_H_INCLUDED
