#include "laps.h"


racecar *head;

/**
 * race_state - keeps track of number of laps made by several cars in a race
 * @id: competitor id array
 * @size: length of @id array
*/

void race_state(int *id, size_t size)
{
	size_t iter = 0;

	if (size == 0 || !id)
	{
		free_race_state();
		return;
	}

	for (iter = 0; iter != size; iter++)
		id_processing(id[iter]);

	print_state();
}

/**
 * id_processing - add new racecars / increase racecar laps, as applicable
 * @id: competitor id which to add / increase lap count
*/

void id_processing(int id)
{
	racecar *tmp = head;
	int found = 0;

	for (; tmp; tmp = tmp->next)
		if (tmp->id == id)
		{
			found = 1;
			tmp->laps++;
			return;
		}

	if (!found)
		add_racecar(id);
}

/**
 * add_racecar - add new racecar to racecar list
 * @id: competitor id
*/

void add_racecar(int id)
{
	racecar *add, *tmp = head;

	add = malloc(sizeof(racecar));
	add->id = id;
	add->laps = 0;

	if (!head)
	{
		add->next = NULL;
		head = add;
	}
	else
	{
		if (id < head->id)
		{
			add->next = head;
			head = add;
		}
		else
			for (; tmp; tmp = tmp->next)
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
*/

void print_state(void)
{
	racecar *tmp = head;

	printf("Race state:\n");
	for (; tmp; tmp = tmp->next)
		printf("Car %d [%d laps]\n", tmp->id, tmp->laps);
}

/**
 * free_race_state - free allocated memory related to race_state
*/

void free_race_state(void)
{
	racecar *tmp = head;

	if (!head)
		return;

	for (; tmp; tmp = head->next)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
	free(head);
	head = NULL;
}
