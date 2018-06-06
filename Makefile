.SUFFIXES:
.SUFFIXES:      .cpp .o .h

FMOD		= /Users/bathiamkwr/Documents/SchoolProjects/POOA/fmod/4.3
WFMOD		= /Users/bathiamkwr/Documents/SchoolProjects/POOA/fmod/Win-4.3
DJPEG		= /usr/local/Cellar/jpeg/9c/lib/

O			= Labyrinthe.o Chasseur.o Gardien.o
OBJ			= Labyrinthe.obj Chasseur.obj


LIBS		= $(FMOD)/libfmodex-4.32.04.so -lglut -lGLU -lGL -ljpeg -lm
WLIBS		= -defaultlib:glut32 -defaultlib:OPENGL32 -defaultlib:GLU32 $(DJPEG)/libjpeg.lib $(WFMOD)/fmodex_vc.lib
MLIBS		= -framework GLUT -framework OpenGL -framework Foundation $(DJPEG)/libjpeg.a $(FMOD)/libfmod.dylib

CXX		= g++ -g
CXXFLAGS	= -I $(FMOD) -O6 -Wall
CPP			= cl -nologo
CPPFLAGS	= -G6 -GX -GR -MD -Ox -I $(WFMOD) -I jpeg

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

%.obj : %.cpp
	$(CPP) $(CPPFLAGS) -c $<

cstrike:	$(O)
	$(CXX) -o $@ $(O)  OpenGL.o $(LIBS)

macstrike:	Labyrinthe.o Chasseur.o
	$(CXX) -o $@ OpenGL.o Labyrinthe.o Chasseur.o $(MLIBS) 
	install_name_tool -change ./libfmod.dylib $(FMOD)/libfmod.dylib $@

cstrike.exe: Labyrinthe.obj Chasseur.obj
	link -nologo -out:$@ Labyrinthe.obj Chasseur.obj $(WLIBS) -defaultlib:OpenGL.lib

clean:
	@rm -f cstrike $(O) macstrike cstrike.exe $(OBJ)

proto:
	@tar zcvf LabyrintheLinuxProto.tgz \
		Labyrinthe.cpp Labyrinthe.h Chasseur.h Chasseur.cpp Gardien.h Environnement.h \
		Mover.h FireBall.h Sound.h OpenGL32.o OpenGL.o OpenGL.lib sons modeles textures fmod \
		jpeg Makefile

Labyrinthe.o:	Labyrinthe.h Chasseur.h Gardien.h
Chasseur.o: Chasseur.cpp Chasseur.h
