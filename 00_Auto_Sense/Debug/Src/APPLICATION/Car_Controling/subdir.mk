################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/APPLICATION/Car_Controling/car_control_prog.c 

OBJS += \
./Src/APPLICATION/Car_Controling/car_control_prog.o 

C_DEPS += \
./Src/APPLICATION/Car_Controling/car_control_prog.d 


# Each subdirectory must supply rules for building sources it contributes
Src/APPLICATION/Car_Controling/%.o Src/APPLICATION/Car_Controling/%.su Src/APPLICATION/Car_Controling/%.cyclo: ../Src/APPLICATION/Car_Controling/%.c Src/APPLICATION/Car_Controling/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401CCUx -DSTM32 -DSTM32F4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-APPLICATION-2f-Car_Controling

clean-Src-2f-APPLICATION-2f-Car_Controling:
	-$(RM) ./Src/APPLICATION/Car_Controling/car_control_prog.cyclo ./Src/APPLICATION/Car_Controling/car_control_prog.d ./Src/APPLICATION/Car_Controling/car_control_prog.o ./Src/APPLICATION/Car_Controling/car_control_prog.su

.PHONY: clean-Src-2f-APPLICATION-2f-Car_Controling

