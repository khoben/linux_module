TARGET = phonebook

$(TARGET)-objs := main.o phone_book.o
obj-m += $(TARGET).o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
test:
	sudo dmesg -C
	sudo insmod main.ko
	sudo rmmod main.ko
	dmesg

