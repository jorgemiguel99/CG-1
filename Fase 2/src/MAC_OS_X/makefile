drawSolarSystem:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib drawSolarSystem.cp -o drawSolarSystem -Wno-deprecated

generateSolarSystem:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib generateSolarSystem.cp -o generateSolarSystem -Wno-deprecated

solarSystem:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib solarSystem.cp -o solarSystem -Wno-deprecated

clear:
		rm *.3d

all:	generateSolarSystem drawSolarSystem solarSystem 
