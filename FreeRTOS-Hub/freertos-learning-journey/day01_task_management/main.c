/**
 * @file    day01_task_management.c
 * @brief   Day 1: Introduction to FreeRTOS Task Creation and Priorities.
 * @details This firmware demonstrates the creation of two periodic tasks running 
 *          at different priorities using xTaskCreate(). It utilizes vTaskDelay() 
 *          for precise, non-blocking timing, allowing the CPU to yield to the 
 *          idle task or other ready tasks.
 * 
 * @note    Target: Generic ARM Cortex-M / FreeRTOS Kernel Port
 *          Author: Shweta Arunkumar Bankapure
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ================================================================= *
 *                       TASK HANDLES & PROTOTYPES                   *
 * ================================================================= */
static TaskHandle_t xTask1Handle = NULL;
static TaskHandle_t xTask2Handle = NULL;

/* 
 * @brief Unified task function implemented for multiple task instances.
 * @param pvParameters Pointer to string message passed during task creation.
 */
static void prvPeriodicTask( void *pvParameters );

/* ================================================================= *
 *                          MAIN ENTRY POINT                         *
 * ================================================================= */
int main( void )
{
    /* 1. Hardware Initialization 
     * (e.g., HAL_Init(), SystemClock_Config(), GPIO setups would go here) */
    // prvSetupHardware();

    /* 2. Create Task 1 (Higher Priority)
     * Parameters breakdown:
     *   - Task function pointer
     *   - Descriptive debug name
     *   - Stack depth in *words* (not bytes)
     *   - Parameter passed to task function
     *   - Task Priority (Higher number = Higher priority)
     *   - Pointer to task handle (optional)
     */
    BaseType_t xStatus1 = xTaskCreate(
        prvPeriodicTask,          
        "Task1",                  
        128,                      
        (void *)"[Task 1] Running (Priority 2)", 
        2,                        /* Priority 2 (Higher) */
        &xTask1Handle             
    );

    /* 3. Create Task 2 (Lower Priority) */
    BaseType_t xStatus2 = xTaskCreate(
        prvPeriodicTask,
        "Task2",
        128,
        (void *)"[Task 2] Running (Priority 1)",
        1,                        /* Priority 1 (Lower) */
        &xTask2Handle
    );

    /* 4. Validate Heap Allocation */
    if ( (xStatus1 == pdPASS) && (xStatus2 == pdPASS) )
    {
        /* Start the FreeRTOS real-time scheduler. 
         * Execution control transfers to the tasks here and never returns to main(). */
        vTaskStartScheduler();
    }
    else
    {
        /* Error handling: Insufficient heap memory available to create tasks */
        // Error_Handler();
    }

    /* Execution should never reach this loop unless the scheduler fails/crashes */
    for( ;; );
}

/* ================================================================= *
 *                       TASK IMPLEMENTATIONS                        *
 * ================================================================= */
static void prvPeriodicTask( void *pvParameters )
{
    char *pcMessage;
    
    /* Convert 500 milliseconds into RTOS ticks using configTICK_RATE_HZ */
    const TickType_t xDelayPeriod = pdMS_TO_TICKS( 500 ); 

    /* Safely retrieve the task parameter */
    pcMessage = (char *) pvParameters;

    /* FreeRTOS tasks must *never* return or exit. They must be infinite loops. */
    for( ;; )
    {
        /* Print/log the message (Ensure thread-safe logging mechanism if multithreaded) */
        vPrintString( pcMessage );

        /* Block the task for 500ms. 
         * Unlike a blocking for-loop/delay, vTaskDelay() frees up the CPU 
         * so lower-priority tasks or the Idle task can execute. */
        vTaskDelay( xDelayPeriod );
    }
}