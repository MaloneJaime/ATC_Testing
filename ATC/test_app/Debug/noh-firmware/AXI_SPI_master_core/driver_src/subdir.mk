################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/git/noh-firmware/AXI_SPI_master_core/driver_src/AXI_SPI_master_core.c \
C:/git/noh-firmware/AXI_SPI_master_core/driver_src/AXI_mem_access.c \
C:/git/noh-firmware/AXI_SPI_master_core/driver_src/main.c 

OBJS += \
./noh-firmware/AXI_SPI_master_core/driver_src/AXI_SPI_master_core.o \
./noh-firmware/AXI_SPI_master_core/driver_src/AXI_mem_access.o \
./noh-firmware/AXI_SPI_master_core/driver_src/main.o 

C_DEPS += \
./noh-firmware/AXI_SPI_master_core/driver_src/AXI_SPI_master_core.d \
./noh-firmware/AXI_SPI_master_core/driver_src/AXI_mem_access.d \
./noh-firmware/AXI_SPI_master_core/driver_src/main.d 


# Each subdirectory must supply rules for building sources it contributes
noh-firmware/AXI_SPI_master_core/driver_src/AXI_SPI_master_core.o: C:/git/noh-firmware/AXI_SPI_master_core/driver_src/AXI_SPI_master_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -I"C:\git\noh-firmware\AXI_CAN_core\drivers" -I"C:\git\noh-firmware\AXI_DAC_core\drivers" -I"C:\git\noh-firmware\AXI_I2C_core\drivers" -I"C:\git\noh-firmware\AXI_UART_core\drivers" -c -fmessage-length=0 -MT"$@" -I../../standalone_bsp_0/psu_cortexa53_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

noh-firmware/AXI_SPI_master_core/driver_src/AXI_mem_access.o: C:/git/noh-firmware/AXI_SPI_master_core/driver_src/AXI_mem_access.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -I"C:\git\noh-firmware\AXI_CAN_core\drivers" -I"C:\git\noh-firmware\AXI_DAC_core\drivers" -I"C:\git\noh-firmware\AXI_I2C_core\drivers" -I"C:\git\noh-firmware\AXI_UART_core\drivers" -c -fmessage-length=0 -MT"$@" -I../../standalone_bsp_0/psu_cortexa53_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

noh-firmware/AXI_SPI_master_core/driver_src/main.o: C:/git/noh-firmware/AXI_SPI_master_core/driver_src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -I"C:\git\noh-firmware\AXI_CAN_core\drivers" -I"C:\git\noh-firmware\AXI_DAC_core\drivers" -I"C:\git\noh-firmware\AXI_I2C_core\drivers" -I"C:\git\noh-firmware\AXI_UART_core\drivers" -c -fmessage-length=0 -MT"$@" -I../../standalone_bsp_0/psu_cortexa53_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


