/*
 * task_handler.c
 *
 *  Created on: Jul 23, 2024
 *      Author: ihsan
 */
#include "main.h"

int extract_command(command_t *cmd);
void process_command(command_t *cmd);
void menu_task(void*parameter)
{
	uint32_t cmd_addr;
	command_t *cmd;
	uint8_t menu_choice;
	const char*msg_menu="\n--------------------\n"
						"******  MENU  ******\n"
						"LED effect 	--->0\n"
						"Date and Time	--->1\n"
						"Exit		--->2\n"
						"--------------------\n";
	const char*msg_inv= "\n____Invalid Entry____\n";
	while(1)
	{
		xQueueSend(q_print,&msg_menu,portMAX_DELAY);

		xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		cmd=(command_t*)cmd_addr;
		if(cmd->len==1)
		{
			menu_choice=(cmd->payload[0]-48);
			switch(menu_choice){
			case 0:
				currentState=ledEffect;
				xTaskNotify(handle_led_task,0,eNoAction);
				break;
			case 1:
				currentState=RTCMenu;
				xTaskNotify(handle_rtc_task,0,eNoAction);
				break;
			case 2:
			break;
			default:
				xQueueSend(q_print,&msg_inv,portMAX_DELAY);
				continue;

			}
		}
		else
		{
			xQueueSend(q_print,&msg_inv,portMAX_DELAY);
			continue;
		}

		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

	}
}

uint8_t getnumber(uint8_t *p , int len)
{

	int value ;

	if(len > 1)
	   value =  ( ((p[0]-48) * 10) + (p[1] - 48) );
	else
		value = p[0] - 48;

	return value;

}

void rtc_task(void *param)
{
	const char* msg_rtc1 =  "------------------------\n"
							"|         RTC          |\n"
							"------------------------\n";

	const char* msg_rtc2 = "Configure Time            ----> 0\n"
							"Configure Date            ----> 1\n"
							"Exit                      ----> 2\n"
							"Enter your choice here : ";

	const char*msg_inv= "\n____Invalid Entry____\n";


	const char *msg_rtc_hh = "Enter hour(1-12):";
	const char *msg_rtc_mm = "Enter minutes(0-59):";
	const char *msg_rtc_ss = "Enter seconds(0-59):";

	const char *msg_rtc_dd  = "Enter date(1-31):";
	const char *msg_rtc_mo  ="Enter month(1-12):";
	const char *msg_rtc_dow  = "Enter day(1-7 sun:1):";
	const char *msg_rtc_yr  = "Enter year(0-99):";

	const char *msg_conf = "Configuration successful\n";


	uint32_t cmd_addr;
	command_t *cmd;

	static int rtc_state = 0;
	int menu_code;

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;


#define HH_CONFIG 		0
#define MM_CONFIG 		1
#define SS_CONFIG 		2

#define DATE_CONFIG 	0
#define MONTH_CONFIG 	1
#define YEAR_CONFIG 	2
#define DAY_CONFIG 		3


	while(1){
		/*Notify wait (wait till someone notifies) */
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		/*Print the menu and show current date and time information */
		xQueueSend(q_print,&msg_rtc1,portMAX_DELAY);
		show_time_date();
		xQueueSend(q_print,&msg_rtc2,portMAX_DELAY);


		while(currentState != mainMenu){

			/*Wait for command notification (Notify wait) */
			xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
			cmd = (command_t*)cmd_addr;

			switch(currentState)
			{
				case RTCMenu:{
					/*process RTC menu commands */
					if(cmd->len == 1)
					{
						menu_code = cmd->payload[0] - 48;
						switch(menu_code)
						{
						case 0:
							currentState = RTCTimeConfig;
							xQueueSend(q_print,&msg_rtc_hh,portMAX_DELAY);
							break;
						case 1:
							currentState = RTCDateConfig;
							xQueueSend(q_print,&msg_rtc_dd,portMAX_DELAY);
							break;
						case 2 :
							currentState = mainMenu;
							break;
						default:
							currentState = mainMenu;
							xQueueSend(q_print,&msg_inv,portMAX_DELAY);
						}

					}else{
						currentState = mainMenu;
						xQueueSend(q_print,&msg_inv,portMAX_DELAY);
					}
					break;}

				case RTCTimeConfig:{
					/*get hh, mm, ss infor and configure RTC */
					/*take care of invalid entries */
					switch(rtc_state)
						{
							case HH_CONFIG:{
								uint8_t hour = getnumber(cmd->payload , cmd->len);
								time.Hours = hour;
								rtc_state = MM_CONFIG;
								xQueueSend(q_print,&msg_rtc_mm,portMAX_DELAY);
								break;}
							case MM_CONFIG:{
								uint8_t min = getnumber(cmd->payload , cmd->len);
								time.Minutes = min;
								rtc_state = SS_CONFIG;
								xQueueSend(q_print,&msg_rtc_ss,portMAX_DELAY);
								break;}
							case SS_CONFIG:{
								uint8_t sec = getnumber(cmd->payload , cmd->len);
								time.Seconds = sec;
								if(!validate_rtc_information(&time,NULL))
								{
									rtc_configure_time(&time);
									xQueueSend(q_print,&msg_conf,portMAX_DELAY);
									show_time_date();
								}else
									xQueueSend(q_print,&msg_inv,portMAX_DELAY);

								currentState = mainMenu;
								rtc_state = 0;
								break;}
						}

					break;}

				case RTCDateConfig:{

					/*get date, month, day , year info and configure RTC */

					/*take care of invalid entries */
					switch(rtc_state)
						{
							case DATE_CONFIG:{
								uint8_t d = getnumber(cmd->payload , cmd->len);
								date.Date = d;
								rtc_state = MONTH_CONFIG;
								xQueueSend(q_print,&msg_rtc_mo,portMAX_DELAY);
								break;}
							case MONTH_CONFIG:{
								uint8_t month = getnumber(cmd->payload , cmd->len);
								date.Month = month;
								rtc_state = DAY_CONFIG;
								xQueueSend(q_print,&msg_rtc_dow,portMAX_DELAY);
								break;}
							case DAY_CONFIG:{
								uint8_t day = getnumber(cmd->payload , cmd->len);
								date.WeekDay = day;
								rtc_state = YEAR_CONFIG;
								xQueueSend(q_print,&msg_rtc_yr,portMAX_DELAY);
								break;}
							case YEAR_CONFIG:{
								uint8_t year = getnumber(cmd->payload , cmd->len);
								date.Year = year;

								if(!validate_rtc_information(NULL,&date))
								{
									rtc_configure_date(&date);
									xQueueSend(q_print,&msg_conf,portMAX_DELAY);
									show_time_date();
								}else
									xQueueSend(q_print,&msg_inv,portMAX_DELAY);

								currentState = mainMenu;
								rtc_state = 0;
								break;}
						}


					break;}

			}// switch end

		} //while end


		/*Notify menu task */
		xTaskNotify(handle_menu_task,0,eNoAction);

		}//while super loop end
}


void print_task(void*parameter)
{
	uint32_t*msg;
	while(1)
	{
		xQueueReceive(q_print, (void*)&msg,portMAX_DELAY);
		HAL_UART_Transmit(&huart2,(uint8_t*)msg , strlen((char*)msg), HAL_MAX_DELAY);
	}
}

void cmd_task(void*parameter)
{
	BaseType_t status;
	command_t cmd;
	while(1){
	status=xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		if(status==pdTRUE)
		{
			process_command(&cmd);
		}

	}
}

void process_command(command_t* cmd)
{
	extract_command(cmd);
	switch(currentState)
	{
	case mainMenu:
		xTaskNotify(handle_menu_task,(uint32_t)cmd,eSetValueWithOverwrite);
		break;
	case ledEffect:
		xTaskNotify(handle_led_task,(uint32_t)cmd,eSetValueWithOverwrite);
		break;
	case RTCMenu:
	case RTCDateConfig:
	case RTCTimeConfig:
		xTaskNotify(handle_rtc_task,(uint32_t)cmd,eSetValueWithOverwrite);
	break;
	}


}
int extract_command(command_t *cmd)
{
	BaseType_t status;
	uint8_t item;
	uint8_t i=0;

	status=uxQueueMessagesWaiting(q_data);
	if(!status)return -1;
	do{
		status=xQueueReceive(q_data,&item,0);
		if(status==pdTRUE)cmd->payload[i++]=item;
	}
	while(item!='\n');
	cmd->len=i-1;
	cmd->payload[i-1]='\0';
return 0;
}


void led_task(void *parameter)
{
	uint32_t cmd_addr;
	command_t *cmd;
	const char* msg_led = "-----------------------\n"
						  "|      LED Effect     |\n"
						  "-----------------------\n"
						  "(none,e1,e2,e3,e4)\n"
						  "Enter your choice here : ";
	const char*msg_inv= "\n____Invalid Entry____\n";

	while(1){
		/*TODO: Wait for notification (Notify wait) */
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		/*TODO: Print LED menu */
		xQueueSend(q_print,&msg_led,portMAX_DELAY);
		/*TODO: wait for LED command (Notify wait) */
		xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		cmd=(command_t*)cmd_addr;
		if(cmd->len <= 4)
		{
			if(! strcmp((char*)cmd->payload,"none"))
				led_effect_stop();
			else if (! strcmp((char*)cmd->payload,"e1"))
				led_effect(1);
			else if (! strcmp((char*)cmd->payload,"e2"))
				led_effect(2);
			else if (! strcmp((char*)cmd->payload,"e3"))
				led_effect(3);
			else if (! strcmp((char*)cmd->payload,"e4"))
				led_effect(4);
			else
				xQueueSend(q_print,&msg_inv,portMAX_DELAY);
		}else
			xQueueSend(q_print,&msg_inv,portMAX_DELAY);

		currentState = mainMenu;

		xTaskNotify(handle_menu_task,0,eNoAction);

	}
}
