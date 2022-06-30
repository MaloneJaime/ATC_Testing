################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/git/noh-firmware/AXI_ADC_core/drivers/AXI_ADC_core_driver.c 

OBJS += \
./noh-firmware/AXI_ADC_core/drivers/AXI_ADC_core_driver.o 

C_DEPS += \
./noh-firmware/AXI_ADC_core/drivers/AXI_ADC_core_driver.d 


# Each subdirectory must supply rules for building sources it contributes
noh-firmware/AXI_ADC_core/drivers/AXI_ADC_core_driver.o: C:/git/noh-firmware/AXI_ADC_core/drivers/AXI_ADC_core_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -I"C:\git\noh-firmware\AXI_CAN_core\drivers" -I"C:\git\noh-firmware\AXI_DMM_core\drivers" -I"C:\git\noh-firmware\AXI_ADC_core\drivers" -I"C:\git\noh-firmware\AXI_DAC_core\drivers" -I"C:\git\noh-firmware\AXI_I2C_core\drivers" -I"C:\git\noh-firmware\AXI_UART_core\drivers" -c -fmessage-length=0 -MT"$@" -I../../standalone_bsp_0/psu_cortexa53_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


