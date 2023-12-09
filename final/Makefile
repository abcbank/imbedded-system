all:
	gcc -c Modules/IOController.cpp -o Modules/IOController.o
	ar rvs output/libIO.a Modules/IOController.o
	gcc -c Modules/SegmentController.cpp -o Modules/SegmentController.o
	ar rvs output/libSeg.a Modules/SegmentController.o
	gcc main.cpp -L./output -lSeg -lIO -lpthread -lstdc++ -o output/main

clean:
	rm -f $(RESULT)
