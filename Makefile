NAME=videoMP

LFLAGS=-fopenmp
CFLAGS=-Wall -O3 -fopenmp
DEBUG=

SOURCES=Input.cpp Item.cpp ItemSwitch.cpp main.cpp Plugin.cpp PluginStack.cpp RingStack.cpp Slicer.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(SOURCES)
	g++ obj/* $(DEBUG) $(LFLAGS) -o $(NAME)

$(SOURCES):
	g++ $(CFLAGS) $(DEBUG) -c src/$@ -o obj/$(@:.cpp=.o)

debug: DEBUG=-D DEBUG

debug: all

clean:
	rm -rf obj/*.*
	rm -f $(NAME)
