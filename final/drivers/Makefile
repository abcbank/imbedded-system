obj-m += convyer_drv.o
obj-m += output_drv.o
obj-m += input_drv.o
obj-m += seg_drv.o

all:
	make -C ~/ws/raspOS/linux M=$(PWD) modules

clean:
	make -C ~/ws/raspOS/linux M=$(PWD) clean