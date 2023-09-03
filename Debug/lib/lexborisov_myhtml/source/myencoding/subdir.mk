################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/lexborisov_myhtml/source/myencoding/detect.c \
../lib/lexborisov_myhtml/source/myencoding/encoding.c \
../lib/lexborisov_myhtml/source/myencoding/mystring.c 

C_DEPS += \
./lib/lexborisov_myhtml/source/myencoding/detect.d \
./lib/lexborisov_myhtml/source/myencoding/encoding.d \
./lib/lexborisov_myhtml/source/myencoding/mystring.d 

OBJS += \
./lib/lexborisov_myhtml/source/myencoding/detect.o \
./lib/lexborisov_myhtml/source/myencoding/encoding.o \
./lib/lexborisov_myhtml/source/myencoding/mystring.o 


# Each subdirectory must supply rules for building sources it contributes
lib/lexborisov_myhtml/source/myencoding/%.o: ../lib/lexborisov_myhtml/source/myencoding/%.c lib/lexborisov_myhtml/source/myencoding/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -I../lib/kisli_vmime_c6b9ad3c -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myencoding

clean-lib-2f-lexborisov_myhtml-2f-source-2f-myencoding:
	-$(RM) ./lib/lexborisov_myhtml/source/myencoding/detect.d ./lib/lexborisov_myhtml/source/myencoding/detect.o ./lib/lexborisov_myhtml/source/myencoding/encoding.d ./lib/lexborisov_myhtml/source/myencoding/encoding.o ./lib/lexborisov_myhtml/source/myencoding/mystring.d ./lib/lexborisov_myhtml/source/myencoding/mystring.o

.PHONY: clean-lib-2f-lexborisov_myhtml-2f-source-2f-myencoding

