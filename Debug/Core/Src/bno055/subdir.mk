################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bno055/bno055.c \
../Core/Src/bno055/bno055UKB.c 

OBJS += \
./Core/Src/bno055/bno055.o \
./Core/Src/bno055/bno055UKB.o 

C_DEPS += \
./Core/Src/bno055/bno055.d \
./Core/Src/bno055/bno055UKB.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bno055/%.o Core/Src/bno055/%.su Core/Src/bno055/%.cyclo: ../Core/Src/bno055/%.c Core/Src/bno055/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-bno055

clean-Core-2f-Src-2f-bno055:
	-$(RM) ./Core/Src/bno055/bno055.cyclo ./Core/Src/bno055/bno055.d ./Core/Src/bno055/bno055.o ./Core/Src/bno055/bno055.su ./Core/Src/bno055/bno055UKB.cyclo ./Core/Src/bno055/bno055UKB.d ./Core/Src/bno055/bno055UKB.o ./Core/Src/bno055/bno055UKB.su

.PHONY: clean-Core-2f-Src-2f-bno055

