#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* Application specific definitions. */
#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             1  // Changed to 1
#define configUSE_TICK_HOOK             0
#define configTICK_RATE_HZ              ( 1000 )
#define configMINIMAL_STACK_SIZE        ( 128 )  // Increased from 128
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 4 * 1028 ) )  // Increased to 4MB
#define configMAX_TASK_NAME_LEN         ( 16 )
#define configUSE_TRACE_FACILITY        1
#define configUSE_16_BIT_TICKS          0
#define configIDLE_SHOULD_YIELD         1
#define configUSE_MUTEXES               1
#define configQUEUE_REGISTRY_SIZE       8
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_RECURSIVE_MUTEXES     1
#define configUSE_MALLOC_FAILED_HOOK    1
#define configUSE_APPLICATION_TASK_TAG  0
#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0

#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* Software timer definitions. */
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       ( 2 )
#define configTIMER_QUEUE_LENGTH        10
#define configTIMER_TASK_STACK_DEPTH    ( 2 * configMINIMAL_STACK_SIZE )  // Doubled

/* Task priorities. 1 is the lowest priority, 5 is the highest. */
#define configMAX_PRIORITIES            ( 5 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskDelay              1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_xTaskGetIdleTaskHandle         1
#define INCLUDE_eTaskGetState                  1
#endif /* FREERTOS_CONFIG_H */