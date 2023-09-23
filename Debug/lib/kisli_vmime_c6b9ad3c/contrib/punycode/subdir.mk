################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/kisli_vmime_c6b9ad3c/contrib/punycode/punycode.c 

C_DEPS += \
./lib/kisli_vmime_c6b9ad3c/contrib/punycode/punycode.d 

OBJS += \
./lib/kisli_vmime_c6b9ad3c/contrib/punycode/punycode.o 


# Each subdirectory must supply rules for building sources it contributes
lib/kisli_vmime_c6b9ad3c/contrib/punycode/%.o: ../lib/kisli_vmime_c6b9ad3c/contrib/punycode/%.c lib/kisli_vmime_c6b9ad3c/contrib/punycode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -I../lib/lexborisov_myhtml/include -I../lib/kisli_vmime_c6b9ad3c -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-contrib-2f-punycode

clean-lib-2f-kisli_vmime_c6b9ad3c-2f-contrib-2f-punycode:
	-$(RM) ./lib/kisli_vmime_c6b9ad3c/contrib/punycode/punycode.d ./lib/kisli_vmime_c6b9ad3c/contrib/punycode/punycode.o

.PHONY: clean-lib-2f-kisli_vmime_c6b9ad3c-2f-contrib-2f-punycode

