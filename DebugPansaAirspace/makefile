################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include weatherlink_v1/model/subdir.mk
-include weatherlink_v1/api/subdir.mk
-include src/subdir.mk
-include pogodacc/model/subdir.mk
-include pogodacc/api/subdir.mk
-include pogodacc/subdir.mk
-include meteoblue/model/subdir.mk
-include meteoblue/api/subdir.mk
-include lib/wjwwood_serial/src/impl/list_ports/subdir.mk
-include lib/wjwwood_serial/src/impl/subdir.mk
-include lib/wjwwood_serial/src/subdir.mk
-include lib/lexborisov_myhtml/source/myport/posix/mycore/utils/subdir.mk
-include lib/lexborisov_myhtml/source/myport/posix/mycore/subdir.mk
-include lib/lexborisov_myhtml/source/myhtml/subdir.mk
-include lib/lexborisov_myhtml/source/myencoding/subdir.mk
-include lib/lexborisov_myhtml/source/mycore/utils/subdir.mk
-include lib/lexborisov_myhtml/source/mycore/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/utility/sync/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/utility/encoder/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/utility/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/sasl/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/digest/sha1/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/digest/md5/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/digest/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/cert/openssl/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/cert/gnutls/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/cert/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/security/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/platforms/windows/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/platforms/posix/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/tls/openssl/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/tls/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/smtp/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/sendmail/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/pop3/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/format/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/maildir/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/imap/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/net/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/misc/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/mdn/subdir.mk
-include lib/kisli_vmime_c6b9ad3c/vmime/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := ParaGADACZ
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: ParaGADACZ

# Tool invocations
ParaGADACZ: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/lib/x86_64-linux-gnu -L/lib/i386-linux-gnu -L/usr/lib -o "ParaGADACZ" $(OBJS) $(USER_OBJS) $(LIBS) -lconfig++ -lctemplate -lpqxx -lpq -lgsasl -licuuc -lssl -lcurl -lfmt -lcrypto -lpthread -lcpprest -lboost_filesystem -lboost_chrono -lboost_thread -lboost_regex -lvlc -lconfig
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ParaGADACZ
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
