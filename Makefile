CC = gcc

FREERTOS_DIR = < Enter your repo location >


CFLAGS = -I$(FREERTOS_DIR) \
         -I$(FREERTOS_DIR)/FreeRTOS-Kernel/include \
         -I$(FREERTOS_DIR)/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix \
         -I$(FREERTOS_DIR)/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils \
         -Dposix \
         -Wall -Wextra  -pedantic -std=c99 -g -O0

LDFLAGS = -lpthread -lrt


SRC_FILES = main.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/list.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/queue.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/tasks.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/timers.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/portable/MemMang/heap_4.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/port.c \
            $(FREERTOS_DIR)/FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c

OBJ_FILES = $(SRC_FILES:.c=.o)

.PHONY: all clean

all: rtos_example

rtos_example: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_FILES) rtos_example