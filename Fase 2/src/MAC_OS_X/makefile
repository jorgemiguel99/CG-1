all:	1	2	3	4

1:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib -c readXML_Fase2.cp readXML_Fase2.h -Wno-deprecated
2:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib -c solarSystem.cp -Wno-deprecated
3:
		g++ -framework GLUT -framework OpenGL -framework Cocoa /usr/local/Cellar/tinyxml/2.6.2/lib/libtinyxml.dylib readXML_Fase2.o solarSystem.o -o fase2
4:
		rm *.o
