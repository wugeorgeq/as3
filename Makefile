CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lm -lstdc++ -L./lib/mac -lfreeimage
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -L./lib/nix -lfreeimage
endif
	
RM = /bin/rm -f 
all: main 
ALL_OBJS = obj/main.o obj/parser.o
main: $(ALL_OBJS)
	$(CC) $(CFLAGS) -o as3 $(ALL_OBJS) $(LDFLAGS) 
MAIN_DEP = include/Parser.h
obj/main.o: src/main.cpp $(MAIN_DEP)
	$(CC) $(CFLAGS) -c src/main.cpp -o obj/main.o
obj/parser.o: src/Parser.cpp
	$(CC) $(CFLAGS) -c src/Parser.cpp -o obj/parser.o
clean: 
	$(RM) *.o as3




