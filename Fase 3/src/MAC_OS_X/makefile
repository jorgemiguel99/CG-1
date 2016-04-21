generate:
		g++ -framework GLUT -framework OpenGL -framework Cocoa generate.cp -o generate -Wno-deprecated

engine:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib engine.cp -o engine -Wno-deprecated

clear:
		rm *.3d

all:	generate engine 
