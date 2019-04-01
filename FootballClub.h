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

FootballClub *add_club(FootballClub *clubs, char *name) {
	//Declar un club de fotbal si ii completez campurile
	FootballClub *club = (FootballClub *) malloc(sizeof(FootballClub));
	if(club == NULL){
		return NULL;
	}
	club->name = (char *) malloc(sizeof(name));
	strcpy(club->name, name);
	club->players = NULL;
	club->injured_players = NULL;
	club->next = NULL;
	//Daca acesta este singurul club, atunci il returnez
	if(clubs == NULL){
		return club;
	}
	//Daca mai exista si alte cluburi, adaug clubul la urma listei
	FootballClub *aux = clubs;
	while(aux->next != NULL){
		aux = aux->next;
	}
	aux->next = club;
	return clubs;
}

FootballClub *initialize_clubs(int clubs_no, char **names) {
	// Daca nu se initializeaza niciun club returnez NULL
	if(clubs_no == 0){
		return NULL;
	}
	// Declar lista de cluburi de fotbal si completez campurile primului club
	FootballClub *clubs = (FootballClub *) malloc(sizeof(FootballClub));
	if(clubs == NULL){
		return NULL;
	}
	int i;	// contor pentru numele respectiv numarul clubului
	//strcpy(clubs->name, names[0]);
	clubs->name = names[0];
	clubs->players = NULL;
	clubs->injured_players = NULL;
	clubs->next = NULL;
	// Adaug restul cluburilor
	for(i = 1; i < clubs_no; i++){
		clubs = add_club(clubs, names[i]);
	}
	return clubs;
}
// Functie care compara jucatorii dupa pozitie, scor, nume
// Returneaza 0 daca jucatorul p1 trebuie pus inainte de p2
// Returneaza 1 daca jucatorul p2 trebuie pus inainte de p1
int compare_players(Player *p1, Player *p2){
	if(strcmp(p1->position, p2->position) < 0){
		return 0;
	}
	if(strcmp(p1->position, p2->position) > 0){
		return 1;
	}
	if(p1->score > p2->score){
		return 0;
	}
	if(p1->score < p2->score){
		return 1;
	}
	if(strcmp(p1->name, p2->name) < 0){
		return 0;
	}
	if(strcmp(p1->name, p2->name) > 0){
		return 1;
	}
	return 0;
}

void add_player(FootballClub *clubs, char *club_name, 
				char *player_name, char *position, int score) {
	if(clubs == NULL){
		return;
	}
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	// Caut clubul la care trebuie adaugat jucatorul
	while(1){
		if(aux == NULL){
			return;
		}
		if(strcmp(aux->name, club_name) == 0){
			break;
		}
		aux = aux->next;
	}
	// Declar un jucator
	Player *player = (Player *) malloc(sizeof(Player));
	if(player == NULL){
		return;
	}
	player->name = (char *) malloc(sizeof(player_name));
	strcpy(player->name, player_name);
	player->position = position;
	player->score = score;
	player->injured = 0;
	player->next = NULL;
	player->prev = NULL;
	// Daca lista de jucatori este goala, doar adaug jucatorul
	if(aux->players == NULL){
		aux->players = player;
		return;
	}
	Player *aux_p1 = aux->players; // variabila de parcurgere a listei
	// Cazul in care exista numai un jucator in lista
	if(aux_p1->next == NULL){
		if(compare_players(aux_p1, player)){
			player->next = aux_p1;
			aux_p1->prev = player;
			aux->players = player;
		}
		else{
			aux_p1->next = player;
			player->prev = aux_p1;
		}
		return;
	}
	// Cazul in care trebuie adaugat la inceput
	if(compare_players(aux_p1, player)){
		player->next = aux_p1;
		aux_p1->prev = player;
		aux->players = player;
		return;
	}
	Player *aux_p2 = aux_p1->next; // variabila de parcurgere a listei
	// Parcurg lista si adaug jucatorul intre p1 si p2 daca trebuie
	for(; aux_p2 != NULL; aux_p1 = aux_p2, aux_p2 = aux_p2->next){
		if(compare_players(player, aux_p2) == 0){
			aux_p1->next = player;
			player->prev = aux_p1;
			aux_p2->prev = player;
			player->next = aux_p2;
			return;
		}		
	}
	// Daca trebuie adaugat la final
	aux_p1->next = player;
	player->prev = aux_p1;
}

void add_injury(FootballClub *clubs, char *club_name,
				char *player_name, int days_no) {
	if(clubs == NULL){
		return;
	}
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player = NULL;		// variabile in care retin jucatorul
	// Caut clubul de la care trebuie actualizat jucatorul
	while(1){
		if(aux == NULL){
			return;
		}
		if(strcmp(aux->name, club_name) == 0){
			break;
		}
		aux = aux->next;
	}
	if(aux->players == NULL){	// Daca lista de jucatori e goala
		return;
	}

	// Caut jucatorul care trebuie actualizat si il elimin din lista

	// Daca este primul jucator din lista
	if(strcmp(aux->players->name, player_name) == 0){
		aux_player = aux->players;
		if(aux_player->next != NULL){	//Daca jucatorul nu esingurul din lista
			aux_player->next->prev = NULL;
		}
		aux->players = aux->players->next;
		aux_player->next = NULL;
	}
	else{	// Daca jucatorul nu era primul
		aux_player = aux->players;
		while( 1 ){
			if(aux_player != NULL){
				if(strcmp(aux_player->name, player_name) == 0){
					break;
				}
				/*if(aux_player != NULL){
					printf("\n%s - %s \n", aux_player->name, player_name);
				}*/
				aux_player = aux_player->next;
				continue;
			}
			break;
		}
		if(aux_player != NULL){	// Daca am gasit jucatorul in scot din lista
			if(aux_player->prev != NULL){
				aux_player->prev->next = aux_player->next;
			}
			if(aux_player->next != NULL){
				aux_player->next->prev = aux_player->prev;
			}
		}
		else{
			return;
		}
	}
	aux_player->prev = NULL;
	aux_player->next = NULL;
	aux_player->injured = 1;
	aux_player->score = aux_player->score - (0.1 * days_no);
	if(aux_player->score < - 100){
		aux_player->score = -100;
	}
	// Adaugarea in lista injured_players
	// Daca lista de jucatori este goala, doar adaug jucatorul
	if(aux->injured_players == NULL){
		aux->injured_players = aux_player;
		return;
	}
	Player *aux_p1 = aux->injured_players; // variabila de parcurgere a listei
	// Cazul in care exista numai un jucator in lista
	if(aux_p1->next == NULL){
		if(compare_players(aux_p1, aux_player)){
			aux_player->next = aux_p1;
			aux_p1->prev = aux_player;
			aux->injured_players = aux_player;
		}
		else{
			aux_p1->next = aux_player;
			aux_player->prev = aux_p1;
		}
		return;
	}
	// Cazul in care trebuie adaugat la inceput
	if(compare_players(aux_p1, aux_player)){
		aux_player->next = aux_p1;
		aux_p1->prev = aux_player;
		aux->injured_players = aux_player;
		return;
	}
	Player *aux_p2 = aux_p1->next; // variabila de parcurgere a listei
	// Parcurg lista si adaug jucatorul intre p1 si p2 daca trebuie
	for(; aux_p2 != NULL; aux_p1 = aux_p2, aux_p2 = aux_p2->next){
		if(compare_players(aux_player, aux_p2) == 0){
			aux_p1->next = aux_player;
			aux_player->prev = aux_p1;
			aux_p2->prev = aux_player;
			aux_player->next = aux_p2;
			return;
		}		
	}
	// Daca trebuie adaugat la final
	aux_p1->next = aux_player;
	aux_player->prev = aux_p1;
}

void transfer_player(FootballClub *clubs, char *player_name, 
					char *old_club, char *new_club) {
	if(clubs == NULL){
		return;
	}
	FootballClub *aux = clubs;		// variabila de parcurgere a listei
	Player *aux_player = NULL;		// variabile in care retin jucatorul
	// Caut clubul de la care trebuie sters jucatorul
	while(1){
		if(aux == NULL){
			return;
		}
		if(strcmp(aux->name, old_club) == 0){
			break;
		}
		aux = aux->next;
	}

	// Parcurg lista de jucatori neaccidentati

	if(aux->players != NULL){	// Daca lista de jucatori neacc nu e goala
		
		// Caut jucatorul care trebuie sters din clubul old_club si il elimin
		// din lista

		// Daca este primul jucator din lista
		if(strcmp(aux->players->name, player_name) == 0){
			aux_player = aux->players;
			if(aux_player->next != NULL){	//Daca lista mai are alti jucatori
				aux_player->next->prev = NULL;
			}
			aux->players = aux->players->next;
			aux_player->next = NULL;
		}
		else{	// Daca jucatorul nu era primul
			aux_player = aux->players;
			while( 1 ){
				if(aux_player != NULL){
					if(strcmp(aux_player->name, player_name) == 0){
						break;
					}
					/*if(aux_player != NULL){
						printf("\n%s - %s \n", aux_player->name, player_name);
					}*/
					aux_player = aux_player->next;
					continue;
				}
				break;
			}
			if(aux_player != NULL){	// Daca am gasit jucatorul
				if(aux_player->prev != NULL){
					aux_player->prev->next = aux_player->next;
				}
				if(aux_player->next != NULL){
					aux_player->next->prev = aux_player->prev;
				}
			}
		}
	}
	if(aux_player == NULL){	// Daca nu l-am gasit in lista de jucatori neacc
		if(aux->injured_players != NULL){
		// Daca lista de jucatori neacc nu e goala
		// Parcurg lista de jucatori accidentati
		// Caut jucatorul care trebuie sters din clubul old_club si il elimin
		// din lista

		// Daca este primul jucator din lista
			if(strcmp(aux->injured_players->name, player_name) == 0){
				aux_player = aux->injured_players;
				if(aux_player->next != NULL){	//Daca lista are alti jucatori
					aux_player->next->prev = NULL;
				}
				aux->injured_players = aux->injured_players->next;
				aux_player->next = NULL;
			}
			else{	// Daca jucatorul nu era primul
				aux_player = aux->injured_players;
				while( 1 ){
					if(aux_player != NULL){
						if(strcmp(aux_player->name, player_name) == 0){
							break;
						}
						aux_player = aux_player->next;
						continue;
					}
					break;
				}
				if(aux_player != NULL){	// Daca am gasit jucatorul
					if(aux_player->prev != NULL){
						aux_player->prev->next = aux_player->next;
					}
					if(aux_player->next != NULL){
						aux_player->next->prev = aux_player->prev;
					}
				}
			}
		}	
	}
	if(new_club == NULL){	/*Cand este NULL transfer a fost apelat din functia
						remove si nu mai trebuie adaugat la niciun club*/
		return;
	}
	else{
		if(aux_player != NULL){	
			add_player(clubs, new_club, aux_player->name, 
						aux_player->position, aux_player->score);
			if(aux_player->injured == 1){
				add_injury(clubs, new_club, aux_player->name, 0);
			}
		}
	}
	free(aux_player);
}

void remove_player(FootballClub *clubs, char *club_name, char *player_name) {
	// Transfer jucatorul de la echipa din care trebuie eliminat catre void
	transfer_player(clubs, player_name, club_name, NULL);
}

void update_game_position(FootballClub *clubs, char *club_name, 
							char *player_name, char *position, int score) {
	if(clubs == NULL){
		return;
	}
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player = NULL;		// variabile in care retin jucatorul
	// Caut clubul de la care trebuie actualizat jucatorul
	while(1){
		if(aux == NULL){
			return;
		}
		if(strcmp(aux->name, club_name) == 0){
			break;
		}
		aux = aux->next;
	}
		// injured players !!!
	if(aux->players != NULL){
		// Daca lista de jucatori nu e goala
		// Caut jucatorul care trebuie actualizat si il elimin din lista

		// Daca este primul jucator din lista
		if(strcmp(aux->players->name, player_name) == 0){
			aux_player = aux->players;
			if(aux_player->next != NULL){	//Daca mai sunt alti si jucatori
				aux_player->next->prev = NULL;
			}
			aux->players = aux->players->next;
			aux_player->next = NULL;
		}
		else{	// Daca jucatorul nu era primul
			aux_player = aux->players;
			while( 1 ){
				if(aux_player != NULL){
					if(strcmp(aux_player->name, player_name) == 0){
						break;
					}
					/*if(aux_player != NULL){
						printf("\n%s - %s \n", aux_player->name, player_name);
					}*/
					aux_player = aux_player->next;
					continue;
				}
				break;
			}
			if(aux_player != NULL){	// Daca am gasit jucatorul
				if(aux_player->prev != NULL){
					aux_player->prev->next = aux_player->next;
				}
				if(aux_player->next != NULL){
					aux_player->next->prev = aux_player->prev;
				}
			}
		}
		if(aux_player != NULL){
			if(position != NULL){
			// Adaug jucatorul la acelasi club, cu pozitia, scorul actualizate
				add_player(clubs, club_name, aux_player->name, 
							position, score);
			}
			else{
				// Adaug jucatorul la acleasi club, doar cu scorul actualizat
				add_player(clubs, club_name, aux_player->name, 
							aux_player->position, score);
			}
		}
	}

	if(aux_player == NULL){ // Daca jucatorul nu a fost gasit in lista de neacc
		if(aux->injured_players != NULL){
			// Daca lista de jucatori accidentati nu e goala
			// Caut jucatorul care trebuie actualizat si il elimin din lista

			// Daca este primul jucator din lista
			if(strcmp(aux->injured_players->name, player_name) == 0){
				aux_player = aux->injured_players;
				if(aux_player->next != NULL){	//Daca mai sunt alti jucatori
					aux_player->next->prev = NULL;
				}
				aux->injured_players = aux->injured_players->next;
				aux_player->next = NULL;
			}
			else{	// Daca jucatorul nu era primul
				aux_player = aux->injured_players;
				while( 1 ){
					if(aux_player != NULL){
						if(strcmp(aux_player->name, player_name) == 0){
							break;
						}
						aux_player = aux_player->next;
						continue;
					}
					break;
				}
				if(aux_player != NULL){	// Daca am gasit jucatorul
					if(aux_player->prev != NULL){
						aux_player->prev->next = aux_player->next;
					}
					if(aux_player->next != NULL){
						aux_player->next->prev = aux_player->prev;
					}
				}
			}
			if(position != NULL){
				// Adaug jucatorul la acelasi club cu valorile actualizate
				add_player(clubs, club_name, aux_player->name, position, 
							score);
				add_injury(clubs,  club_name, aux_player->name, 0);
			}
			else{
				// Adaug jucatorul la acleasi club, doar cu scorul actualizat
				add_player(clubs, club_name, aux_player->name, 
							aux_player->position, score);
				add_injury(clubs,  club_name, aux_player->name, 0);
			}
		}
	}
	free(aux_player);
}

void update_score(FootballClub *clubs, char *club_name, 
					char *player_name, int score) {
	// Nu modific pozitia jucatorului, ci doar scorul folosind functia 
	// update_game_position cu position = NULL
	update_game_position(clubs, club_name, player_name, NULL, score);
}

void recover_from_injury(FootballClub *clubs, char *club_name, 
							char *player_name) {
	if(clubs == NULL){
		return;
	}
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player = NULL;		// variabile in care retin jucatorul
	// Caut clubul de la care trebuie actualizat jucatorul
	while(1){
		if(aux == NULL){
			return;
		}
		if(strcmp(aux->name, club_name) == 0){
			break;
		}
		aux = aux->next;
	}
	if(aux->injured_players == NULL){	// Daca lista de jucatori e goala
		return;
	}

	// Caut jucatorul care trebuie actualizat si il elimin din lista injured

	// Daca este primul jucator din lista
	if(strcmp(aux->injured_players->name, player_name) == 0){
		aux_player = aux->injured_players;
		if(aux_player->next != NULL){	//Daca jucatorul nu esingurul din lista
			aux_player->next->prev = NULL;
		}
		aux->injured_players = aux->injured_players->next;
		aux_player->next = NULL;
	}
	else{	// Daca jucatorul nu era primul
		aux_player = aux->injured_players;
		while( 1 ){
			if(aux_player != NULL){
				if(strcmp(aux_player->name, player_name) == 0){
					break;
				}
				/*if(aux_player != NULL){
					printf("\n%s - %s \n", aux_player->name, player_name);
				}*/
				aux_player = aux_player->next;
				continue;
			}
			break;
		}
		if(aux_player != NULL){	// Daca am gasit jucatorul in scot din lista
			if(aux_player->prev != NULL){
				aux_player->prev->next = aux_player->next;
			}
			if(aux_player->next != NULL){
				aux_player->next->prev = aux_player->prev;
			}
		}
	}
	aux_player->prev = NULL;
	aux_player->next = NULL;
	aux_player->injured = 0;
	add_player(clubs, club_name, aux_player->name, aux_player->position, 
				aux_player->score);
	free(aux_player);
}

// Frees memory for a list of Player.
void destroy_player_list(Player *player) {
	/*if(player == NULL){
		return;
	}
	Player *aux = player->next;
	free(player->name);
	free(player);
	destroy_player_list(aux);*/
}

// Frees memory for a list of FootballClub.
void destroy_club_list(FootballClub *clubs) {
	/*if(clubs == NULL){
		return;
	}
	FootballClub *aux = clubs->next;
	destroy_player_list(clubs->players);
	destroy_player_list(clubs->injured_players);
	free(clubs->name);
	destroy_club_list(aux);*/
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
