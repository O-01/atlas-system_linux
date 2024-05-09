#include "multithreading.h"

/**
 * create_task - creates task node & populates relevant member variable values
 * @entry: pointer to task entry function
 * @param: parameter to be passed to entry function
 * Return: pointer to created task struct
*/
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task;

	task = malloc(sizeof(task_t));
	if (!task)
		return (NULL);
	task->entry = entry;
	task->param = param;
	task->status = 0;
	pthread_mutex_init(&task->lock, NULL);
	return (task);
}

/**
 * destroy_task - destroy task nodes & task list
 * @task: pointer to task to destroy
*/
void destroy_task(task_t *task)
{
	if (task->result)
	{
		list_destroy((list_t *)task->result, free);
		free((list_t *)task->result);
	}
	free(task);
}

/**
 * exec_tasks - execute tasks
 * @tasks: pointer to list of tasks to be executed
 * Return: NULL upon completion
*/
void *exec_tasks(list_t const *tasks)
{
	node_t *tmp = NULL;
	task_t *task = NULL;
	int task_id = 0;

	if (!tasks)
		pthread_exit(NULL);
	for (tmp = tasks->head, task_id = 0; tmp; tmp = tmp->next, ++task_id)
	{
		task = tmp->content;
		if (!pthread_mutex_trylock(&task->lock))
			continue;
		if (!task->status)
		{
			task->status = STARTED;
			task->result = task->entry(task->param);
			tprintf("[%02d] Started\n", task_id);
			if (!task->result)
				task->status = FAILURE;
			else
				task->status = SUCCESS;
			tprintf("[%02d] %s\n", task_id,
				task->status == FAILURE ? "Failure" : "Success");
		}
	}
	return (NULL);
}
