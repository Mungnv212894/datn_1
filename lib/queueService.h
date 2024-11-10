#pragma once
#ifndef __QUEUE_SERVICE
#define __QUEUE_SERVICE

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define ITEMS_NUM 30
#define ITEM_SIZE 700

extern QueueHandle_t queue;


#endif 