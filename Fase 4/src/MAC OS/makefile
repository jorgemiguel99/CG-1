generator: generator.cpp
		g++ -framework GLUT -framework OpenGL -framework Cocoa generator.cpp -o Generator -Wno-deprecated

engine: engine.cpp read.cpp read.h
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.dylib engine.cpp read.cpp -o Engine -Wno-deprecated

xml:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.dylib generateXML.cpp -o generateXML -Wno-deprecated

lights:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.dylib SphereLights.cp -o SphereLights -Wno-deprecated

clear:
		rm *.3d

all:		generator engine xml lights
