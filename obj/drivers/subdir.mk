################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/drv_gpio.c \
../drivers/drv_usart.c 

OBJS += \
./drivers/drv_gpio.o \
./drivers/drv_usart.o 

C_DEPS += \
./drivers/drv_gpio.d \
./drivers/drv_usart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Debug" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Core" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\User" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Peripheral\inc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\drivers" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include\libc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v\common" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\src" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\misc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\serial" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

