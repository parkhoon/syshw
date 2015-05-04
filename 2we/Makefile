obj-m := chr_dev.o

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

All: test_dd test_app

test_dd: chr_dev.c
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) modules
test_app : chr_appl.c
	gcc -o chr_appl chr_appl.c

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf gyapp2
