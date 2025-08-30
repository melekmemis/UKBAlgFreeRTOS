################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/gps/nmea_parse.c 

OBJS += \
./Core/Src/gps/nmea_parse.o 

C_DEPS += \
./Core/Src/gps/nmea_parse.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/gps/%.o Core/Src/gps/%.su Core/Src/gps/%.cyclo: ../Core/Src/gps/%.c Core/Src/gps/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-gps

clean-Core-2f-Src-2f-gps:
	-$(RM) ./Core/Src/gps/nmea_parse.cyclo ./Core/Src/gps/nmea_parse.d ./Core/Src/gps/nmea_parse.o ./Core/Src/gps/nmea_parse.su

.PHONY: clean-Core-2f-Src-2f-gps

