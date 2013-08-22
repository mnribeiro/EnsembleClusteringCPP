################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../clustering/avaliacao.cpp \
../clustering/ensemble.cpp \
../clustering/estabilidade.cpp \
../clustering/featureselection.cpp \
../clustering/nuvens.cpp \
../clustering/prototipacao.cpp \
../clustering/prototipacao_caracteristicas.cpp \
../clustering/wrapper.cpp 

OBJS += \
./clustering/avaliacao.o \
./clustering/ensemble.o \
./clustering/estabilidade.o \
./clustering/featureselection.o \
./clustering/nuvens.o \
./clustering/prototipacao.o \
./clustering/prototipacao_caracteristicas.o \
./clustering/wrapper.o 

CPP_DEPS += \
./clustering/avaliacao.d \
./clustering/ensemble.d \
./clustering/estabilidade.d \
./clustering/featureselection.d \
./clustering/nuvens.d \
./clustering/prototipacao.d \
./clustering/prototipacao_caracteristicas.d \
./clustering/wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
clustering/%.o: ../clustering/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


