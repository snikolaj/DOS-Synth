################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Debug" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Core" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\User" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Peripheral\inc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\drivers" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include\libc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v\common" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\src" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\misc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\serial" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

