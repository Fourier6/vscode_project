#include "task_all.h"
QueueHandle_t pid_queue;
void creat_all_task(void)
{
    pid_queue = xQueueCreate(1, sizeof(pid_dat));
    xTaskCreatePinnedToCore(
        task_mpu6500,          // 任务函数
        "mpu6500",        // 任务名称
        10000,          // 任务堆栈大小
        NULL,           // 任务参数
        2,              // 任务优先级
        &Task_mpu6500,         // 任务句柄
        0               // 核心编号
    );
    xTaskCreatePinnedToCore(
        task_motor,          // 任务函数
        "motor",        // 任务名称
        10000,          // 任务堆栈大小
        NULL,           // 任务参数
        1,              // 任务优先级
        &Task_motor,         // 任务句柄
        1               // 核心编号
    );
    xTaskCreatePinnedToCore(
        task_web,          // 任务函数
        "web",        // 任务名称
        30000,          // 任务堆栈大小
        NULL,           // 任务参数
        1,              // 任务优先级
        &Task_web,         // 任务句柄
        0               // 核心编号
    );
}