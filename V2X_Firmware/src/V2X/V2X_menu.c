/*
 * V2X_menu.c
 *
 * Created: 2/23/2016 9:00:14 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"
#include "ctype.h"

char CMD_buffer [30] = "\0";
uint8_t sample[6] = {0,0,0,0,0,0};
Bool verbose = true;

void menu_init(void) {
	verbose = nvm_eeprom_read_byte(EE_verbose);
}

void menu_add_to_command(char value) {
	if (value == 8 || value == 0x7f) {//if backspace or delete
		int leng = strlen(CMD_buffer); //get buffer sting length
		if (leng > 0) {	//if length not 0
			CMD_buffer[leng-1] = '\0';	//null out the last char
		}
	} else if (value > 0x20 && value < 0x7E ) {		//if in ascii set
		strcat(CMD_buffer, &value);					//store to buffer
	}
}

void menu_main(void) {
	for(int i = 0; i < 4; i++){
		CMD_buffer[i] = tolower(CMD_buffer[i]);
	}
	if (CMD_buffer[0] == 'v' && CMD_buffer[1] == 'x') {
		switch(CMD_buffer[2]) {
			case 'i': //information
				usb_tx_string_P(PSTR("Vehicle to Everything (V2X) RVI Node 2016\rOpen source hardware and software\rHW Rev1.2 \rSW Rev0.1\r"));
				break;
			case 'j': //Jaguar
				usb_tx_string_P(PSTR("\r\r   ,ggp@@@@mgg,,\r,$Q$(`S@@$;g$$$$$@$@@gg,\r;gP'$@gg)$$@@$@@@$(L$$||$$@g,\r  `g$P`  ``*%@@@P`)Eg|||lLLL||$Bgg,\r    `       ,gg$$@gg,`$..``$Z$$$$$EB$$@g,\r         @P`pgg$$$||`)gggg;,,     |$$$|$$$@g,\r         9w&    '*^^` ``*P#9BB00000$$$@|`$$$g|Sg,\r                                    *$@@L ```T$W~)%g,\r                                      *%@gg,,,,,    5/Sw,     ,\r                                          ```` ` `9g `9g,``*^|'\r                                                    `#g,`)h\r\r   Developed at Jaguar Land Rover OSCT. Portland OR 2016\r"));
				break;
			case 'q':
				menu_status();
				break;
			case 'r':  //reset
				reset_trigger_SYSTEM();
				break;
			case 'a':  //accel
				menu_accel();
				break;
			case 'c':  //CAN interface
				menu_can();
				break;
			case 'm':  //modem
				menu_modem();
				break;
			case 'p':  //power
				menu_power();
				break;
			case 't':  //timer functions
				menu_timer();
				break;
			case 'v': //toggle verbose setting
				verbose = !verbose;
				nvm_eeprom_write_byte(EE_verbose,verbose);
				usb_tx_string_PV(PSTR("Verbose is ON"));
				break;
			case '?':  //timer functions
			default:
				usb_tx_string_P(PSTR("*** Main Menu ***\rI: Device information\rA: Accelerometer menu(ACL)\rC: ELM327 menu(CAN)\rM: SIM Modem menu(GSM)\rP: Power menu\rT: Timer menu\rQ: Query system status\rV: Toggle verbose messages\rR: Reboot\r"));
				break;
			}
	}else{
			usb_tx_string_PV(PSTR("All commands start VX\r"));
	}
	clear_buffer(CMD_buffer);	//clear the buffer for next command	
	menu_send_n_st();
}

void menu_accel (void) {
	int speed, data16;
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		ACL_set_sample_off();
		usb_tx_string_PV(PSTR("Accelerometer is off"));
		break;
	case 'e':  //enable
		ACL_set_sample_on();
		usb_tx_string_PV(PSTR("Accelerometer started"));
		break;
	case 'i':
		usb_tx_string_P(PSTR("V2X uses the ADXL345 accelerometer from Analog Devices\r"));
		break;
	case 'q':
		menu_accel_status();
		break;
	case 'r':  //restart
		ACL_set_sample_off();
		ACL_take_sample(sample); //pushes "off" command
		ACL_init(); //
		ACL_set_sample_on();
		ACL_take_sample(sample); //pushes init
		usb_tx_string_PV(PSTR("Accelerometer has been restarted"));
		break;
	case 's':  //sample rate
 		speed = atoi(CMD_buffer+4);
		if (speed > 3199) {
			ACL_set_rate(ACL_RATE_3200);	
			usb_tx_string_PV(PSTR("Set to 3200Hz"));
		}else if (speed > 1599) {
			ACL_set_rate(ACL_RATE_1600);
			usb_tx_string_PV(PSTR("Set to 1600Hz"));
		}else if (speed > 799) {
			ACL_set_rate(ACL_RATE_800);
			usb_tx_string_PV(PSTR("Set to 800Hz"));
		}else if (speed > 399) {
			ACL_set_rate(ACL_RATE_400);
			usb_tx_string_PV(PSTR("Set to 400Hz"));
		}else if (speed > 199) {
			ACL_set_rate(ACL_RATE_200);
			usb_tx_string_PV(PSTR("Set to 200Hz"));
		}else if (speed > 99) {
			ACL_set_rate(ACL_RATE_100);
			usb_tx_string_PV(PSTR("Set to 100Hz"));
		}else if (speed > 49) {
			ACL_set_rate(ACL_RATE_50);
			usb_tx_string_PV(PSTR("Set to 50Hz"));
		}else if (speed > 24) {
			ACL_set_rate(ACL_RATE_25);
			usb_tx_string_PV(PSTR("Set to 25Hz"));
		}else if (speed > 11) {
			ACL_set_rate(ACL_RATE_12);
			usb_tx_string_PV(PSTR("Set to 12Hz"));
		}else if (speed > 5) {
			ACL_set_rate(ACL_RATE_6);
			usb_tx_string_PV(PSTR("Set to 6Hz"));
		}else if (speed > 2) {
			ACL_set_rate(ACL_RATE_3);
			usb_tx_string_PV(PSTR("Set to 3Hz"));
		}else if (speed > 0) {
			ACL_set_rate(ACL_RATE_1);
			usb_tx_string_PV(PSTR("Set to 1Hz"));
 		}else {	
			usb_tx_string_P(PSTR("ERROR: Out of range, try again."));
		}
		break;
	case 'w':
		speed = atoi(CMD_buffer+4);
		if (speed > 16) {speed = 0;}
		if (speed > 8) {
			ACL_set_range(ACL_16G_RANGE);
			usb_tx_string_PV(PSTR("+/-16G Range"));
		}else if (speed > 4) {
			ACL_set_range(ACL_8G_RANGE);
			usb_tx_string_PV(PSTR("+/-8G Range"));
		}else if (speed > 2) {
			ACL_set_range(ACL_4G_RANGE);
			usb_tx_string_PV(PSTR("+/-4G Range"));
		}else if (speed > 0) {
			ACL_set_range(ACL_2G_RANGE);
			usb_tx_string_PV(PSTR("+/-2G Range"));
		}else {menu_send_out_of_range();}
		break;
		
	case 'x':
	case 'y':
	case 'z':
		data16 = atoi(CMD_buffer+4);  //convert input chars to int
		if (data16 == 0){ //conversion resulted in 0
			if (CMD_buffer[4] != '0'){ //check if it is really a '0' of an invalid conversion
				menu_send_out_of_range(); //invalid result
				break; //exit offset menu
			}
		}
		if (data16 < -127 || data16 > 127) {  //must be 16 to detect out of range
			menu_send_out_of_range();
		} else {
			uint8_t data8 = (uint8_t)data16;  //reduce to 8 bits of data
			switch (CMD_buffer[3]) {
				case 'x':
					if (verbose) {usb_tx_string_P(PSTR("X"));}
					ACL_set_offset(ACL_X_OFFSET, data8);
					break;
				case 'y':
					if (verbose) {usb_tx_string_P(PSTR("Y"));}
					ACL_set_offset(ACL_Y_OFFSET, data8);
					break;
				case 'z':
					if (verbose) {usb_tx_string_P(PSTR("Z"));}
					ACL_set_offset(ACL_Z_OFFSET, data8);
					break;
				default:
					menu_send_q();
					break;
			}
			usb_tx_string_PV(PSTR(" Offset: "));
			if (verbose) {
				menu_print_int(data16);
				menu_send_n();
			}
		}
		break;
	case 'g':
		if (ACL_sampling()){
			if (usb_cdc_is_active(USB_ACL)) {
				ACL_get_last_sample(sample);
			} else {
				ACL_take_sample(sample);
			}
			ACL_data_to_string(sample, CMD_buffer); 
			for (int i = 0; i < strlen(CMD_buffer); i++)
				{usb_cdc_send_byte(USB_CMD, CMD_buffer[i]);}
		} else {
			usb_tx_string_P(PSTR("ERROR: Not currently sampling, use VXAE\r"));}
		
		break;
	case 'f':
		if (CMD_buffer[4] == '0') {
			ACL_set_full_resolution(0);
			usb_tx_string_PV(PSTR("10bit resolution, scale changes with 'G' range\r"));
		}
		else if (CMD_buffer[4] == '1') {
			ACL_set_full_resolution(1);
			usb_tx_string_PV(PSTR("Full resolution, 4mg/bit\r"));
		}
		else {menu_send_q();}
		break;
	case '?':  
	default:
		usb_tx_string_P(PSTR("*** Accelerometer Menu ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\rG: Get last Sample\rSn: Set sample rate in HZ (1, 3, 6, 12, 25, 50, 100, 200, 400, 800)\rWn: Set \"G\" Range (2, 4, 8, 16)\rFn: Full range (1: full 0: 10bit)\rXn: Set X offset (+/-127)\rYn: Set Y offset\rZn: Set Z offset\r"));
		break;
	}
}

void menu_modem (void) {
	int i; 
	switch (CMD_buffer[3]) {
	case 'd':  
		PWR_gsm_stop(); //disable
		usb_tx_string_PV(PSTR("GSM is off\r"));
		break;
	case 'e':  //enable modem
		usb_tx_string_PV(PSTR("GSM Start pending"));
		GSM_modem_init();
		break;
	case 'r':  //reset
		reset_trigger_GSM();
		usb_tx_string_PV(PSTR("GSM restarting"));
		break;
	case 'q':
		menu_modem_status();
		break;
	case 'i':
		usb_tx_string_P(PSTR("V2X uses the SIM5320A 3G GSM modem + GPS receiver by SIMCOM\r"));
		break;
	case 'x':
		strcat_P(CMD_buffer, PSTR("\r\n"));  //put these char at the end of the string
		GSM_add_string_to_buffer(BUFFER_OUT, &CMD_buffer[4]); //send it on to the modem
		GSM_mark_for_processing(BUFFER_OUT); //initiate send
 		break;
	case '?':
	default:
		usb_tx_string_P(PSTR("*** Modem Menu ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\rX: AT Command Pass through\r"));
		break;
	}
}

void menu_can (void) {
	int i;
	char buffer[EE_CAN_ARRAY_SIZE+1];
	switch (CMD_buffer[3]) {
	case 'd':  //disable;
		usb_tx_string_PV(PSTR("CAN is off\r"));
		PWR_can_stop();
		PWR_is_5_needed();
		break;
	case 'e':  //enable
		usb_tx_string_PV(PSTR("CAN Starting\r"));
		CAN_elm_init();
		break;
	case 'r':  //reset
		usb_tx_string_PV(PSTR("CAN restarting\r"));
		reset_trigger_CAN();
		break;
	case 'q': //query
		menu_can_status();
		break;
	case 'i': //info
		usb_tx_string_P(PSTR("V2X uses the STN1110 CANbus interface from Scantool\rThe STN1110 is compliant with the ELM327 V1.3\r"));
		break;
	case 'x': //pass command
		strcat_P(CMD_buffer, PSTR("\r"));
		
		CAN_add_string_to_buffer(BUFFER_OUT, CMD_buffer+4);
		CAN_mark_for_processing(BUFFER_OUT);
		CAN_start_snoop();
		break;
	case 's':  //save string to eeprom
		//strcat_P(CMD_buffer, PSTR("\r"));
		eeprom_store_CAN_string(CMD_buffer+4);
		break;
	case 'w': //what is stored in EE?
 		eeprom_read_CAN_string(buffer);
 		usb_cdc_send_string(USB_CMD, buffer);
		break;
	case 'a': //load string and parse from eeprom
		CAN_EE_start();
		break;
	case '?':
	default:
		usb_tx_string_P(PSTR("*** CANbus Menu ***\rE: Enable\rD: Disable\rR: Restart\rS: Store config string to EE\rA: Execute config string in EE\rW: Display config string in EE\rI: Subsystem Information\rQ: Query status\r"));
		break;
	}
}

void menu_power (void) {
	switch (CMD_buffer[3]) {
	case 'd':  //disable power
		switch (CMD_buffer[4]) {
		case '3':  //3v
			usb_tx_string_PV(PSTR("Disabling 3V supply"));
			PWR_turn_off((1<<ENABLE_3V3));
			PWR_push();
			break;
		case '4':  //4v
			usb_tx_string_PV(PSTR("Disabling 4V supply"));
			PWR_turn_off((1<<ENABLE_4V1)|(1<<ENABLE_SIM_RESET));
			PWR_push();
			break;
		case '5':  //5v
			usb_tx_string_PV(PSTR("Disabling 5V supply"));
			PWR_turn_off((1<<ENABLE_5V0)|(1<<ENABLE_5V0B)|(1<<ENABLE_CAN_RESET));
			PWR_push();
			break;
		case 'h':  //host
			usb_tx_string_PV(PSTR("Disabling Host power supply"));
			PWR_turn_off((1<<ENABLE_5V0B));
			PWR_is_5_needed();
			break;
		default:
			menu_send_q();
			break;
		}
		break; //disable power
	case 'e':  //enable power
		switch (CMD_buffer[4]) {
		case '3':  //3v
			usb_tx_string_PV(PSTR("Enabling 3V supply"));
			PWR_turn_on((1<<ENABLE_3V3));
			PWR_push();
			break;
		case '4':  //4v
			usb_tx_string_PV(PSTR("Enabling 4V supply"));
			PWR_turn_on((1<<ENABLE_4V1));
			PWR_push();
			break;
		case '5':  //5v
			usb_tx_string_PV(PSTR("Enabling 5V supply"));
			PWR_turn_on((1<<ENABLE_5V0));
			PWR_push();
			break;
		case 'h':  //host
			usb_tx_string_PV(PSTR("Enabling Host power supply"));
			PWR_turn_on((1<<ENABLE_5V0B));
			PWR_push();
			break;
		default:
			menu_send_q();
			break;
		}
		break;  //enable power
	case 'r':  
		PWR_turn_off(~POWER_CONTROL_DEFAULT_VALUE);
		PWR_turn_on(POWER_CONTROL_DEFAULT_VALUE);
		PWR_push();
		menu_send_ok();
		break;
	case 'q':
		menu_power_status();
		break;
	case '?':
	default:
		usb_tx_string_P(PSTR("*** Power Menu ***\rEn: Enable power supply (3, 4, 5, H)\rDn: Disable power supply (3, 4, 5, H)\rR: Reset to defaults\rQ: Query status\r"));
		break;
	}
}

void menu_timer(void) {
	long data32;
	switch (CMD_buffer[3]) {
	case 's':  //set V2X time
		time_set(menu_sample_number(CMD_buffer+4));
		usb_tx_string_PV(PSTR("Time has been set\r"));
		break;
	case 'g':  //get V2X time
		usb_tx_string_P(PSTR("UET: "));
		menu_print_int(time_get());
		usb_tx_string_P(PSTR("\rTime: "));
		time_print_human_readable();
		menu_send_n();
		break;
	case 'a':  //absolute alarm
		time_alarm_set(menu_sample_number(CMD_buffer+4));
		usb_tx_string_PV(PSTR("Alarm has been set"));
		break;
	case 'r':  //relative alarm
		time_alarm_set_relative(menu_sample_number(CMD_buffer+4));
		usb_tx_string_PV(PSTR("Alarm has been set"));
		break;
	case 'z':  //set time zone
		time_zone_set(menu_sample_number(CMD_buffer+4));
		usb_tx_string_P(PSTR("TZN="));
		menu_print_int(time_zone_get());
		break;
	case 'd':  //set time zone
		usb_tx_string_P(PSTR("DST="));
		if (menu_sample_number(CMD_buffer+4)) {
			time_dst_set(1);
			menu_send_1();
		} else {
			time_dst_set(0);
			menu_send_0();
		}
		break;
	case 'i':  //timer system information
		usb_tx_string_P(PSTR("The timer module uses Unix Epoch timestamps (UET) \rH24: clock has been set/sync within 24hrs\rALM: alarm is set for the future\r"));
		break;
	case 'q':  //timer inquery
		menu_timer_status();
		break;
	case 'u':
		//usb_tx_string_P(PSTR("GPS update\r"));
		GSM_time_job();
		break;
	case '?':  //Menu options
	default:
		usb_tx_string_P(PSTR("*** Timer Menu ***\rSn: Set V2X time (UET)\rDn: Daylight Savings Time\rG: Get V2X time\rAn: Set absolute alarm (UET) \rRn: Set relative alarm (Seconds)\rI: timer information\rQ: Timer inquery\rU: Update using gps\rZn: Set timezone\r"));
		break;
	}
}

void menu_send_ok(void) {usb_tx_string_P(PSTR("OK\r"));
}

void menu_send_q(void) {usb_tx_string_P(PSTR("?\r"));
}

void menu_send_1(void) {usb_tx_string_P(PSTR("1\r"));
}

void menu_send_0(void) {usb_tx_string_P(PSTR("0\r"));
}

void menu_send_n(void) {usb_tx_string_P(PSTR("\r"));
}

void menu_send_n_st(void) {usb_tx_string_P(PSTR("\r>"));
}			

void menu_send_out_of_range(void) {usb_tx_string_P(PSTR("ERROR: out of range\r"));
}

void menu_send_GSM(void) {usb_tx_string_P(PSTR("GSM>:"));
}

void menu_send_CTL(void) {usb_tx_string_P(PSTR("CTL>:"));
}
	
void menu_send_CAN(void) {usb_tx_string_P(PSTR("CAN>:"));
}
	
void menu_send_BTN(void) {usb_tx_string_P(PSTR("BTN>:"));
}

Bool menu_verbose(void) {
	return verbose;
}

void menu_print_int(long value) {	
	char c_buf[13];
	ltoa(value, c_buf, 10);
	int i = 0;  //clear the pointer
	while (c_buf[i] != 0)
		{usb_cdc_send_byte(USB_CMD, c_buf[i++]);}
}

long menu_sample_number(char * input) {
	return atol(input);  //convert input chars to int
}

void usb_tx_string_P(const char *data) {
	while (pgm_read_byte(data))
		usb_cdc_send_byte(USB_CMD, pgm_read_byte(data++));
}

void usb_tx_string_PV(const char *data) {
	if (verbose) {
		usb_tx_string_P(data);
	} else {
		usb_tx_string_P(PSTR("OK"));
	}

}

void menu_lockup (void) {
	char msg[] = "AVR>>RESET:\r";
	usb_cdc_send_string(USB_CMD, msg);
	usb_cdc_send_string(USB_ACL, msg);
	usb_cdc_send_string(USB_CAN, msg);
	delay_s(1);
	PWR_hub_stop();
	delay_s(1);
	RST_CTRL = true; //force SW reset
}

void menu_status (void) {
	menu_power_status();
	menu_accel_status();
	menu_can_status();
	menu_modem_status();
	menu_timer_status();
}

void menu_accel_status(void) {
	usb_tx_string_P(PSTR("ACL="));
	if (ACL_sampling())
	{menu_send_1();}
	else	{menu_send_0();}
}

void menu_modem_status(void) {
	usb_tx_string_P(PSTR("SIMPWR="));
	if (sim_power_status())
	{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("SIMNET="));
	if (sim_net_status())
	{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("SIMCARD="));
	if (simcard_status())
	{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("IMEI="));
	usb_cdc_send_string(USB_CMD, GSM_get_imei());
	menu_send_n();
}

void menu_simcard_status(void){
	if (simcard_status())
		{menu_send_1();}
	else	{menu_send_0();}
}

void menu_can_status(void) {
	usb_tx_string_P(PSTR("CAN="));
	if (PWR_query((1<<ENABLE_CAN_RESET)))
			{menu_send_1();}
	else	{menu_send_0();}
}

void menu_power_status(void) {
	usb_tx_string_P(PSTR("3V3="));
	if (PWR_query((1<<ENABLE_3V3)))
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("4V1="));
	if (PWR_query((1<<ENABLE_4V1)))
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("5V0="));
	if (PWR_query((1<<ENABLE_5V0)))
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("HOST="));
	if (PWR_query((1<<ENABLE_5V0B)))
			{menu_send_1();}
	else	{menu_send_0();}
}

void menu_timer_status (void) {
	usb_tx_string_P(PSTR("TZN="));
	menu_print_int(time_zone_get());
	menu_send_n();
	usb_tx_string_P(PSTR("DST="));
	if (time_dst_get())
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("UTC="));
	menu_print_int(time_get());		
	menu_send_n();
	usb_tx_string_P(PSTR("H24="));
	if (time_is_current())
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("ALM="));
	menu_print_int(time_alarm_get());		
	menu_send_n();
	usb_tx_string_P(PSTR("ALS="));
	if (time_alarm_active())
			{menu_send_1();}
	else	{menu_send_0();}

}