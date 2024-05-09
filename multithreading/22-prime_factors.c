#include "multithreading.h"

void __attribute__((constructor)) initialize_task_mutex(void);
void __attribute__((destructor)) destroy_task_mutex(void);

task_t *create_task(task_entry_t entry, void *param);
void destroy_task(task_t *task);
void *exec_tasks(list_t const *tasks);

static pthread_mutex_t mutex;

/**
 * initialize_task_mutex - initialize mutual exception
*/
void __attribute__((constructor)) initialize_task_mutex(void)
{
	if (pthread_mutex_init(&mutex, NULL))
		write(2, "Mutex init failure\n", 19), exit(1);
}

/**
 * destroy_task_mutex - destroy mutual exception
*/
void __attribute__((destructor)) destroy_task_mutex(void)
{
	pthread_mutex_destroy(&mutex);
}

/**
 * create_task - creates task node & populates relevant member variable values
 * @entry: pointer to task entry function
 * @param: parameter to be passed to entry function
 * Return: pointer to created task struct
*/
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task;

	task = calloc(1, sizeof(task_t));
	if (!task)
		return (NULL);
	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	task->lock = mutex;
	return (task);
}

/**
 * destroy_task - destroy task nodes & task list
 * @task: pointer to task to destroy
*/
void destroy_task(task_t *task)
{
	if (!task)
		return;
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
	size_t id = 0;

	if (!tasks)
		return (NULL);
	for (tmp = tasks->head; tmp && id < tasks->size; tmp = tmp->next, ++id)
	{
		task = tmp->content;
		pthread_mutex_lock(&task->lock);
		if (task->status == PENDING)
		{
			task->status = STARTED;
			pthread_mutex_unlock(&task->lock);
			tprintf("[%02lu] Started\n", id);
			task->result = task->entry(task->param);
			task->status = task->result ? SUCCESS : FAILURE;
			tprintf("[%02lu] %s\n", id, task->result ? "Success" : "Failure");
		}
		else
			pthread_mutex_unlock(&task->lock);
	}
	return (NULL);
}
