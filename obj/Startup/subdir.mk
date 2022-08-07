################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Startup/startup_ch32v30x.S 

OBJS += \
./Startup/startup_ch32v30x.o 

S_UPPER_DEPS += \
./Startup/startup_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.S
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"C:\MRS_DATA\workspace\RT-Thread_test-1\Startup" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\drivers" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\include\libc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\libcpu\risc-v\common" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\src" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\include" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\misc" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\drivers\serial" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread\components\finsh" -I"C:\MRS_DATA\workspace\RT-Thread_test-1\rtthread" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

