################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/AXI_mem_access.c \
../src/main.c \
../src/platform.c 

OBJS += \
./src/AXI_mem_access.o \
./src/main.o \
./src/platform.o 

C_DEPS += \
./src/AXI_mem_access.d \
./src/main.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -I"C:\git\noh-firmware\AXI_CAN_core\drivers" -I"C:\git\noh-firmware\AXI_DMM_core\drivers" -I"C:\git\noh-firmware\AXI_ADC_core\drivers" -I"C:\git\noh-firmware\AXI_DAC_core\drivers" -I"C:\git\noh-firmware\AXI_I2C_core\drivers" -I"C:\git\noh-firmware\AXI_UART_core\drivers" -c -fmessage-length=0 -MT"$@" -I../../standalone_bsp_0/psu_cortexa53_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


