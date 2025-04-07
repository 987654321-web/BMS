#include "freertos_code.h"
#include "stdio.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "KEY.h"
#include "usart.h"
/*freertos相关的头文件，必须的*/
#include "FreeRTOS.h"
#include "task.h"

/*启动任务的配置*/
#define START_TASK_STACK 128
#define START_TASK_priority 1
TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

/*任务1的配置*/
#define TASK1_STACK 128
#define TASK1_priority 2
TaskHandle_t task1_handle;
void task1(void *pvParameters);

/*任务2的配置*/
#define TASK2_STACK 128
#define TASK2_priority 1
TaskHandle_t task2_handle;
void task2(void *pvParameters);

/*任务3的配置*/
#define TASK3_STACK 128
#define TASK3_priority 3
TaskHandle_t task3_handle;
void task3(void *pvParameters);

/**
 *@description:启动FreeRTOS
 *@return {*}
 */
void freertos_start(void)
{
	/*1.动态创建一个启动任务*/
	xTaskCreate((TaskFunction_t)start_task,				  // 任务函数的地址
				(char *)"start_task",					  // 任务名字（可以自定义）
				(configSTACK_DEPTH_TYPE)START_TASK_STACK, // 任务栈大小。默认是128，单位是4字节
				(void *)NULL,							  // 传递给任务的参数
				(UBaseType_t)START_TASK_priority,		  // 任务的优先级
				(TaskHandle_t *)&start_task_handle);	  // 任务句柄的地址
	/*2.启动调度器*/
	vTaskStartScheduler();
}

/**
 *@description: 启动任务 ：用来创建其他task
 *@param {void} *pvParameters
 *@return {*}
 */
void start_task(void *pvParameters)
{
	/*进入临界区：保护临界区内的代码在执行时不被打断*/
	taskENTER_CRITICAL();

	/*1.动态创建task1*/
	xTaskCreate((TaskFunction_t)task1,				 // 任务函数的地址
				(char *)"task1",					 // 任务名字（可以自定义）
				(configSTACK_DEPTH_TYPE)TASK1_STACK, // 任务栈大小。默认是128，单位是4字节
				(void *)NULL,						 // 传递给任务的参数
				(UBaseType_t)TASK1_priority,		 // 任务的优先级
				(TaskHandle_t *)&task1_handle);		 // 任务句柄的地址

	/*2. 动态创建task2*/
	xTaskCreate((TaskFunction_t)task2,				 // 任务函数的地址
				(char *)"task2",					 // 任务名字（可以自定义）
				(configSTACK_DEPTH_TYPE)TASK2_STACK, // 任务栈大小。默认是128，单位是4字节
				(void *)NULL,						 // 传递给任务的参数
				(UBaseType_t)TASK2_priority,		 // 任务的优先级
				(TaskHandle_t *)&task2_handle);		 // 任务句柄的地址

	/*3. 动态创建task3*/
	xTaskCreate((TaskFunction_t)task3,				 // 任务函数的地址
				(char *)"task3",					 // 任务名字（可以自定义）
				(configSTACK_DEPTH_TYPE)TASK3_STACK, // 任务栈大小。默认是128，单位是4字节
				(void *)NULL,						 // 传递给任务的参数
				(UBaseType_t)TASK3_priority,		 // 任务的优先级
				(TaskHandle_t *)&task3_handle);		 // 任务句柄的地址

	/*启动任务只需要执行一次即可，用完就删除自己*/
	vTaskDelete(NULL);
	/*退出临界区*/
	taskEXIT_CRITICAL();
}

/**
 *@description: task1任务:实现LED0每500ms闪烁一次
 *@param {void} *pvParameters
 *@return {*}
 */
void task1(void *pvParameters)
{
	while (1)
	{
		printf("task1 is running...\r\n");
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		vTaskDelay(100); // 延时500ms
	}
}

/**
 *@description: task2任务:实现LED1每500ms闪烁一次
 *@param {void} *pvParameters
 *@return {*}
 */
void task2(void *pvParameters)
{
	while (1)
	{
		printf("task2 is running...\r\n");
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		vTaskDelay(100); // 延时500ms
	}
}

/**
 *@description: task3任务
 *@param {void} *pvParameters
 *@return {*}
 */
void task3(void *pvParameters)
{
	while (1)
	{
		if (Is_Button_Pressed())
		{
			if (task1_handle != NULL)
			{
				/*删除任务1*/
				vTaskDelete(task1_handle);
			}
		}
		vTaskDelay(500); // 延时500ms
	}
}
