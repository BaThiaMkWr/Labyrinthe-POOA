O =		jcomapi.o jdapimin.o jdapistd.o jdatasrc.o jdcoefct.o jdcolor.o \
		jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o jdmaster.o \
		jdmerge.o jdphuff.o jdpostct.o jdsample.o jdtrans.o jerror.o \
		jidctflt.o jidctfst.o jidctint.o jidctred.o jmemmgr.o jmemnobs.o \
		jquant1.o jquant2.o jutils.o

OBJ =	jcomapi.obj jdapimin.obj jdapistd.obj jdatasrc.obj jdcoefct.obj jdcolor.obj \
		jddctmgr.obj jdhuff.obj jdinput.obj jdmainct.obj jdmarker.obj jdmaster.obj \
		jdmerge.obj jdphuff.obj jdpostct.obj jdsample.obj jdtrans.obj jerror.obj \
		jidctflt.obj jidctfst.obj jidctint.obj jidctred.obj jmemmgr.obj jmemnobs.obj \
		jquant1.obj jquant2.obj jutils.obj

CC		=	gcc
CFLAGS	=	-O6
CPP			= cl -nologo
CPPFLAGS	= -G6 -GX -GR -MD -Ox

%.obj : %.c
	$(CPP) $(CPPFLAGS) -c $<

%.o : %.c
	$(CC) $(CFLAGS) -c $<

libjpeg.a:	$(O)
	ar cr $@ $(O)

libjpeg.lib:	$(OBJ)
	lib -nologo -out:$@ $(OBJ)

clean:
	rm -f $(OBJ) libjpeg.lib $(O) libjpeg.a
