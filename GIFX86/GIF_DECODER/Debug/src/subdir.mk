################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gif_display.c \
../src/gif_lzw.c \
../src/gif_read.c \
../src/sysm.c \
../src/x86_main.c 

OBJS += \
./src/gif_display.o \
./src/gif_lzw.o \
./src/gif_read.o \
./src/sysm.o \
./src/x86_main.o 

C_DEPS += \
./src/gif_display.d \
./src/gif_lzw.d \
./src/gif_read.d \
./src/sysm.d \
./src/x86_main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


