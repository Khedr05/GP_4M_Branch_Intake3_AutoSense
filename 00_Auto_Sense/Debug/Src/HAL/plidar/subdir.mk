################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/HAL/plidar/rplidar_a2m8.c 

OBJS += \
./Src/HAL/plidar/rplidar_a2m8.o 

C_DEPS += \
./Src/HAL/plidar/rplidar_a2m8.d 


# Each subdirectory must supply rules for building sources it contributes
Src/HAL/plidar/%.o Src/HAL/plidar/%.su Src/HAL/plidar/%.cyclo: ../Src/HAL/plidar/%.c Src/HAL/plidar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401CCUx -DSTM32 -DSTM32F4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-HAL-2f-plidar

clean-Src-2f-HAL-2f-plidar:
	-$(RM) ./Src/HAL/plidar/rplidar_a2m8.cyclo ./Src/HAL/plidar/rplidar_a2m8.d ./Src/HAL/plidar/rplidar_a2m8.o ./Src/HAL/plidar/rplidar_a2m8.su

.PHONY: clean-Src-2f-HAL-2f-plidar

