# FreeRTOS Projects on STM32F407 MCU

## 001_Segger
- Integrated the FreeRTOS kernel source into the project.
- Created two tasks to print on the SWV ITM Data Console to observe task execution.
- Integrated Segger SystemView to analyze and visualize task execution.

## 002_LED
- Created three tasks with different periods: task 1 every 400ms, task 2 every 800ms, and task 3 every 1000ms.
- Implemented tasks using `HAL_DELAY()`, which led to improper task execution and the MCU operating at full load without an IDLE state.

## 003_LED_Block_Tasks
- Replaced `HAL_DELAY()` with `vTaskDelay()` to synchronize tasks and manage delays more effectively.

## 004_LED_Periodic_Tasks
- Used `vTaskDelayUntil()` instead of `vTaskDelay()` for precise periodic execution, avoiding task drift.

## 005_Task_Notify
- Implemented task-to-task notifications using FreeRTOS direct notification APIs.
- Created three tasks to toggle different LEDs and a task to poll for button presses every 10ms. When a button press is detected, it sends a notification to the LED toggling task, which then deletes itself.

## 006_Button_ISR
- Implemented ISR-to-task notifications using FreeRTOS direct notification APIs.
- Created three tasks to toggle different LEDs. When a button press occurs, the interrupt handler sends a notification to the current LED toggling task, which then deletes itself.

## 008_Queues_Menu
- Developed a menu system with options for controlling LED effects, viewing or configuring the date and time, or exiting.
- Selecting "LED effect" (option 0) allows choosing one of four effects (e1, e2, e3, or e4).
- Selecting "Date and Time" (option 1) displays the current date and time and allows configuration. The updated settings are confirmed and displayed.
- The menu reappears after each action, allowing further choices.

![Queue_Menu_Screen](https://github.com/user-attachments/assets/2fba9ac0-2b32-4e25-92e3-66e487188875)

## 009_Binary_Semaphore
- Used a binary semaphore for synchronization between two tasks.
- The interrupt triggered by a button press uses `xSemaphoreGiveFromISR()` to release the semaphore.
- `Task_Semaph` waits for the semaphore using `xSemaphoreTake()` and continues execution upon release.
- `Task_Printf` runs periodically and prints messages using `Uart_Print()`, independent of the semaphore.

![Binary_Semaphore](https://github.com/user-attachments/assets/608905e8-a98d-45ba-a517-f89e808747ad)

## 010_Counter_Semaphore
- Used a counting semaphore for synchronization between two tasks.
- The interrupt triggered by a button press uses `xSemaphoreGiveFromISR()` to release the semaphore multiple times.
- `task1` runs periodically every 1 second and prints messages using `Uart_Print()`.
- `task_semaph` waits for the semaphore using `xSemaphoreTake()` and continues execution three times based on the semaphore count.

![Counter_Semaphore_Output](https://github.com/user-attachments/assets/140f6daa-2bb9-4617-8f3b-f1b02e6e48d7)

## 011_Mutual_Exclusion_ByBinary_Semaphore
This project demonstrates the use of semaphores in a FreeRTOS environment on an STM32F407 microcontroller. 
It includes two tasks that use a binary semaphore for synchronization, ensuring mutual exclusion when accessing shared resources.

![image](https://github.com/user-attachments/assets/6ef0849a-bd1b-428c-8c08-83bb7de72df1)

