################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../usbdrvasm.asm 

C_SRCS += \
../UsbMiniMediaKeyboard.c \
../oddebug.c \
../usbdrv.c 

S_UPPER_SRCS += \
../usbdrvasm.S 

OBJS += \
./UsbMiniMediaKeyboard.o \
./oddebug.o \
./usbdrv.o \
./usbdrvasm.o 

ASM_DEPS += \
./usbdrvasm.d 

S_UPPER_DEPS += \
./usbdrvasm.d 

C_DEPS += \
./UsbMiniMediaKeyboard.d \
./oddebug.d \
./usbdrv.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O2 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=attiny2313 -DF_CPU=12000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -g2 -gstabs -mmcu=attiny2313 -DF_CPU=12000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -g2 -gstabs -mmcu=attiny2313 -DF_CPU=12000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


