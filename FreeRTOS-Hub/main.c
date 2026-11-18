/**
 * @file main.c
 * @brief Generic FreeRTOS Multi-Tasking & Scheduling Framework
 * @author Shweta Arunkumar Bankapure
 * @portfolio Low-Level-Logic
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* --- Task Handle Definitions --- */
static TaskHandle_t Task1Handle = NULL;
static TaskHandle_t Task2Handle = NULL;
static TaskHandle_t Task3Handle = NULL;

/* --- Inter-Task Communication Primitives --- */
static QueueHandle_t xDataQueue = NULL;
static SemaphoreHandle_t xResourceMutex = NULL;

/* --- Generic Data Packet Structure --- */
typedef struct {
    uint32_t messageID;
    uint32_t timestamp;
    float payloadValue;
} DataPacket_t;

/* --- Function Prototypes --- */
static void prvTask1(void *pvParameters);
static void prvTask2(void *pvParameters);
static void prvTask3(void *pvParameters);

int main(void)
{
    /* 1. System Initialization Placeholder */
    printf("Initializing FreeRTOS Generic Scheduler Framework...\r\n");

    /* 2. Create Inter-Task Communication Objects */
    // Queue to hold up to 10 generic data packets
    xDataQueue = xQueueCreate(10, sizeof(DataPacket_t));
    
    // Mutex to protect shared resources
    xResourceMutex = xSemaphoreCreateMutex();

    if ((xDataQueue != NULL) && (xResourceMutex != NULL))
    {
        /* 3. Create FreeRTOS Tasks with Designated Priorities */
        
        // High Priority: Periodic Producer Task
        xTaskCreate(prvTask1, "Task1_Producer", 128, NULL, 3, &Task1Handle);

        // Medium Priority: Consumer / Processor Task
        xTaskCreate(prvTask2, "Task2_Processor", 128, NULL, 2, &Task2Handle);

        // Low Priority: Background / Maintenance Task
        xTaskCreate(prvTask3, "Task3_Background", 128, NULL, 1, &Task3Handle);

        /* 4. Start Kernel Scheduler */
        vTaskStartScheduler();
    }
    else
    {
        printf("Error: Failed to allocate kernel objects!\r\n");
    }

    /* Execution should never reach here unless heap allocation fails */
    for (;;);
}

/* ========================================================================= */
/* TASK IMPLEMENTATIONS                                                      */
/* ========================================================================= */

/**
 * @brief Task 1 (High Priority): Periodic Data Producer
 */
static void prvTask1(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // 100ms periodic execution
    uint32_t counter = 0;

    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        DataPacket_t packet;
        packet.messageID = ++counter;
        packet.timestamp = xTaskGetTickCount();
        packet.payloadValue = (float)(counter * 1.25f);

        // Send packet to queue with 10ms timeout
        if (xQueueSend(xDataQueue, &packet, pdMS_TO_TICKS(10)) != pdPASS)
        {
            // Handle queue send error / buffer full condition
        }

        // Maintain precise periodic timing
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief Task 2 (Medium Priority): Event-Driven Processor
 */
static void prvTask2(void *pvParameters)
{
    DataPacket_t receivedPacket;

    for (;;)
    {
        // Block indefinitely until new data arrives in the queue
        if (xQueueReceive(xDataQueue, &receivedPacket, portMAX_DELAY) == pdPASS)
        {
            // Safely protect shared state using the Mutex
            if (xSemaphoreTake(xResourceMutex, portMAX_DELAY) == pdTRUE)
            {
                // Process payload data securely here
                // printf("[Task2] ID: %lu | Value: %.2f\r\n", receivedPacket.messageID, receivedPacket.payloadValue);
                
                xSemaphoreGive(xResourceMutex);
            }
        }
    }
}

/**
 * @brief Task 3 (Low Priority): Background Maintenance / Heartbeat
 */
static void prvTask3(void *pvParameters)
{
    for (;;)
    {
        // Perform non-critical background routines or system monitoring
        vTaskDelay(pdMS_TO_TICKS(1000)); // Runs every 1 second
    }
}