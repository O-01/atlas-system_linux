#include "laps.h"

/**
 * race_state - keeps track of number of laps made by several cars in a race
 * @id: competitor id array
 * @size: length of @id array
*/

void race_state(int *id, size_t size)
{
	static racecar *list;

	if (size == 0 || !id)
	{
		free_race_state(&list);
		return;
	}
	for (size_t iter = 0; iter != size; iter++)
		id_processing(&list, id[iter]);
	print_state(list);
}

/**
 * id_processing - add new racecars / increase racecar laps, as applicable
 * @list: list of racecars to be checked against input id
 * @id: competitor id which to add / increase lap count
*/

void id_processing(racecar **list, const int id)
{
	int found = 0;

	for (racecar *tmp = *list; tmp; tmp = tmp->next)
		if (tmp->id == id)
		{
			found = 1;
			tmp->laps++;
			return;
		}
	if (!found)
		add_racecar(list, id);
}

/**
 * add_racecar - add new racecar to racecar list
 * @list: list of racecars to which new racecar added having input id
 * @id: competitor id
*/

void add_racecar(racecar **list, const int id)
{
	racecar *add = NULL;

	add = malloc(sizeof(racecar));
	if (!add)
	{
		free(add);
		return;
	}
	add->id = id;
	add->laps = 0;
	if (!*list)
	{
		add->next = NULL;
		*list = add;
	}
	else
	{
		if (id < (*list)->id)
		{
			add->next = *list;
			*list = add;
		}
		else
			for (racecar *tmp = *list; tmp; tmp = tmp->next)
				if (id > tmp->id)
				{
					if (tmp->next && id > tmp->next->id)
						continue;
					else
					{
						add->next = tmp->next;
						tmp->next = add;
						break;
					}
				}
	}
	printf("Car %d joined the race\n", add->id);
}

/**
 * print_state - print race state & lap count for each racecar
 * @list: list of racecars of which to print status
*/

void print_state(racecar *list)
{
	printf("Race state:\n");
	for (racecar *tmp = list; tmp; tmp = tmp->next)
		printf("Car %d [%d laps]\n", tmp->id, tmp->laps);
}

/**
 * free_race_state - free allocated memory related to race_state
 * @list: list of racecars whose allocated memory is to be freed
*/

void free_race_state(racecar **list)
{
	if (!*list)
		return;
	else if ((*list)->next)
		for (racecar *tmp = *list; tmp; tmp = (*list)->next)
		{
			tmp = (*list)->next;
			free(*list);
			*list = tmp;
		}
	free(*list);
	*list = NULL;
}
