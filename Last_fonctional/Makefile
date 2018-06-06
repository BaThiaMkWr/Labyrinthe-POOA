.SUFFIXES:
.SUFFIXES:      .cpp .o .h

FMOD		= fmod/4.3
WFMOD		= fmod/Win-4.3

O			= Labyrinthe.o Chasseur.o
OBJ			= Labyrinthe.obj Chasseur.obj


LIBS		= $(FMOD)/libfmodex64-4.32.04.so -lglut -lGLU -lGL -ljpeg -lm
WLIBS		= -defaultlib:glut32 -defaultlib:OPENGL32 -defaultlib:GLU32 jpeg/libjpeg.lib $(WFMOD)/fmodex_vc.lib
MLIBS		= -framework GLUT -framework OpenGL -framework Foundation jpeg/libjpeg.a /Developer/FMOD\ Programmers\ API\ Mac/api/lib/libfmodex.dylib

CXX		= g++ -g
CXXFLAGS	= -I $(FMOD) -O6 -Wall
CPP			= cl -nologo
CPPFLAGS	= -G6 -GX -GR -MD -Ox -I $(WFMOD) -I jpeg

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

%.obj : %.cpp
	$(CPP) $(CPPFLAGS) -c $<

cstrike:	$(O)
	$(CXX) -o $@ $(O)  OpenGL64.o $(LIBS) 

macstrike:	Labyrinthe.o Chasseur.o
	$(CXX) -o $@ OpenGL.o Labyrinthe.o Chasseur.o $(MLIBS)
	install_name_tool -change ./libfmodex.dylib /Developer/FMOD\ Programmers\ API\ Mac/api/lib/libfmodex.dylib $@

cstrike.exe: Labyrinthe.obj Chasseur.obj
	link -nologo -out:$@ Labyrinthe.obj Chasseur.obj $(WLIBS) -defaultlib:OpenGL.lib

clean:
	@rm -f cstrike $(O) macstrike cstrike.exe $(OBJ)

proto:
	@tar zcvf LabyrintheLinuxProto.tgz \
		Labyrinthe.cpp Labyrinthe.h Chasseur.h Chasseur.cpp Gardien.h Environnement.h \
		Mover.h FireBall.h Sound.h OpenGL64.o OpenGL.o OpenGL.lib sons modeles textures fmod \
		jpeg Makefile

Labyrinthe.o:	Labyrinthe.h Chasseur.h Gardien.h
Chasseur.o: Chasseur.cpp Chasseur.h
