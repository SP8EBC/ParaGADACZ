################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../lib/wjwwood_serial/src/impl/unix.cc \
../lib/wjwwood_serial/src/impl/win.cc 

CC_DEPS += \
./lib/wjwwood_serial/src/impl/unix.d \
./lib/wjwwood_serial/src/impl/win.d 

OBJS += \
./lib/wjwwood_serial/src/impl/unix.o \
./lib/wjwwood_serial/src/impl/win.o 


# Each subdirectory must supply rules for building sources it contributes
lib/wjwwood_serial/src/impl/%.o: ../lib/wjwwood_serial/src/impl/%.cc lib/wjwwood_serial/src/impl/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG -DPANSA_AIRSPACE -I../lib/nlohman_json_3_7_1/include/ -I../lib/kisli_vmime_c6b9ad3c -I../lib/lexborisov_myhtml/include -I../lib/HtmlParser/include -I../lib/wjwwood_serial/include -I../meteoblue/api -I../meteoblue/model -I../weatherlink_v1/api/ -I../weatherlink_v1/model/ -I../pogodacc/api -I../pogodacc -I../pogodacc/model -I/usr/include/curl -I/usr/include/spdlog -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -Wunused-function -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-wjwwood_serial-2f-src-2f-impl

clean-lib-2f-wjwwood_serial-2f-src-2f-impl:
	-$(RM) ./lib/wjwwood_serial/src/impl/unix.d ./lib/wjwwood_serial/src/impl/unix.o ./lib/wjwwood_serial/src/impl/win.d ./lib/wjwwood_serial/src/impl/win.o

.PHONY: clean-lib-2f-wjwwood_serial-2f-src-2f-impl

