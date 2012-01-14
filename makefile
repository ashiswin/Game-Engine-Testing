INCLUDEDIR=-IIncludes
LIBDIR=-LLib
LIBRARIES=-lengine -framework OpenGL -framework GLUT
CFLAGS=$(INCLUDEDIR) $(LIBDIR) $(LIBRARIES)
all: tools

library: clean
	cd Src; g++ -c *.cpp -I../Includes
	ar rcs Lib/libengine.a Src/*.o

tools: library
	g++ Tools/OBJ2SMF.cpp -o Bin/Tools/OBJ2SMF $(CFLAGS)
	g++ Tools/SMFFileTester.cpp -o Bin/Tools/SMFFileTester $(CFLAGS)
	g++ Tools/TGA2MTF.cpp -o Bin/Tools/TGA2MTF $(CFLAGS)
	g++ Tools/SMFLoader.cpp -o Bin/Tools/SMFLoader $(CFLAGS)
	
clean:
	rm Lib/*.a
	rm Src/*.o