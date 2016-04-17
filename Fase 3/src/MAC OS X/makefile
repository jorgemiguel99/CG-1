all:	generate

engine:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib engine.cp -o engine -Wno-deprecated

generate:
		g++ -framework GLUT -framework OpenGL -framework Cocoa generate.cp -o generate -Wno-deprecated

clear:
		rm generate
