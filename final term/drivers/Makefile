obj-m += convyer_drv.o
obj-m += output_drv.o
obj-m += seg_drv.o

all:
	make -C ~/ws/kernel M=$(PWD) modules

clean:
	make -C ~/ws/kernel M=$(PWD) clean