CC = g++ -Wall
# Hopefully this works on Mac OS X -- I'll test it at some point :P
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
        CFLAGS = -g -dosx -Iinclude
        LDFLAGS = -L"/system/library/frameworks/opengl.framework/libraries" \
        	-lm -lstdc++ -L./lib/mac -lfreeimage
else
        CFLAGS = -g -Iinclude
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




