/** @file   task.c (modified)
    @author M. P. Hayes, UCECE & Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   17 August 2011
    @brief  Simple task scheduler.
*/
#include "system.h"
#include "task.h"
#include "timer.h"


/** With 16-bit times the maximum value is 32768.  */
#define TASK_OVERRUN_MAX 32767
static uint8_t stop_signal = 0;


/**
 * Terminate the task wherever this function is called
 */
void task_stop(void){
    stop_signal = 1;
}

/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void task_schedule (task_t *tasks, uint8_t num_tasks)
{
    uint8_t i;
    timer_tick_t now;
    task_t *next_task;

    timer_init ();
    now = timer_get ();

    /* Start by scheduling the first task.  */
    next_task = tasks;

    while (1)
    {
        if (stop_signal){
            stop_signal = 0;
            return;
        }
        timer_tick_t sleep_min;

        /* Wait until the next task is ready to run.  */
        timer_wait_until (next_task->reschedule);

        /* Schedule the task.  */
        next_task->func ();

        /* Update the reschedule time.  */
        next_task->reschedule += next_task->period;

        sleep_min = ~0;
        now = timer_get ();

        /* Search array of tasks.  Schedule the first task (highest priority)
           that needs to run otherwise wait until first task ready.  */
        for (i = 0; i < num_tasks; i++)
        {
            task_t * task = tasks + i;
            timer_tick_t overrun;

            overrun = now - task->reschedule;
            if (overrun < TASK_OVERRUN_MAX)
            {
                /* Have found a task that can run immediately.  */
                next_task = task;
                break;
            }
            else
            {
                timer_tick_t sleep;

                sleep = -overrun;
                if (sleep < sleep_min)
                {
                    sleep_min = sleep;
                    next_task = task;
                }
            }
        }
    }
}
