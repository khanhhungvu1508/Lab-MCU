/*
 * Scheduler.h
 *
 *  Created on: Oct 16, 2021
 *      Author: PC
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include <stdio.h>

struct SCH_Task {
	void (*pFunc)(void);
	uint32_t delay;
	uint32_t period;
};

#define MAX_TASK 	5
#define TICK_TIME 	10
#define TIME_CYCLE	10
#define INF 		10000

void SCH_Init();
uint32_t SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
void SCH_Update(void);
void SCH_Sort_Delay(void);
void SCH_Dispatch_Tasks(void);
uint32_t get_time();

#endif /* INC_SCHEDULER_H_ */
