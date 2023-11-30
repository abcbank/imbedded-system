obj-m += seg_drv.o
obj-m += btn_drv.o
RESULT = hw
SRC = $(RESULT).c

all:
	make -C ~/ws/kernel M=$(PWD) modules
	aarch64-linux-gnu-gcc -o $(RESULT) $(SRC)

clean:
	make -C ~/ws/kernel M=$(PWD) clean
	rm -f $(RESULT)
