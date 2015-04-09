NAME=videoMP

LFLAGS=-fopenmp
CFLAGS=-Wall -O3 -fopenmp

SOURCES=Input.cpp Item.cpp ItemSwitch.cpp main.cpp Plugin.cpp PluginStack.cpp RingStack.cpp Slicer.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(SOURCES)
	g++ obj/* $(LFLAGS) -o $(NAME)

$(SOURCES):
	g++ $(CFLAGS) -c src/$@ -o obj/$(@:.cpp=.o)

clean:
	rm -rf obj/*.*
	rm -f $(NAME)
