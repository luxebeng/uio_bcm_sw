KDIR := /lib/modules/$(shell uname -r)/build
obj-m := uio_bcm_sw.o
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko
	rm -rf modules.order Module.symvers *.ko.unsigned *.mod.c
