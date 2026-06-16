TARGET_MODULE:=simple-module


# If we are running with kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := main.o device_file.o
	obj-m := $(TARGET_MODULE).o


# If we are running without kernel build system
else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all :
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules


clean:
# run kernel build system to clean up in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean


load:
	sudo insmod ./$(TARGET_MODULE).ko


unload:
	sudo rmmod ./$(TARGET_MODULE).ko

endif
