################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g0b1cbtxn.s 

OBJS += \
./Core/Startup/startup_stm32g0b1cbtxn.o 

S_DEPS += \
./Core/Startup/startup_stm32g0b1cbtxn.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -I"C:/Users/mzeml/embedded/nemo2.space_tracker_p_2/my_drivers/Inc" -I"C:/Users/mzeml/embedded/nemo2.space_tracker_p_2/my_libraries/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g0b1cbtxn.d ./Core/Startup/startup_stm32g0b1cbtxn.o

.PHONY: clean-Core-2f-Startup

