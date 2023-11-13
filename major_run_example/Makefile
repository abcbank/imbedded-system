obj-m += gpio_driver.o
RESULT = led_button_example
SRC = $(RESULT).c

all:
	make -C ~/ws/kernel M=$(PWD) modules
	aarch64-linux-gnu-gcc -O $(RESULT) $(SRC)

clean:
	make -C ~/ws/kernel M=$(PWD) clean
	rm -f $(RESULT)
