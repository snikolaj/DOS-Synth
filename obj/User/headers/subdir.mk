################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/headers/adc_helpers.c \
../User/headers/analog_sensors.c 

OBJS += \
./User/headers/adc_helpers.o \
./User/headers/analog_sensors.o 

C_DEPS += \
./User/headers/adc_helpers.d \
./User/headers/analog_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
User/headers/%.o: ../User/headers/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Debug" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Core" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\User" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Peripheral\inc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\drivers" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include\libc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v\common" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\src" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\misc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\serial" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

