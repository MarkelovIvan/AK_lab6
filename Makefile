ifneq ($(KERNELRELEASE),)
# kbuild part of Makefile
obj-m := hello2.o
ccflags-y += -g

else
# Normal Makefile
KDIR ?= /home/kpi/repos/linux-stable

default:
	$(MAKE) -C $(KDIR) M=$(PWD)

	cp hello2.ko hello2.ko.unstripped
	$(CROSS_COMPILE)strip -g hello2.ko # Strip only debugging info

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

%.s %.i: %.c # Generate assembly or preprocessed output
	$(MAKE) -C $(KDIR) M=$(PWD) $@

endif
