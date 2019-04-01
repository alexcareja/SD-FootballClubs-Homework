
#ifndef TEAM_EXTRACTOR_H_D
#define TEAM_EXTRACTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FootballClub.h"


Player *union_teams(FootballClub *clubs, char *club_A, char *club_B) {
	Player *auxP;
	FootballClub *club = initialize_clubs(0, NULL);
	club = add_club(club, "A+B");
	if(clubs == NULL){
		return NULL;
	}
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_players = NULL;	//variabila de parcurgere a listei
	// Caut clubul A
	while(1){
		if(aux == NULL){
			break;
		}
		if(strcmp(aux->name, club_A) == 0){
			break;
		}
		aux = aux->next;
	}
	if(aux != NULL){
		aux_players = aux->players;
	}
	// Adaug jucatorii din clubul A daca exista
	while(aux_players != NULL){
		add_player(club, club->name, aux_players->name, aux_players->position,
					aux_players->score);
		aux_players = aux_players->next;
	}
	
	// Caut clubul B
	aux = clubs;
	while(1){
		if(aux == NULL){
			break;
		}
		if(strcmp(aux->name, club_B) == 0){
			break;
		}
		aux = aux->next;
	}
	if(aux != NULL){
		aux_players = aux->players;
	}
	// Adaug jucatorii din clubul B daca exista
	while(aux_players != NULL){
		add_player(club, "A+B", aux_players->name, aux_players->position,
					aux_players->score);
		aux_players = aux_players->next;
	}
	auxP = club->players;
	free(club->name);
	free(club);
	return auxP;
} 

Player *get_best_player(FootballClub *clubs, char *position) {
	Player *auxP;
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player;			// variabila de parcurgere a listei
	int max_score = -101;		// scorul maxim
	FootballClub *club = initialize_clubs(0, NULL);
	club = add_club(club, "best");

	// Trec prin toate echipele si caut scorul maxim de pe pozitia coresp.
	for(; aux != NULL; aux = aux->next){
		aux_player = aux->players;
		while(aux_player != NULL){
			if(strcmp(aux_player->position, position) == 0 &&
				aux_player->score > max_score){
				max_score = aux_player->score;
			}
			aux_player = aux_player->next;
		}
	}
	// Caut jucatorii cu scorul maxim si ii adaug intr-o lista
	for(aux = clubs; aux != NULL; aux = aux->next){
		aux_player = aux->players;
		while(aux_player != NULL){
			if(strcmp(aux_player->position, position) == 0 &&
				aux_player->score == max_score){
				add_player(club, "best", aux_player->name, 
							aux_player->position, aux_player->score);	
				// Functia add_player adauga jucatorii in ordine alfabetica
			}
			aux_player = aux_player->next;
		}
	}
	if(club->players == NULL){
		return NULL;
	}
	auxP = club->players;
	if(auxP->next != NULL){
		destroy_player_list(auxP->next);
		auxP->next = NULL;
	}
	free(club->name);
	free(club);
	//club->players->next = NULL;
	return auxP;
}

// Functie care compara jucatorii dupa scor, nume
// Returneaza 0 daca jucatorul p1 trebuie pus inainte de p2
// Returneaza 1 daca jucatorul p2 trebuie pus inainte de p1
int compare_best_players(Player *p1, Player *p2){
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

// Functie de adaugat playeri in lista comparati dupa criteriile scor, nume
void add_best_players(FootballClub *clubs, char *player_name, 
						char *position, int score, int injured) {
	if(clubs == NULL){
		return;
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
	player->injured = injured;
	player->next = NULL;
	player->prev = NULL;
	// Daca lista de jucatori este goala, doar adaug jucatorul
	if(clubs->players == NULL){
		clubs->players = player;
		return;
	}
	Player *aux_p1 = clubs->players; // variabila de parcurgere a listei
	// Cazul in care exista numai un jucator in lista
	if(aux_p1->next == NULL){
		if(compare_best_players(aux_p1, player)){
			player->next = aux_p1;
			aux_p1->prev = player;
			clubs->players = player;
		}
		else{
			aux_p1->next = player;
			player->prev = aux_p1;
		}
		return;
	}
	// Cazul in care trebuie adaugat la inceput
	if(compare_best_players(aux_p1, player)){
		player->next = aux_p1;
		aux_p1->prev = player;
		clubs->players = player;
		return;
	}
	Player *aux_p2 = aux_p1->next; // variabila de parcurgere a listei
	// Parcurg lista si adaug jucatorul intre p1 si p2 daca trebuie
	for(; aux_p2 != NULL; aux_p1 = aux_p2, aux_p2 = aux_p2->next){
		if(compare_best_players(player, aux_p2) == 0){
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

Player *get_top_players(FootballClub *clubs, int N) {
	Player *auxP;
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player;			// variabila de parcurgere a listei
	int i;						// contor
	FootballClub *club;			// club temporar pentru cei mai buni 
								// jucatori dintr-o echipa
	FootballClub *best = initialize_clubs(0, NULL);
	best = add_club(best, "bestest");
	
	// Parcurg fiecare club
	for(; aux != NULL; aux = aux->next){
		club = initialize_clubs(0, NULL);
		club = add_club(club, "bestN");
		aux_player = aux->players;
		// Creez un nou club cu jucatorii ordonati dupa scor, nume
		while(aux_player != NULL){
			add_best_players(club, aux_player->name, 
						aux_player->position, aux_player->score, 0);
			aux_player = aux_player->next;
		}
		// Primii N jucatori din clubul nou ii adaug in lista de returnat
		for(i = 0, aux_player = club->players; i < N; i++){
			if(aux_player != NULL){
				add_best_players(best, aux_player->name, 
							aux_player->position, aux_player->score, 0);
				aux_player = aux_player->next;
			}
			else{
				break;
			}
		}
		destroy_club_list(club);
	}
	// Eliberez memorie
	auxP = best->players;
	free(best->name);
	free(best);
	return auxP;
}

Player *get_players_by_score(FootballClub *clubs, int score) {
	Player *auxP;
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player;			// variabila de parcurgere a listei
	FootballClub *best = initialize_clubs(0, NULL);
	best = add_club(best, "bestest");
	// Parcurg fiecare club
	for(; aux != NULL; aux = aux->next){
		// Parcurg lista de jucatori neaccidentati
		aux_player = aux->players;
		while(aux_player != NULL){
			if(aux_player->score >= score){
				add_best_players(best, aux_player->name, 
							aux_player->position, aux_player->score, 0);
			}
			aux_player = aux_player->next;
		}
		// Parcurg lista de jucatori accidentati
		aux_player = aux->injured_players;
		while(aux_player != NULL){
			if(aux_player->score >= score){
				add_best_players(best, aux_player->name, 
							aux_player->position, aux_player->score, 1);
			}
			aux_player = aux_player->next;
		}
	}
	// Eliberez memorie
	auxP = best->players;
	free(best->name);
	free(best);
	return auxP;
}

Player *get_players_by_position(FootballClub *clubs, char *position) {
	Player *auxP;
	FootballClub *aux = clubs;	// variabila de parcurgere a listei
	Player *aux_player;			// variabila de parcurgere a listei
	FootballClub *best = initialize_clubs(0, NULL);
	best = add_club(best, "bestest");
	// Parcurg fiecare club
	for(; aux != NULL; aux = aux->next){
		// Parcurg lista de jucatori neaccidentati
		aux_player = aux->players;
		while(aux_player != NULL){
			if(strcmp(aux_player->position, position) == 0){
				add_best_players(best, aux_player->name, 
							aux_player->position, aux_player->score, 0);
			}
			aux_player = aux_player->next;
		}
		// Parcurg lista de jucatori accidentati
		aux_player = aux->injured_players;
		while(aux_player != NULL){
			if(strcmp(aux_player->position, position) == 0){
				add_best_players(best, aux_player->name, 
							aux_player->position, aux_player->score, 1);
				
			}
			aux_player = aux_player->next;
		}
	}
	// Eliberez memorie
	auxP = best->players;
	free(best->name);
	free(best);
	return auxP;
}

Player *get_best_team(FootballClub *clubs) {
	// Iau cei mai buni playeri de pe fiecare pozitie
	Player *auxP;
	Player *portari = get_players_by_position(clubs, "portar");
	Player *fundasi = get_players_by_position(clubs, "fundas");
	Player *mijlocasi = get_players_by_position(clubs, "mijlocas");
	Player *atacanti = get_players_by_position(clubs, "atacant");
	FootballClub *best = initialize_clubs(0, NULL);
	best = add_club(best, "bestest");
	int i;
	// Pastrez cel mai bun portar
	if(portari != NULL){
		add_best_players(best, portari->name, portari->position, 
						portari->score, portari->injured);
	}
	destroy_player_list(portari);
	// Pastez cei mai buni 4 fundasi
	for(auxP = fundasi, i = 0; i < 4; i++){
		if(fundasi != NULL){
			add_best_players(best, auxP->name, auxP->position, 
							auxP->score, auxP->injured);
			auxP = auxP->next;
		}
		else{
			break;
		}
	}
	destroy_player_list(fundasi);
	// Pastrez cei mai buni 3 mijlocasi
	for(auxP = mijlocasi, i = 0; i < 3; i++){
		if(mijlocasi != NULL){
			add_best_players(best, auxP->name, auxP->position, 
							auxP->score, auxP->injured);
			auxP = auxP->next;
		}
		else{
			break;
		}
	}
	// Pastrez cei mai buni 3 atacanti
	destroy_player_list(mijlocasi);
	for(auxP = atacanti, i = 0; i < 3; i++){
		if(atacanti != NULL){
			add_best_players(best, auxP->name, auxP->position, 
							auxP->score, auxP->injured);
			auxP = auxP->next;
		}
		else{
			break;
		}
	}
	// Eliberez memorie
	destroy_player_list(atacanti);
	auxP = best->players;
	free(best->name);
	free(best);
	return auxP;
}

#endif // TEAM_EXTRACTOR_H_INCLUDED
