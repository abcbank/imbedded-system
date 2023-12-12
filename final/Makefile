all:
	sudo gcc -c Modules/IOController.cpp -o Modules/IOController.o
	ar rvs output/libIO.a Modules/IOController.o
	sudo gcc -c Modules/SegmentController.cpp -o Modules/SegmentController.o
	ar rvs output/libSeg.a Modules/SegmentController.o
	sudo gcc -c Modules/Camera.cpp -o Modules/Camera.o `pkg-config --cflags --libs opencv4`
	ar rvs output/libCamera.a Modules/Camera.o
	sudo gcc -c Modules/Master.cpp `pkg-config  --cflags --libs opencv4` -o Modules/Master.o
	ar rvs output/libMaster.a Modules/Master.o
	sudo gcc main.cpp -L./output -lMaster -lCamera -lSeg -lIO `pkg-config --cflags --libs opencv4` -lpthread -lstdc++ -o output/main

clean:
	rm -f $(RESULT)
