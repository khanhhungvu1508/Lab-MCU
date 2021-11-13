/*
 * Scheduler.c
 *
 *  Created on: Oct 16, 2021
 *      Author: PC
 */

#include "main.h"
#include "Scheduler.h"
#include <stdio.h>
#include <stdlib.h>

struct SCH_Task SCH_task[MAX_TASK];
uint32_t time;
uint32_t newest;
int updated;
UART_HandleTypeDef huart1;

void SCH_Init(void) {
	for(int i = 0; i < MAX_TASK; i++) {
		SCH_task[i].delay = 0;
		SCH_task[i].pFunc = NULL;
		SCH_task[i].period = 0;
	}
	time = 0;
	newest = 0;
	updated = -1;
}

uint32_t SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
	for (int i = 0; i < MAX_TASK; i++)
	{
		if (SCH_task[i].pFunc == 0) {
			SCH_task[i].pFunc = pFunction;
			SCH_task[i].delay = DELAY;
			SCH_task[i].period = PERIOD;
			return i;
		}
	}
	return -1;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (taskID < 0 || taskID >= MAX_TASK) {
		return 0;
	}
	SCH_task[taskID].pFunc = NULL;
	SCH_task[taskID].delay = INF;
	SCH_task[taskID].period = 0;
	return 1;
}

void SCH_Update(void) {
	time++;
	if ((newest + SCH_task[0].delay) == time){
		newest = time;
		updated = 1;
	}
}

int compareTask (const void *task1, const void *task2)
{
	struct SCH_Task *Task1 = (struct SCH_Task *)task1;
	struct SCH_Task *Task2 = (struct SCH_Task *)task2;
	return (Task1 -> delay - Task2 -> delay);
}

void SCH_Sort_Delay(void){
	qsort(SCH_task, MAX_TASK, sizeof(struct SCH_Task), compareTask);
}

void SCH_Dispatch_Tasks(void) {
	if (updated == 1){
		int time_passed = SCH_task[0].delay;
		for (int i = 0; i < MAX_TASK; i++){
			SCH_task[i].delay = SCH_task[i].delay - time_passed;
			if (SCH_task[i].delay == 0 && SCH_task[i].pFunc){
				(*SCH_task[i].pFunc)();
				if (SCH_task[i].period)
					SCH_task[i].delay = SCH_task[i].period;
				else SCH_Delete_Task(i);
			}
		}
		SCH_Sort_Delay();
		updated = 0;
	}
	else if (updated == -1){
		SCH_Sort_Delay();
		updated = 0;
	}
}

uint32_t get_time(){
	return time * TICK_TIME;
}
