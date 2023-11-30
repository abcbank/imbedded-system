obj-m += driver.o
RESULT_1 = seg_example
RESULT_2 = seg_example2
SRC_1 = $(RESULT_1).c
SRC_2 = $(RESULT_2).c

all:
	make -C ~/ws/kernel M=$(PWD) modules
	aarch64-linux-gnu-gcc -o $(RESULT_1) $(SRC_1)
	aarch64-linux-gnu-gcc -o $(RESULT_2) $(SRC_2)

clean:
	make -C ~/ws/kernel M=$(PWD) clean
	rm -f $(RESULT_1)
	rm -f $(RESULT_2)
