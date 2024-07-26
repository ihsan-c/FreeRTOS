################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SEGGER/rec/segger_uart.c 

OBJS += \
./ThirdParty/SEGGER/rec/segger_uart.o 

C_DEPS += \
./ThirdParty/SEGGER/rec/segger_uart.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SEGGER/rec/%.o ThirdParty/SEGGER/rec/%.su ThirdParty/SEGGER/rec/%.cyclo: ../ThirdParty/SEGGER/rec/%.c ThirdParty/SEGGER/rec/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/ihsan/STM32CubeIDE/workspace_1.12.0/Task001_Segger/ThirdParty/SEGGER/Config" -I"C:/Users/ihsan/STM32CubeIDE/workspace_1.12.0/Task001_Segger/ThirdParty/SEGGER/OS" -I"C:/Users/ihsan/STM32CubeIDE/workspace_1.12.0/Task001_Segger/ThirdParty/SEGGER/SEGGER" -I"C:/Users/ihsan/STM32CubeIDE/workspace_1.12.0/Task001_Segger/ThirdParty/FreeRTOS" -I"C:/Users/ihsan/STM32CubeIDE/workspace_1.12.0/Task001_Segger/ThirdParty/FreeRTOS/include" -I"C:/Users/ihsan/STM32CubeIDE/workspace_1.12.0/Task001_Segger/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-SEGGER-2f-rec

clean-ThirdParty-2f-SEGGER-2f-rec:
	-$(RM) ./ThirdParty/SEGGER/rec/segger_uart.cyclo ./ThirdParty/SEGGER/rec/segger_uart.d ./ThirdParty/SEGGER/rec/segger_uart.o ./ThirdParty/SEGGER/rec/segger_uart.su

.PHONY: clean-ThirdParty-2f-SEGGER-2f-rec

