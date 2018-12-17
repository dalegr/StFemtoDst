CC=g++
CFLAGS=-I.. -I$(shell root-config --cflags) -O0 -D_VANILLA_ROOT_ -g -lEG
LIBS=$(shell root-config --libs)
STFEMTO=libStFemto.so
SOURCES_CXX=$(shell find . -name "*.cxx")

all: $(STFEMTO)

$(STFEMTO): $(SOURCES_CXX:.cxx=.o) StFemtoDict.cxx
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $(STFEMTO) $(LIBS)

%.o: %.cxx
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

StFemtoDict.cxx: $(shell find . -name "*.h" ! -name "*LinkDef*")
	rootcint -f $@ -c $(CFLAGS) -D__ROOT__ -p $^ StFemtoDstLinkDef.h
	sed -i 's;.*StHbtEventReader.*ShowMembers.*;;g' StFemtoDict.cxx
clean:
	rm -vf *.o StFemtoDict.* $(STFEMTO)
