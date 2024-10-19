#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "FreeRTOSConfig.h"


// ------------ Define examples here: -----------------
// #define PRIORITY_INVERSION
// #define DEADLOCK
// #define ISR
// #define TIMER
// #define SEMAPHORE
// #define MUTEX
// #define QUEUE
// #define MEMORY
#define SIMPLE_TASK
// ----------------------------------------------------


// ------------ Include the required Libraries: -----------------


#ifdef QUEUE
    #include "queue.h"
#endif

#ifdef MUTEX
     #include "semphr.h"
#endif

#ifndef SEMAPHORE
     #include "semphr.h"
#endif

#ifdef TIMER
    #include "timers.h"
#endif

#ifdef ISR
    #include "semphr.h"
    #include "timers.h"
#endif

#ifdef DEADLOCK
    #include "semphr.h"
    #include "timers.h"
#endif

#ifdef PRIORITY_INVERSION
    #include "semphr.h"
    #include "timers.h"
#endif

// ------------------------------------------------------ Simple Task Create --------------------------------------------------------------

// Simple task function example
#ifdef SIMPLE_TASK
{
    static volatile int taskCounter = 0;
    void PrintTask(void *pvParameters) {
        const TickType_t xDelay = pdMS_TO_TICKS(500);
        for (;;) {
            taskCounter++;
            printf("Hello, FreeRTOS! Counter: %d\n", taskCounter);
            fflush(stdout);
            vTaskDelay(xDelay);
        }
    }

    // Task with parameters example
    void taskN(void *pvParameters) {
        const TickType_t xDelay = pdMS_TO_TICKS(500);
        int taskNumber = *(int *)pvParameters;
        for (;;) {
            taskCounter++;
            printf("I am inside task %d. Counter: %d\n", taskNumber, taskCounter);
            fflush(stdout);
            vTaskDelay(xDelay);
        }
    }

}
#endif

// ------------------------------------------------------ Memory --------------------------------------------------------------

// Task that simulates memory usage
#ifdef MEMORY 
    void taskMemory(void *pvParameters) {
        int a = 0;
        while (1) {
            
            int b[1000];

            for (int i = 0; i < 1000; i++) {
                b[i] = a + 1;
                // printf("value of b[%d] = %d \n", i, b[i]);
            }
            a++;
            printf("Value of b[0] = %d\n", b[0]);
            fflush(stdout);

            printf("Heap before memory alloc \n");
            printf("Heap size = %d \n", xPortGetFreeHeapSize());
            fflush(stdout);

            int* prt = pvPortMalloc(10 * sizeof(int));

            if (prt == NULL) {
                printf("Heap allocation failed! No memory available.\n");
                fflush(stdout);
                break;
            }

            printf("Heap After memory alloc \n");
            printf("Heap size = %d \n", xPortGetFreeHeapSize());
            fflush(stdout);

            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
    }
#endif

// ------------------------------------------------------ Queue--------------------------------------------------------------

#ifdef QUEUE

    BaseType_t Max_Queue_size = 5;
    static QueueHandle_t queue1;
    static QueueHandle_t queue2;

    char string[9] = "Hii There";

    void send(void *pvParameters) {
        BaseType_t value = 0;
        while(1){
            if (xQueueSend(queue2, (void*) &string, 10) != pdTRUE){
                printf("failed to send message to task2 \n");
            }
            printf("Send = %s \n", string);
            value++;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

    void receive(void *pvParameters) {
        BaseType_t received_value; 
        char recv_string[10];
        while(1){
            if (xQueueReceive(queue2, (void*) &recv_string, 10) != pdTRUE){
                printf("Failed to receive value from task1\n");
            }
            printf("Receive  = %s \n", recv_string);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

#endif

// ------------------------------------------------------ MUTEX--------------------------------------------------------------

#ifdef MUTEX

    static SemaphoreHandle_t mutex;
    static int global_var = 0;

    void task1(void *pvParameters)
    {
        while(1)
        {
            if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                global_var++;
                printf("Value of global var in Task 1 = %d\n", global_var);

                vTaskDelay(pdMS_TO_TICKS(2000));

                xSemaphoreGive(mutex);
            }else{
                printf("Mutex is not released from Task 2 \n");
            }
            
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
    
   void task2(void *pvParameters)
    {
        while(1)
        {
            if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                global_var++;
                printf("Value of global var in Task 2 = %d \n", global_var);
                
                vTaskDelay(pdMS_TO_TICKS(1000));

                xSemaphoreGive(mutex);
            } else{
                printf("Mutex is not released from Task 1 \n");
            }
            
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

#endif

// ------------------------------------------------------ SEMAPHORE--------------------------------------------------------------


#ifdef SEMAPHORE
    #define BUFFER_SIZE 5

    typedef struct
    {
        int data[BUFFER_SIZE];
        int writeIndex;
        int readIndex;
    } SharedBuffer;

    static SemaphoreHandle_t xBufferMutex;
    static SemaphoreHandle_t xBufferCountSemaphore;
    static SemaphoreHandle_t xBufferFreeStatusSemaphore;

    SharedBuffer Buffer = {{0}, 0, 0};

    int readBuffer(){
        int number;
        xSemaphoreTake(xBufferMutex, portMAX_DELAY);

        number = Buffer.data[Buffer.readIndex];
        Buffer.readIndex = (Buffer.readIndex + 1) % BUFFER_SIZE;

        xSemaphoreGive(xBufferMutex);

        return number;
    }

    void writeBuffer(int item){
        xSemaphoreTake(xBufferMutex, portMAX_DELAY);

        Buffer.data[Buffer.writeIndex] = item;
        Buffer.writeIndex = (Buffer.writeIndex + 1) % BUFFER_SIZE;

        xSemaphoreGive(xBufferMutex);
    }

    void task1(void *pvParameters) {
        int variable = (int) pvParameters;

        while(1){
            xSemaphoreTake(xBufferFreeStatusSemaphore, portMAX_DELAY);

            writeBuffer(variable);
            printf("Add a value to buffer from task1 = %d\n", variable);

            xSemaphoreGive(xBufferCountSemaphore); 

            vTaskDelay(pdMS_TO_TICKS(1000)); 
        }
    }

    void task2(void *pvParameters){  
        while(1){
            xSemaphoreTake(xBufferCountSemaphore, portMAX_DELAY);

            int value = readBuffer();
            printf("Value read from Buffer in task 2 = %d\n", value); 

            xSemaphoreGive(xBufferFreeStatusSemaphore); 

            vTaskDelay(pdMS_TO_TICKS(1000)); 
        }
    }

#endif

// ------------------------------------------------------ Timer--------------------------------------------------------------

#ifdef TIMER

    void timerCallback(void* xTimer)
    {
        (void)xTimer;
        
        printf("Timer Callback\n");
        fflush(stdout);
    }

#endif

// ------------------------------------------------------ ISR--------------------------------------------------------------

#ifdef ISR

    TimerHandle_t xTimer;

    // Semaphore for ISR synchronization
    SemaphoreHandle_t xISRSemaphore;

    // Flag to indicate if ISR is active
    volatile BaseType_t xISRActive = pdFALSE;

    void vsampleTask()
    {
        printf("Sample Task\n");
        fflush(stdout);
        vTaskDelete(NULL);
    }

    void vtimerCallback(void* xTimer)
    {
        xTaskCreate(vsampleTask, "Samepl task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        printf("Created Sample Task\n");
    }

    void vISRTask(){
        TickType_t xLastWakeTime;
        const TickType_t xFrequency = pdMS_TO_TICKS(100); 

        xLastWakeTime = xTaskGetTickCount();

        for (int i = 0; i < 20; i++)  
        {
            printf("ISR Task: Interrupting for 2 seconds!\n");
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }

        xISRActive = pdFALSE;
        vTaskDelete(NULL);  
    }

    void vkeyBoardISR()
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // Signal the semaphore to trigger the ISR task
        xSemaphoreGiveFromISR(xISRSemaphore, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    // Main task to handle ISR triggering
    void vScanTask(void *pvParameters)
    {
        char cInput;

        for (;;)
        {
            // Simulated keyboard input
            scanf(" %c", &cInput);  // Note: This is blocking and not suitable for a real RTOS application
            printf("Enter value = %c \n", cInput);
            if (cInput == 'q' && xISRActive == pdFALSE)
            {
                vkeyBoardISR();  // Simulate ISR trigger
            }
        }
    }

    // This task waits for the ISR semaphore and creates the ISR task
    void vISRHandler(void *pvParameters)
    {
        for (;;)
        {
            // Wait for semaphore
            if (xSemaphoreTake(xISRSemaphore, portMAX_DELAY) == pdTRUE)
            {
                if (xISRActive == pdFALSE)
                {
                    xISRActive = pdTRUE;
                    xTaskCreate(vISRTask, "ISRTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
                }
            }
        }
    }

#endif

// ------------------------------------------------------ DEADLOCK--------------------------------------------------------------


#ifdef DEADLOCK

    static SemaphoreHandle_t mutex1;
    static SemaphoreHandle_t mutex2;


    void task1 (){
        printf("inside task1 \n");
        fflush(stdout);

        while(1)
        {
            if(xSemaphoreTake(mutex1, portMAX_DELAY) != pdFALSE){
                printf("Task1  takes mutex 1 \n");
                fflush(stdout);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                if(xSemaphoreTake(mutex2, portMAX_DELAY)!= pdFALSE){
                    printf("Task1  takes mutex 2 \n");
                    fflush(stdout);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    xSemaphoreGive(mutex2);
                    printf("Task1  gives mutex 2 \n");
                    fflush(stdout);
                }
                xSemaphoreGive(mutex1);
                printf("Task1  gives mutex 1 \n");
                fflush(stdout);
            }
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }

    void task2 (){
        printf("inside task 2 \n");
        fflush(stdout);

        if(xSemaphoreTake(mutex2, portMAX_DELAY) != pdFALSE){
            printf("Task2  takes mutex 2 \n");
            fflush(stdout);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            if(xSemaphoreTake(mutex1, portMAX_DELAY)!= pdFALSE){
                printf("Task2  takes mutex 1 \n");
                fflush(stdout);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                xSemaphoreGive(mutex1);
                printf("Task2  gives mutex 1 \n");
                fflush(stdout);
            }
            xSemaphoreGive(mutex2);
            printf("Task2  gives mutex 2 \n");
            fflush(stdout);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }

#endif

// ------------------------------------------------------ PRIORITY_INVERSION--------------------------------------------------------------


#ifdef PRIORITY_INVERSION
    static SemaphoreHandle_t mutexD;

    void taskL(void *pvParameters) {
        while(1) {
            if(xSemaphoreTake(mutexD, portMAX_DELAY) == pdTRUE) {
                printf("TaskL takes mutex\n");
                fflush(stdout);
                vTaskDelay(pdMS_TO_TICKS(1500));
                xSemaphoreGive(mutexD);
                printf("TaskL gives mutex\n");
                fflush(stdout);
            }
        }
    }

    void taskH(void *pvParameters) {
        vTaskDelay(pdMS_TO_TICKS(50));
        while(1) {
            if(xSemaphoreTake(mutexD, portMAX_DELAY) == pdTRUE) {
                printf("TaskH takes mutex\n");
                fflush(stdout);
                vTaskDelay(pdMS_TO_TICKS(500));
                xSemaphoreGive(mutexD);
                printf("TaskH gives mutex\n");
                fflush(stdout);
            }
        }
    }

    void taskM(void *pvParameters) {
        vTaskDelay(pdMS_TO_TICKS(100));
        while(1) {
            printf("TaskM - I am inside taskM\n");
            fflush(stdout);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
#endif


// ------------------------------------------------------ Main--------------------------------------------------------------

int main(void) {
    BaseType_t xReturned;

    printf("Starting main()\n");
    fflush(stdout);

    #ifdef SIMPLE_TASK
    {
        // Create the simple print task
        xReturned = xTaskCreate(PrintTask, "Print Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        if (xReturned != pdPASS) {
            printf("Failed to create Print Task = %ld\n", xReturned);
            return 1;
        } 

        // Create multiple tasks with parameters
        for (int i = 1; i <= 3; i++) {  // Reduced to 3 tasks for simplicity
            int *number = pvPortMalloc(sizeof(int));
            if (number == NULL) {
                printf("Failed to allocate memory for task %d\n", i);
                return 1;
            }
            *number = i; 
            xReturned = xTaskCreate(taskN, "Task", configMINIMAL_STACK_SIZE, (void *)number, 1, NULL);
            if (xReturned != pdPASS) {
                printf("Failed to create Task %d , status = %ld\n", i, xReturned);
                vPortFree(number);  // Free memory if task creation fails
                return 1;
            } else {
                printf("Created Task %d\n", i);
                fflush(stdout);
            }
        }
    }
    #endif  

    #ifdef MEMORY
    {
        // Create the memory monitoring task
        xReturned = xTaskCreate(taskMemory, "Memory Monitor", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if (xReturned != pdPASS) {
            printf("Failed to create Memory Monitor task\n");
            return 1;
        }
    }
    #endif 

    #ifdef QUEUE
    { 
        // create the queue1

        // queue1 = xQueueCreate(Max_Queue_size, 64);
        queue2 = xQueueCreate(Max_Queue_size, sizeof(char[10]));

        xReturned = xTaskCreate(send, "Send Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned != pdPASS){
            printf("Failed to create the send task");
        }

        xReturned = xTaskCreate(receive, "Receive Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned != pdPASS){
            printf("Failed to create the receive Task");
        }
    }
    #endif


    #ifdef MUTEX
    {
        mutex = xSemaphoreCreateMutex();
        if (mutex == NULL)
        {
            printf("Failed to create mutex\n");
        }

        xReturned = xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        if(xReturned != pdPASS){
            printf("Failed to create Task 1");
        }

        xReturned = xTaskCreate(task2, "TASK 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned != pdPASS){
            printf("Failed to create Task 2 ");
        }

    }
    #endif

    #ifdef SEMAPHORE
    {
        BaseType_t xReturned; 

        xBufferMutex = xSemaphoreCreateMutex();
        xBufferCountSemaphore = xSemaphoreCreateCounting(BUFFER_SIZE, 0);
        xBufferFreeStatusSemaphore = xSemaphoreCreateCounting(BUFFER_SIZE, BUFFER_SIZE);

        if (xBufferMutex == NULL || xBufferCountSemaphore == NULL || xBufferFreeStatusSemaphore == NULL) {
            printf("Failed to create semaphores\n");
            return 1;
        }

        xReturned = xTaskCreate(task1, "Writer 1", configMINIMAL_STACK_SIZE, (void*)1, 1, NULL);
        if(xReturned != pdPASS) {
            printf("Failed to create Writer 1 Task\n");
        }

        xReturned = xTaskCreate(task1, "Writer 2", configMINIMAL_STACK_SIZE, (void*)2, 1, NULL);
        if(xReturned != pdPASS) {
            printf("Failed to create Writer 2 Task\n");
        }

        xReturned = xTaskCreate(task2, "Reader 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned != pdPASS) {
            printf("Failed to create Reader 1 Task\n");
        }

        xReturned = xTaskCreate(task2, "Reader 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned != pdPASS) {
            printf("Failed to create Reader 2 Task\n");
        }
    }
    #endif

    #ifdef TIMER
    {
                
        TimerHandle_t timer = NULL;

        timer = xTimerCreate(
            "test timer",                  // Timer name
            pdMS_TO_TICKS(2000),           // Timer period in ticks
            pdTRUE,                       // Auto-reload
            (void*)0,                      // Timer ID
            timerCallback                  // Callback function
        );

        if (timer == NULL)
        {
            // Timer creation failed
            printf("Failed to create timer\n");
        }
        else
        {
            // Start the timer
            if (xTimerStart((TimerHandle_t)timer, 0) != pdPASS)
            {
                printf("Failed to start timer\n");
            }
        }
    }
    #endif

    #ifdef ISR
    {
        // Create timer
        xTimer = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, 0, vtimerCallback);

        // Create semaphore
        xISRSemaphore = xSemaphoreCreateBinary();

        // Start timer
        xTimerStart(xTimer, 0);

        // Create main task
        xTaskCreate(vScanTask, "MainTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

        // Start the scheduler
        vTaskStartScheduler();

        // We should never get here
        for (;;);

    }
    #endif

    #ifdef DEADLOCK

        mutex1 = xSemaphoreCreateMutex();
        mutex2 = xSemaphoreCreateMutex();

        if(!mutex1 || !mutex2){
            printf("Failed to create mutexes");
        }

        xReturned = xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned!= pdPASS){
            printf("Failed to create Task 1");
        }

        xReturned = xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if(xReturned!= pdPASS){
            printf("Failed to create Task 2 ");
        }

    #endif

    #ifdef PRIORITY_INVERSION

        mutexD = xSemaphoreCreateMutex();

        if (mutexD == NULL) {
            printf("Failed to create mutex\n");
            return 1;
        }

        xReturned = xTaskCreate(taskL, "Task L", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        if (xReturned != pdPASS) {
            printf("Failed to create Task L\n");
            fflush(stdout);
            return 1;
        }

        // vTaskDelay(pdMS_TO_TICKS(50));

        xReturned = xTaskCreate(taskM, "Task M", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        if (xReturned != pdPASS) {
            printf("Failed to create Task M\n");
            fflush(stdout);
            return 1;
        }

        // vTaskDelay(pdMS_TO_TICKS(50));

        xReturned = xTaskCreate(taskH, "Task H", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
        if (xReturned != pdPASS) {
            printf("Failed to create Task H\n");
            fflush(stdout);
            return 1;
        }
    #endif

    printf("Starting scheduler...\n");
    fflush(stdout);

    // Start the scheduler
    vTaskStartScheduler();

    // The scheduler should never return, so this code should never run
    printf("Scheduler has stopped!\n");
    fflush(stdout);
    for (;;);

    return 0;
}

