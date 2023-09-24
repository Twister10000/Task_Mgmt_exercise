/*
 * Embedded_Systems_Übung_2.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : chaos
 */ 

//#include <avr/io.h>
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"
#include "ButtonHandler.h"


extern void vApplicationIdleHook( void );
void vButtonTask(void *pvParameters);
void vLEDTASK1(void *pvParameters);
void vLEDTASK2(void *pvParameters);
void vLEDTASK3(void *pvParameters);
void vLEDTASK4(void *pvParameters);

TaskHandle_t ledTask;
TaskHandle_t LED1;
TaskHandle_t LED2;
TaskHandle_t LED3;
TaskHandle_t LED4;
void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();
	
	PORTF.DIRSET = 0x0F;
	PORTE.DIRSET = 0x0F;
	
	
	xTaskCreate(vButtonTask, (const char *) "btTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(vLEDTASK1, (const char *) "LEDTSK1", configMINIMAL_STACK_SIZE, NULL, 0, &LED1);
	xTaskCreate(vLEDTASK2, (const char *) "LEDTSK2", configMINIMAL_STACK_SIZE, NULL, 0, &LED2);
	xTaskCreate(vLEDTASK3, (const char *) "LEDTSK3", configMINIMAL_STACK_SIZE, NULL, 0, &LED3);
	xTaskCreate(vLEDTASK4, (const char *) "LEDTSK4", configMINIMAL_STACK_SIZE, NULL, 0, &LED4);
	
	vTaskSuspend(LED1);
	vTaskSuspend(LED2);
	vTaskSuspend(LED3);
	vTaskSuspend(LED4);

	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
	vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
	vDisplayWriteStringAtPos(2,0,"Template");
	vDisplayWriteStringAtPos(3,0,"ResetReason: %d", reason);
	vTaskStartScheduler();
	return 0;
}
void vLEDTASK1(void *pvParameters){
	
	(void) pvParameters;
	PORTF.OUT = 0x0F;
	PORTE.OUT = 0x0F;
	for(;;) {
		PORTF.OUTTGL = 0x03;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vLEDTASK2(void *pvParameters){
	
	(void) pvParameters;
	PORTF.OUT = 0x0F;
	PORTE.OUT = 0x0F;
	for(;;) {
		PORTF.OUTTGL = 0x0C;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vLEDTASK3(void *pvParameters){
	
	(void) pvParameters;
	PORTF.OUT = 0x0F;
	PORTE.OUT = 0x0F;
	for(;;) {
		PORTE.OUTTGL = 0x03;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vLEDTASK4(void *pvParameters){
	
	(void) pvParameters;
	PORTF.OUT = 0x0F;
	PORTE.OUT = 0x0F;
	for(;;) {
		PORTE.OUTTGL = 0x0C;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vButtonTask(void *pvParameters) {
	initButtons();
	vTaskDelay(3000);
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
	vDisplayWriteStringAtPos(1,0, "LastPress: None");
	vDisplayWriteStringAtPos(2,0, "Type: Not Pressed");
	for(;;) {
		updateButtons();
		
		if(getButtonPress(BUTTON1) == SHORT_PRESSED) {
			
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button1");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume(LED1);
			vTaskSuspend(LED2);
			vTaskSuspend(LED3);
			vTaskSuspend(LED4);
		}
		if(getButtonPress(BUTTON2) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button2");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume(LED2);
			vTaskSuspend(LED1);
			vTaskSuspend(LED3);
			vTaskSuspend(LED4);
		}
		if(getButtonPress(BUTTON3) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button3");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume(LED3);
			vTaskSuspend(LED2);
			vTaskSuspend(LED1);
			vTaskSuspend(LED4);
		}
		if(getButtonPress(BUTTON4) == SHORT_PRESSED) {
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button4");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume(LED4);
			vTaskSuspend(LED2);
			vTaskSuspend(LED3);
			vTaskSuspend(LED1);
		}
		vTaskDelay((1000/BUTTON_UPDATE_FREQUENCY_HZ)/portTICK_RATE_MS);
	}
}
