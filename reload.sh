sudo rmmod phonebook.ko
sudo insmod phonebook.ko
sudo unlink /dev/phonebook
sudo mknod /dev/phonebook c 75 0
