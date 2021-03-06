

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "xil_cache.h"
#include <time.h>



#include "AXI_mem_access.h"

#include <AXI_CAN_core.h>
#include <AXI_UART_master_core.h>
#include <AXI_DAC_core_driver.h>
#include <AXI_ADC_core_driver.h>
#include <AXI_I2C.h>
#include <AXI_DMM_core_driver.h>

#include "waves.h"


#define GPIO_BASE_ADDR 0x80007000
#define DAC_BASE_ADDR 0x80009000
#define ADC_BASE_ADDR 0x80008000
#define FPGA_VERSION_BASE_ADDR 0x8000A000
#define DMM_BASE_ADDR 0x8000B000

#define ADC_SYS_RAM_BASE_ADDR 0x40000000

#define CAN_DATA_BUFF_SIZE (8)

#define DAC_AMPLIFIER_GAIN 4990




#define CHANNEL_NUM 3
#define CHANNEL_POS 0
#define CHANNEL_NEG 1
#define ADC_ATTEN (x10)
#define ADC_TRIG_VAL 0
#define ADC_TRIG_RISING_EDGE true
#define BIN_SAVE false

//#define TUNE
//#define DMM_DC
#define DMM_AC


#ifndef DMM_DC
	#define CAL_DMM false
#else
	#define CAL_DMM false
#endif

#ifdef TUNE
	#define ADC_SAMPLE_COUNT 2000
	#define SAMPLE_RATE 1
	#define CAPTURE_COUNT 1
	#define RMS_ON false
	#define WAVE_PRINT_ON true
	#define FORCE_TRIG false
#else

	#ifdef DMM_DC
		#define SAMPLE_RATE 1

		#if(CAL_DMM)
			#define CAPTURE_COUNT 100
			#if CHANNEL_NUM > 1
				#define ADC_SAMPLE_COUNT 50000
			#else
				#define ADC_SAMPLE_COUNT 125000
			#endif
		#else
			#define CAPTURE_COUNT 1
			#if CHANNEL_NUM > 1
				#define ADC_SAMPLE_COUNT 500000
			#else
				#define ADC_SAMPLE_COUNT 5000000
			#endif
		#endif

		#define RMS_ON false
		#define WAVE_PRINT_ON false
		#define FORCE_TRIG true
	#else

		#ifdef DMM_AC
			#define ADC_SAMPLE_COUNT 100000
			#define SAMPLE_RATE 1
			#define CAPTURE_COUNT 1
			#define RMS_ON true
			#define WAVE_PRINT_ON false
			#define FORCE_TRIG false
		#else
			#define ADC_SAMPLE_COUNT 500000
			#define SAMPLE_RATE 100
			#define CAPTURE_COUNT 1
			#define RMS_ON false
			#define WAVE_PRINT_ON true
			#define FORCE_TRIG true
		#endif

	#endif

#endif














unsigned int pars_int_arg_val(char* val)
{
  if((val[0]=='0') && (val[1]=='x'))
    return (unsigned int)strtol(val+2, NULL, 16);
  else
    return (unsigned int)strtol(val, NULL, 10);
}

unsigned int DAC_voltage_convert(int mVolts)
{
	if(mVolts == 0x80000000)//Reserved value for Hi-Z
		return 0xFFFF;
	if(mVolts > ((2500*DAC_AMPLIFIER_GAIN) / 1000))
		return 0xFFFE;//MAX VALUE (0xFFFF is reserved for Hi-Z)
	else if(mVolts < (((-2500)*DAC_AMPLIFIER_GAIN) / 1000))
		return 0;
	else
		return (((((unsigned int)(mVolts + ((2500*DAC_AMPLIFIER_GAIN) / 1000))) * 1000) / DAC_AMPLIFIER_GAIN) * 0xFFFF) / 5000;
}


uint64_t sqrt64(uint64_t a) {
        uint64_t rem = 0, root = 0;

        for (int i = 64 / 2; i > 0; i--) {
                root <<= 1;
                rem = (rem << 2) | (a >> (64 - 2));
                a <<= 2;
                if (root < rem) {
                        rem -= root | 1;
                        root += 2;
                }
        }
        return root >> 1;
}



ATC_ADC_Calibration_Dat* ATC_ADC_cal_data;

int main()
{
    init_platform();
    Xil_DCacheDisable();

    /*
    unsigned* ram_buf = (unsigned*)0x40000000;
    for(unsigned i = 0; i < 0x100000; i++)
	{
		ram_buf[i] = 0;//((i*2) << 16) | ((i*2)+1);
	}
   */



    ATC_ADC_cal_data = ATC_ADC_Calibration_Dat_new();


    //---ADC CH 0---
    	ATC_ADC_cal_data->x10.offset[0] = 51;
        ATC_ADC_cal_data->x10.gain_p[0] = 32768;
    	ATC_ADC_cal_data->x10.gain_err_p[0] = 0;
    	ATC_ADC_cal_data->x10.gain_n[0] = 32768;
		ATC_ADC_cal_data->x10.gain_err_n[0] = 0;

        ATC_ADC_cal_data->x100.offset[0] = 0;
        ATC_ADC_cal_data->x100.gain_p[0] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_p[0] = 0;
        ATC_ADC_cal_data->x100.gain_n[0] = 0x8000;
    	ATC_ADC_cal_data->x100.gain_err_n[0] = 0;

        ATC_ADC_cal_data->x500.offset[0] = 0;
    	ATC_ADC_cal_data->x500.gain_p[0] = 32768;
    	ATC_ADC_cal_data->x500.gain_err_p[0] = 0;
    	ATC_ADC_cal_data->x500.gain_n[0] = 32768;
		ATC_ADC_cal_data->x500.gain_err_n[0] = 0;
    //**************

	//---ADC CH 1---
		ATC_ADC_cal_data->x10.offset[1] = 7;
		ATC_ADC_cal_data->x10.gain_p[1] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_p[1] = 0;
		ATC_ADC_cal_data->x10.gain_n[1] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_n[1] = 0;

		ATC_ADC_cal_data->x100.offset[1] = 0;
		ATC_ADC_cal_data->x100.gain_p[1] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_p[1] = 0;
		ATC_ADC_cal_data->x100.gain_n[1] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_n[1] = 0;

		ATC_ADC_cal_data->x500.offset[1] = 6;
		ATC_ADC_cal_data->x500.gain_p[1] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_p[1] = 0;
		ATC_ADC_cal_data->x500.gain_n[1] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_n[1] = 0;
	//**************

	//---ADC CH 2---
		ATC_ADC_cal_data->x10.offset[2] = 51;
		ATC_ADC_cal_data->x10.gain_p[2] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_p[2] = 0;
		ATC_ADC_cal_data->x10.gain_n[2] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_n[2] = 0;

		ATC_ADC_cal_data->x100.offset[2] = 0;
		ATC_ADC_cal_data->x100.gain_p[2] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_p[2] = 0;
		ATC_ADC_cal_data->x100.gain_n[2] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_n[2] = 0;

		ATC_ADC_cal_data->x500.offset[2] = 51;
		ATC_ADC_cal_data->x500.gain_p[2] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_p[2] = 0;
		ATC_ADC_cal_data->x500.gain_n[2] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_n[2] = 0;
	//**************

	//---ADC CH 3---
		ATC_ADC_cal_data->x10.offset[3] = 133;
		ATC_ADC_cal_data->x10.gain_p[3] = 0x826B;
		ATC_ADC_cal_data->x10.gain_err_p[3] = 0;
		ATC_ADC_cal_data->x10.gain_n[3] = 0x8265;
		ATC_ADC_cal_data->x10.gain_err_n[3] = 0;

		ATC_ADC_cal_data->x100.offset[3] = 0;
		ATC_ADC_cal_data->x100.gain_p[3] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_p[3] = 0;
		ATC_ADC_cal_data->x100.gain_n[3] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_n[3] = 0;

		ATC_ADC_cal_data->x500.offset[3] = 41;
		ATC_ADC_cal_data->x500.gain_p[3] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_p[3] = 0;
		ATC_ADC_cal_data->x500.gain_n[3] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_n[3] = 0;
	//**************

	//---ADC CH 4---
		ATC_ADC_cal_data->x10.offset[4] = 39;
		ATC_ADC_cal_data->x10.gain_p[4] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_p[4] = 0;
		ATC_ADC_cal_data->x10.gain_n[4] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_n[4] = 0;

		ATC_ADC_cal_data->x100.offset[4] = 0;
		ATC_ADC_cal_data->x100.gain_p[4] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_p[4] = 0;
		ATC_ADC_cal_data->x100.gain_n[4] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_n[4] = 0;

		ATC_ADC_cal_data->x500.offset[4] = 39;
		ATC_ADC_cal_data->x500.gain_p[4] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_p[4] = 0;
		ATC_ADC_cal_data->x500.gain_n[4] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_n[4] = 0;
	//**************

	//---ADC CH 5---
		ATC_ADC_cal_data->x10.offset[5] = 51;
		ATC_ADC_cal_data->x10.gain_p[5] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_p[5] = 0;
		ATC_ADC_cal_data->x10.gain_n[5] = 0x8000;
		ATC_ADC_cal_data->x10.gain_err_n[5] = 0;

		ATC_ADC_cal_data->x100.offset[5] = 0;
		ATC_ADC_cal_data->x100.gain_p[5] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_p[5] = 0;
		ATC_ADC_cal_data->x100.gain_n[5] = 0x8000;
		ATC_ADC_cal_data->x100.gain_err_n[5] = 0;

		ATC_ADC_cal_data->x500.offset[5] = 50;
		ATC_ADC_cal_data->x500.gain_p[5] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_p[5] = 0;
		ATC_ADC_cal_data->x500.gain_n[5] = 0x8000;
		ATC_ADC_cal_data->x500.gain_err_n[5] = 0;
	//**************


    char cmd_str[100];
    char addr_str[16];
    unsigned int addr = 0;
    char data_str[16];
    unsigned int data = 0;
    char get_char = '~';
    unsigned int char_count;
    unsigned int char_count_2;

    unsigned uart_pause_counter;

	// Setup Tx frame struct
	uint8_t tx_data_buff[CAN_DATA_BUFF_SIZE] = {0};
	CAN_tx_frame_t tx_frame_1 = {0};
	tx_frame_1.data = tx_data_buff;

	axi_adc_reset(ADC_BASE_ADDR);

	uint32_t fpga_ver[3];

    while(1)
    {
    	fpga_ver[0] = axi_read(FPGA_VERSION_BASE_ADDR);
    	fpga_ver[1] = axi_read(FPGA_VERSION_BASE_ADDR+4);
    	fpga_ver[2] = axi_read(FPGA_VERSION_BASE_ADDR+8);

    	printf("\n\nBARE-METAL TEST APP");
    	printf("\n(FPGA Version: %u.%u.%u}", fpga_ver[0], fpga_ver[1], fpga_ver[2]);
    	printf("\n\nEnter r/w/c: ");

    	char_count = 0;
    	char_count_2 = 0;
		get_char = getc(stdin);
		while(get_char != '\r')
		{
			cmd_str[char_count] = get_char;
			++char_count;
			get_char = getc(stdin);
		}
		if((cmd_str[0] == 'r') || (cmd_str[0] == 'R'))
		{
			printf("\nEnter <Address Offset>: ");

			char_count = 0;
			char_count_2 = 0;
			get_char = getc(stdin);
			while(get_char != '\r')
			{
				cmd_str[char_count] = get_char;
				++char_count;
				get_char = getc(stdin);
			}
			addr_str[char_count] = '\0';
			memcpy(addr_str, cmd_str, char_count);
			addr = pars_int_arg_val(addr_str);
			printf("\nRead Data: 0x%08x", axi_read(0x80000000 + addr));
		}
		else if((cmd_str[0] == 'w') || (cmd_str[0] == 'W'))
		{

			printf("\nEnter <Address Offset> <Data>: ");


			char_count = 0;
			char_count_2 = 0;
			get_char = getc(stdin);
			while(get_char != '\r')
			{
				cmd_str[char_count] = get_char;
				++char_count;
				get_char = getc(stdin);
			}

			char_count_2 = char_count;

			char_count = strchr(cmd_str, ' ') - cmd_str;
			memcpy(addr_str, cmd_str, char_count);
			addr_str[char_count] = '\0';

			addr = pars_int_arg_val(addr_str);
			memcpy(data_str, &cmd_str[char_count + 1], char_count_2 - char_count);
			data_str[char_count_2 - char_count - 1] = '\0';
			data = pars_int_arg_val(data_str);
			axi_write(0x80000000 + addr, data);
		}
		else if((cmd_str[0] == 'c') || (cmd_str[0] == 'C'))
		{
			printf("\nEnter Test Mode (1 through 4): ");

			char_count = 0;
			char_count_2 = 0;
			get_char = getc(stdin);
			while(get_char != '\r')
			{
				cmd_str[char_count] = get_char;
				++char_count;
				get_char = getc(stdin);
			}

			/*if(cmd_str[0] == '1')
			{


				//---Init AXI_CAN module---
				CAN_init(CAN_BASE_ADDR, 100);
				//*************************

				tx_frame_1.id = 0x82;
				tx_frame_1.extended_id = false;
				tx_frame_1.remote_request = false;
				tx_frame_1.data_len = 3;
				tx_frame_1.data[0] = 0xAB;
				tx_frame_1.data[1] = 0xCD;
				tx_frame_1.data[2] = 0xEF;



				CAN_queue_frame(CAN_BASE_ADDR, &tx_frame_1);

				CAN_set_ctrl_flags(CAN_BASE_ADDR, 0x3);

			}
			else if(cmd_str[0] == '2')
			{
				axi_uart_init(UART_0_BASE_ADDR, 8, 0, 1, 115200);
				axi_uart_init(UART_1_BASE_ADDR, 8, 0, 1, 200000);
				axi_uart_init(UART_2_BASE_ADDR, 8, 0, 1, 300000);
				axi_uart_init(UART_3_BASE_ADDR, 8, 0, 1, 400000);
				axi_write(UART_0_BASE_ADDR + 0x414, 3);
			}
			else if(cmd_str[0] == '3')
			{
				unsigned int uart_data_tx_0[] = {0x0A, 0x0B, 0x0C, 0x0D, 0x01, 0x02, 0x03, 0x04};
				unsigned int uart_data_tx_1[] = {0x1A, 0x1B, 0x1C, 0x1D, 0x11, 0x12, 0x13, 0x14};
				unsigned int uart_data_tx_2[] = {0x2A, 0x2B, 0x2C, 0x2D, 0x21, 0x22, 0x23, 0x24};
				unsigned int uart_data_tx_3[] = {0x3A, 0x3B, 0x3C, 0x3D, 0x31, 0x32, 0x33, 0x34};

				axi_uart_send (UART_0_BASE_ADDR, uart_data_tx_0, sizeof(uart_data_tx_0)/4);
				axi_uart_send (UART_1_BASE_ADDR, uart_data_tx_1, sizeof(uart_data_tx_1)/4);
				axi_uart_send (UART_2_BASE_ADDR, uart_data_tx_2, sizeof(uart_data_tx_2)/4);
				axi_uart_send (UART_3_BASE_ADDR, uart_data_tx_3, sizeof(uart_data_tx_3)/4);


			}
			else if(cmd_str[0] == '4')
			{
				unsigned int uart_data_rx[] = {0, 0, 0, 0, 0, 0, 0, 0};
				unsigned int words_read = 0;

				axi_uart_read (UART_0_BASE_ADDR, uart_data_rx, sizeof(uart_data_rx)/4, &words_read);
				printf("\nUART 0 Read Data: ");
				for(int i = 0; i < words_read; i++)
					printf("0x%02x, ", uart_data_rx[i]);

				axi_uart_read (UART_1_BASE_ADDR, uart_data_rx, sizeof(uart_data_rx)/4, &words_read);
				printf("\nUART 1 Read Data: ");
				for(int i = 0; i < words_read; i++)
					printf("0x%02x, ", uart_data_rx[i]);

				axi_uart_read (UART_2_BASE_ADDR, uart_data_rx, sizeof(uart_data_rx)/4, &words_read);
				printf("\nUART 2 Read Data: ");
				for(int i = 0; i < words_read; i++)
					printf("0x%02x, ", uart_data_rx[i]);

				axi_uart_read (UART_3_BASE_ADDR, uart_data_rx, sizeof(uart_data_rx)/4, &words_read);
				printf("\nUART 3 Read Data: ");
				for(int i = 0; i < words_read; i++)
					printf("0x%02x, ", uart_data_rx[i]);


			}*/
			if(cmd_str[0] == '2')
			{
				axi_write(GPIO_BASE_ADDR + 0x7FC, 1);
				printf("GPIO RUNNING...");
			}
			else if(cmd_str[0] == '3')
			{


			}

			else if(cmd_str[0] == '4')
			{
				for(unsigned i=0; i < 16; i++)
				{
					axi_write(0x40000000 + i*4, 0);
				}

			}

			else if(cmd_str[0] == '5')
			{
				printf("\n");
				for(unsigned i=0; i < 16; i++)
				{
					printf("Data: %x\n", axi_read(0x40000000 + i*4));
				}

			}

			else if(cmd_str[0] == '6')
			{
				axi_dac_reset(DAC_BASE_ADDR);

			}

			else if(cmd_str[0] == '7')
			{

				unsigned int tx_data0[(sizeof(square_wave_mV)/4) + 2];
				unsigned int tx_data1[(sizeof(square_wave_mV)/4) + 2];
				unsigned int tx_data2[(sizeof(square_wave_mV)/4) + 2];
				unsigned int tx_data3[(sizeof(square_wave_mV)/4) + 2];

				tx_data0[0] = DAC_START;
				tx_data0[(sizeof(tx_data0)/4) - 1] = DAC_JUMP;

				tx_data1[0] = DAC_START;
				tx_data1[(sizeof(tx_data1)/4) - 1] = DAC_JUMP;

				tx_data2[0] = DAC_START;
				tx_data2[(sizeof(tx_data2)/4) - 1] = DAC_JUMP;

				tx_data3[0] = DAC_START;
				tx_data3[(sizeof(tx_data3)/4) - 1] = DAC_JUMP;



				for(int i = 0; i < sizeof(square_wave_mV)/4; i++)
				{
					tx_data0[1+i] = 0;//45901;//DAC_voltage_convert(square_wave_mV[i]);
					tx_data1[1+i] = 0;//DAC_voltage_convert(square_wave_mV[i]);
					tx_data2[1+i] = 6501;//DAC_voltage_convert(square_wave_mV[i]);
					tx_data3[1+i] = 0;//DAC_voltage_convert(square_wave_mV[i]);
				}

				//axi_dac_reset(DAC_BASE_ADDR);

				axi_dac_load_data(DAC_BASE_ADDR, 1<<0, sizeof(tx_data0)/4, tx_data0);
				axi_dac_load_ch_timer(DAC_BASE_ADDR, 1<<0, 1);
				axi_dac_load_ch_cal(DAC_BASE_ADDR, 1<<0, 32775, 24);

				axi_dac_load_data(DAC_BASE_ADDR, 1<<1, sizeof(tx_data1)/4, tx_data1);
				axi_dac_load_ch_timer(DAC_BASE_ADDR, 1<<1, 1);
				axi_dac_load_ch_cal(DAC_BASE_ADDR, 1<<1, 32781, 29);

				axi_dac_load_data(DAC_BASE_ADDR, 1<<2, sizeof(tx_data2)/4, tx_data2);
				axi_dac_load_ch_timer(DAC_BASE_ADDR, 1<<2, 1);
				axi_dac_load_ch_cal(DAC_BASE_ADDR, 1<<2, 32781, 24);

				axi_dac_load_data(DAC_BASE_ADDR, 1<<3, sizeof(tx_data3)/4, tx_data3);
				axi_dac_load_ch_timer(DAC_BASE_ADDR, 1<<3, 1);
				axi_dac_load_ch_cal(DAC_BASE_ADDR, 1<<3, 32775, 26);

				axi_dac_load_core_timer(DAC_BASE_ADDR, 1000);

				axi_dac_enable(DAC_BASE_ADDR, 0xF, 1);

			}
			else if(cmd_str[0] == '8')
			{
				//axi_dac_reset(DAC_BASE_ADDR);
				axi_dac_enable(DAC_BASE_ADDR, 0xF, 0);

			}
			else if(cmd_str[0] == '9')
			{

				ADC_Ch_Setup_Dat* adc_ch_data_0 = ADC_Ch_Setup_Dat_new(CHANNEL_NUM);


				axi_adc_channel_enable(ADC_BASE_ADDR, 0);

				if(CHANNEL_NUM > 1)
					axi_adc_set_channel_switch(ADC_BASE_ADDR, CHANNEL_NUM - 2, 1<<CHANNEL_POS, 1<<CHANNEL_NEG);


				int* read_data_0 = (int*)(ADC_SYS_RAM_BASE_ADDR + (CHANNEL_NUM*0x1000000));



				adc_ch_data_0->RAM_write_base_addr = (unsigned)(ADC_SYS_RAM_BASE_ADDR + (CHANNEL_NUM*0x1000000));
				adc_ch_data_0->force_trigger = FORCE_TRIG;
				adc_ch_data_0->trigger_delay = 0;
				adc_ch_data_0->adc_attenuation = ADC_ATTEN;
				adc_ch_data_0->rising_edge_trigger = ADC_TRIG_RISING_EDGE;
				adc_ch_data_0->trigger_threshold_val = (unsigned short)mV_to_adc_2s_comp(ADC_TRIG_VAL, adc_ch_data_0->adc_attenuation);
				adc_ch_data_0->sample_count = ADC_SAMPLE_COUNT;
				adc_ch_data_0->sample_rate = SAMPLE_RATE;
				adc_ch_data_0->adc_user_scale = 2500;



				axi_adc_setup_channel(ADC_BASE_ADDR, ATC_ADC_cal_data, adc_ch_data_0);


				axi_adc_channel_enable(ADC_BASE_ADDR, 1<<CHANNEL_NUM);
				axi_dmm_enable(DMM_BASE_ADDR);

				long voltage_ave;
                
#if (!WAVE_PRINT_ON)
				printf("\n\n");
#if (!CAL_DMM)
				while(1)
				{
#endif
					//printf("\n\n\b\r\b\r");
					printf("|");

#endif

					for(int j = CHANNEL_NUM; j < CHANNEL_NUM+1; j++)
					{
						voltage_ave = 0;
						for(int a = 0; a < CAPTURE_COUNT; a++)
						{
							axi_adc_set_trigger_enable(ADC_BASE_ADDR, j, true);




							while(axi_adc_get_status(ADC_BASE_ADDR, j))
							{


							}
							printf("\nAve Freq: %u\n", axi_dmm_get_ave_freq(DMM_BASE_ADDR, CHANNEL_NUM));
							printf("\nInst Freq: %u\n", axi_dmm_get_inst_freq(DMM_BASE_ADDR, CHANNEL_NUM));
							printf("\nAve RMS: %u\n", axi_dmm_get_ave_RMS(DMM_BASE_ADDR, CHANNEL_NUM));
							printf("\nInst RMS: %u\n", axi_dmm_get_inst_RMS(DMM_BASE_ADDR, CHANNEL_NUM));

							usleep(10000);



							uart_pause_counter = 0;
#if WAVE_PRINT_ON
							if(BIN_SAVE)
							{
								printf("\n\n\n\n%08x", 0);
								if(j > 1)
									printf("\n%08x", 1000000/SAMPLE_RATE);
								else
									printf("\n%08x", 25000000/SAMPLE_RATE);

								printf("\n%08x", 0);
								printf("\n%08x", 0);
								printf("\n%08x", ADC_SAMPLE_COUNT);
							}
#endif
							for(unsigned i = 0; i < ADC_SAMPLE_COUNT; i++)
							{
#if WAVE_PRINT_ON
								if(BIN_SAVE)
								{

									printf("\n%02x", (read_data_0[i] & 0xFF));
									printf("%02x", (read_data_0[i]>>8) & 0xFF);
									printf("%02x", (read_data_0[i]>>16) & 0xFF);
									printf("%02x", (read_data_0[i]>>24) & 0xFF);
								}
								else
									printf("\n%i", read_data_0[i]);

#elif RMS_ON
									voltage_ave += read_data_0[i] * read_data_0[i];
#else
									voltage_ave += read_data_0[i];
#endif
							}
#if WAVE_PRINT_ON
							if(BIN_SAVE)
								printf("\nEOB\n");
#endif


						}

#if (!WAVE_PRINT_ON)
						{
							voltage_ave = voltage_ave/ADC_SAMPLE_COUNT/CAPTURE_COUNT;
#if RMS_ON
								voltage_ave = sqrt64(voltage_ave);
#endif


							if((voltage_ave < 0) && (voltage_ave >= -1000))
							{
								printf("     Ch%i:  -0.%.3i    |", j, (int)(abs(voltage_ave%1000)));
							}
							else
							{
								printf("     Ch%i: %3i.%.3i    |", j, (int)(voltage_ave/1000), (int)(abs(voltage_ave%1000)));
							}



						}
#endif

					}




#if ((!WAVE_PRINT_ON) && (!CAL_DMM))
				}
#endif


			}
			else
				printf("\nInvalid Entry!");

		}
		else
		{
			printf("\nInvalid Entry!");
		}
    }



    cleanup_platform();
    return 0;
}
