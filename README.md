# FreeRTOS for Beginners

FreeRTOS is a real-time operating system kernel designed for embedded systems. It helps developers manage time-sensitive tasks efficiently. This README provides a beginner-friendly introduction to important concepts like Deadlock, ISR, Timer, Semaphore, Mutex, Queue, Memory, and Simple Tasks in FreeRTOS.

## Table of Contents
- [Introduction to FreeRTOS](#introduction-to-freertos)
- [Concepts](#concepts)
  - [Simple Task](#simple-task)
  - [ISR (Interrupt Service Routine)](#isr-interrupt-service-routine)
  - [Timer](#timer)
  - [Semaphore](#semaphore)
  - [Mutex](#mutex)
  - [Queue](#queue)
  - [Memory Management](#memory-management)
  - [Deadlock](#deadlock)

----------------------------------------- Introduction to FreeRTOS -----------------------------------------------------
## Introduction to FreeRTOS
   
    FreeRTOS is a lightweight, open-source operating system designed for microcontrollers and small embedded systems. It is used for scheduling tasks in a predictable manner, ensuring real-time performance where deadlines are crucial. 

    FreeRTOS is a real-time operating system (RTOS) for microcontrollers that can be used in embedded systems. RTOS schedulers are designed to provide a predictable execution pattern, which is important for embedded systems that have real-time requirements. 

----------------------------------------------------- Concepts --------------------------------------------------------

## Concepts

### Simple Task

A **task** is the fundamental unit of execution in FreeRTOS. Each task runs in its own thread of execution, performing specific functions. Tasks can be created, suspended, resumed, or deleted, and they typically include an infinite loop where they perform operations continuously or wait for specific events.

### ISR (Interrupt Service Routine)

An **ISR** is a function that is called in response to hardware interrupts. In FreeRTOS, ISRs should be short and cannot call blocking APIs directly. However, mechanisms like semaphores or queues can be used to notify tasks from within an ISR.

### Timer

FreeRTOS offers **software timers** that can execute functions at specified intervals. These timers are useful for periodic operations, allowing developers to trigger tasks or actions without the need to dedicate a full task to timing functionality.

### Semaphore

A **semaphore** is a synchronization primitive used in FreeRTOS to manage access to shared resources or to signal task events. Semaphores can be binary or counting, allowing for flexible synchronization between tasks or between tasks and ISRs.

### Mutex

A **mutex** (mutual exclusion) is used to ensure exclusive access to shared resources, preventing data corruption when multiple tasks attempt to access the same resource. FreeRTOS mutexes include priority inheritance, which helps mitigate the risk of priority inversion.

### Queue

A **queue** is used for communication between tasks or between tasks and ISRs. It provides a safe way to pass data between tasks, ensuring data integrity and synchronization in concurrent environments.

### Memory Management

FreeRTOS supports both dynamic and static memory allocation for tasks and other system components. While dynamic memory allocation can simplify task creation, static allocation can be preferred in systems with limited memory to prevent fragmentation and ensure reliability.

### Deadlock

**Deadlock** occurs when tasks are waiting for each other to release resources, causing the system to become stuck. This can happen when two tasks hold resources and wait indefinitely for the other to release a resource. Avoiding deadlock requires careful planning when acquiring resources like semaphores or mutexes, and using timeouts can help mitigate the risk.

--------------------------------------------- real-time examples -------------------------------------------------

FreeRTOS is widely used in embedded systems across various industries. Below are some common applications of FreeRTOS in real-time environments:

### Managing Multiple Tasks

FreeRTOS can manage and schedule multiple tasks efficiently, even on multi-core processors. For example, in IoT-connected vending machines, tasks such as playing promotional videos and processing beverage selections can run concurrently without interfering with each other.

### Data Collection and Processing

In industrial environments, FreeRTOS is used to collect performance data from sensors and take real-time actions to prevent equipment failures. By processing sensor data locally, FreeRTOS-based systems can react quickly to critical events.

### Remote Device Updates

FreeRTOS devices can be remotely managed using AWS IoT Device Management or similar services. This enables secure over-the-air (OTA) updates for devices, ensuring they stay up-to-date with the latest firmware and security patches.

### Other Use Cases

FreeRTOS has been deployed in a wide range of embedded systems, including:
- Smart meters for energy monitoring
- Oil pump sensors for industrial automation
- Home appliances with connected features
- Commercial security systems
- Fitness trackers and wearables
- Wireless sensor networks for environmental monitoring


For more information, refer to the official [FreeRTOS documentation](https://www.freertos.org/).


--------------------------------------------- Build setup -------------------------------------------------

Prerequisites

1. git
2. Make
3. gcc


Steps to build and run the rtos code in linux:

1. git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git

2. Create your own config File.

3. Update the Make File with proper path

4. run makeall command.

5. Run the rtos_example binary in linux.




