generate:
		g++ -framework GLUT -framework OpenGL -framework Cocoa generate.cp -o generate -Wno-deprecated

engine:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib engine.cp -o engine -Wno-deprecated

lights:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEW.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.1.13.dylib /usr/local/Cellar/glew/1.13.0/lib/libGLEWmx.dylib SphereLights.cp -o SphereLights -Wno-deprecated

clear:
		rm *.3d

all:	generate engine lights
