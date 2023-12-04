################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/gpio.c \
../MCAL/i2c.c \
../MCAL/timer0_pwm.c \
../MCAL/timer1.c \
../MCAL/uart.c 

OBJS += \
./MCAL/gpio.o \
./MCAL/i2c.o \
./MCAL/timer0_pwm.o \
./MCAL/timer1.o \
./MCAL/uart.o 

C_DEPS += \
./MCAL/gpio.d \
./MCAL/i2c.d \
./MCAL/timer0_pwm.d \
./MCAL/timer1.d \
./MCAL/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


