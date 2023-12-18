all:
	sudo gcc -c Modules/IOController.cpp -o Modules/IOController.o
	ar rvs output/libIO.a Modules/IOController.o
	sudo gcc -c Modules/SegmentController.cpp -o Modules/SegmentController.o
	ar rvs output/libSeg.a Modules/SegmentController.o
	sudo gcc -c Modules/ConvyerController.cpp -o Modules/ConvyerController.o
	ar rvs output/libConvyer.a Modules/ConvyerController.o
	# sudo gcc -c Modules/HCSR04.cpp -o Modules/HCSR04.o
	# ar rvs output/libHCSR04.a Modules/HCSR04.o
	sudo gcc -c Modules/tf.cpp -I/usr/include/python3.9 -o Modules/TFModel.o 
	ar rvs output/libTFModel.a Modules/TFModel.o
	sudo gcc -c Modules/Camera.cpp -o Modules/Camera.o `pkg-config --cflags --libs opencv4`
	ar rvs output/libCamera.a Modules/Camera.o
	sudo gcc -c Modules/Master.cpp `pkg-config  --cflags --libs opencv4` -I/usr/include/python3.9 -o Modules/Master.o
	ar rvs output/libMaster.a Modules/Master.o
	sudo gcc main.cpp -L./output -lMaster -lTFModel -lCamera -lConvyer -lSeg -lIO `pkg-config --cflags --libs opencv4` -lpi-gpio -lpthread -lstdc++ -I/usr/include/python3.9 -lpython3.9 -o output/main

clean:
	rm -f $(RESULT)
