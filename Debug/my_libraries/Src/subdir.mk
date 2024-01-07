################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../my_libraries/Src/my_conversions.c \
../my_libraries/Src/my_global.c \
../my_libraries/Src/my_nmea.c \
../my_libraries/Src/my_rand.c 

OBJS += \
./my_libraries/Src/my_conversions.o \
./my_libraries/Src/my_global.o \
./my_libraries/Src/my_nmea.o \
./my_libraries/Src/my_rand.o 

C_DEPS += \
./my_libraries/Src/my_conversions.d \
./my_libraries/Src/my_global.d \
./my_libraries/Src/my_nmea.d \
./my_libraries/Src/my_rand.d 


# Each subdirectory must supply rules for building sources it contributes
my_libraries/Src/%.o my_libraries/Src/%.su my_libraries/Src/%.cyclo: ../my_libraries/Src/%.c my_libraries/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/mzeml/embedded/nemo2.space_tracker_p_2/my_drivers/Inc" -I"C:/Users/mzeml/embedded/nemo2.space_tracker_p_2/my_libraries/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-my_libraries-2f-Src

clean-my_libraries-2f-Src:
	-$(RM) ./my_libraries/Src/my_conversions.cyclo ./my_libraries/Src/my_conversions.d ./my_libraries/Src/my_conversions.o ./my_libraries/Src/my_conversions.su ./my_libraries/Src/my_global.cyclo ./my_libraries/Src/my_global.d ./my_libraries/Src/my_global.o ./my_libraries/Src/my_global.su ./my_libraries/Src/my_nmea.cyclo ./my_libraries/Src/my_nmea.d ./my_libraries/Src/my_nmea.o ./my_libraries/Src/my_nmea.su ./my_libraries/Src/my_rand.cyclo ./my_libraries/Src/my_rand.d ./my_libraries/Src/my_rand.o ./my_libraries/Src/my_rand.su

.PHONY: clean-my_libraries-2f-Src

