################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zian1/Src/BluePill.c \
../zian1/Src/Functions.c \
../zian1/Src/usb_keyboard.c \
../zian1/Src/ws2812b.c 

OBJS += \
./zian1/Src/BluePill.o \
./zian1/Src/Functions.o \
./zian1/Src/usb_keyboard.o \
./zian1/Src/ws2812b.o 

C_DEPS += \
./zian1/Src/BluePill.d \
./zian1/Src/Functions.d \
./zian1/Src/usb_keyboard.d \
./zian1/Src/ws2812b.d 


# Each subdirectory must supply rules for building sources it contributes
zian1/Src/%.o zian1/Src/%.su: ../zian1/Src/%.c zian1/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc -I../zian1/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-zian1-2f-Src

clean-zian1-2f-Src:
	-$(RM) ./zian1/Src/BluePill.d ./zian1/Src/BluePill.o ./zian1/Src/BluePill.su ./zian1/Src/Functions.d ./zian1/Src/Functions.o ./zian1/Src/Functions.su ./zian1/Src/usb_keyboard.d ./zian1/Src/usb_keyboard.o ./zian1/Src/usb_keyboard.su ./zian1/Src/ws2812b.d ./zian1/Src/ws2812b.o ./zian1/Src/ws2812b.su

.PHONY: clean-zian1-2f-Src

