CC=g++
#CC=gcc

ifndef PREFIX
	PREFIX := /usr/local
endif


CFLAGS=-g -c -Wall -std=gnu++11 -Iinclude
#CFLAGS=-g -c -Wall -Iinclude
#LDFLAGS= -L$(PREFIX)/lib -lMagick++-7.Q16HDRI -lMagickCore-7.Q16HDRI -lMagickWand-7.Q16HDRI
#LDFLAGS= `Magick++-config --ldflags --libs`

SOURCES=  ./common/inputanalyzer.cpp ./common/qrparam.cpp ./common/qrencode.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=qrencode

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) 

clean:
	rm -f ./common/*.o $(EXECUTABLE)
