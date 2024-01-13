#ifndef _LAPS_H_
#define _LAPS_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * struct race_car - node prototype for each car competing in the race
 * @id: competitor's unique identification number
 * @laps: recorded lap count
 * @next: next contender in roster
*/

typedef struct race_car
{
	int id;
	int laps;
	struct race_car *next;
} racecar;

void race_state(int *id, size_t size);
void id_processing(racecar **list, const int id);
void add_racecar(racecar **list, const int id);
void print_state(racecar *list);
void free_race_state(racecar **list);

#endif
