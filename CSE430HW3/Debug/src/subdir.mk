################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/one_unisexRR.c \
../src/two_seperateRR.c \
../src/two_unisexRR.c 

OBJS += \
./src/one_unisexRR.o \
./src/two_seperateRR.o \
./src/two_unisexRR.o 

C_DEPS += \
./src/one_unisexRR.d \
./src/two_seperateRR.d \
./src/two_unisexRR.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


