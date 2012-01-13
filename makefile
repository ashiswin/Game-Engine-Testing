INCLUDEDIR=-IIncludes
LIBDIR=-LLib
LIBRARIES=-lengine
CFLAGS=$(INCLUDEDIR) $(LIBDIR) $(LIBRARIES)
all: tools

library: clean
	cd Src; g++ -c *.cpp -I../Includes
	ar rcs Lib/libengine.a Src/*.o

tools: library
	g++ Tools/OBJ2SMF.cpp -o Bin/Tools/OBJ2SMF $(CFLAGS) -framework OpenGL -framework GLUT
	g++ Tools/SMFFileTester.cpp -o Bin/Tools/SMFFileTester $(CFLAGS) -framework OpenGL -framework GLUT
	G++ Tools/TGA2MTF.cpp -o Bin/Tools/TGA2MTF $(CFLAGS) -framework OpenGL -framework GLUT
	g++ Tools/SMFLoader.cpp -o Bin/Tools/SMFLoader $(CFLAGS) -framework OpenGL -framework GLUT
	
clean:
	rm Lib/*.a
	rm Src/*.o