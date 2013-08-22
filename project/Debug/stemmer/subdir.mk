################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../stemmer/common_lang_constants.cpp 

OBJS += \
./stemmer/common_lang_constants.o 

CPP_DEPS += \
./stemmer/common_lang_constants.d 


# Each subdirectory must supply rules for building sources it contributes
stemmer/%.o: ../stemmer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


