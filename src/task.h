/** @file   task.h
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   7 October 2014
    @brief  modified task scheduler header file
*/

#ifndef TASKER_H
#define TASKER_H

#include "system.h"
#include "timer.h"


#define TASK_RATE TIMER_RATE


/** Task function prototype.  
Modified function prototype that do not takes a parameter
*/
typedef void (* task_func_t)(void);


/** Task structure.  */
typedef struct task_struct
{
    /** Function to call.  */
    task_func_t func;
    /** Generic pointer to pass to function.  */
    void *data;
    /** How many ticks between scheduling events.  */
    timer_tick_t period;    
    /** When to reschedule.  */
    timer_tick_t reschedule;
} task_t;

/**
 * Terminate the task wherever this function is called
 */
void task_stop(void);

/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void task_schedule (task_t *tasks, uint8_t num_tasks);

#endif