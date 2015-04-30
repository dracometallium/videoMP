NAME=videoMP

LFLAGS=-fopenmp
CFLAGS=-Wall -g -fopenmp -pipe
#CFLAGS=-Wall -O3 -fopenmp -pipe
INCPATH=-I/usr/share/qt4/mkspecs/linux-g++-64 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I./src -I./src/vrobots  -I/usr/include/opencv2 -I/usr/include/opencv -I/usr/local/lib -I/usr/local/include
LIBS=-lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lcvblob -lQtGui -lQtCore 
DEBUG=

SOURCES=Input.o Item.o ItemSwitch.o main.o Plugin.o PluginStack.o RingStack.o Slicer.o

SVROBOTS=ball.o CaptureFromFile.o colorspace.o Frame.o FrameSlicer.o homography.o marker.o pattern.o pattern_matching.o PluginBlur.o PluginCalibration.o PluginColorConversions.o PluginColorSegmentation.o PluginDetectBalls.o PluginFindSecondariesBlobs.o PluginFindBlobs.o PluginMorphology.o PluginNetworking.o practicalsocket.o segmentation.o team.o

all: $(NAME)

$(NAME): $(SOURCES) $(SVROBOTS)
	g++ *.o $(DEBUG) $(LFLAGS) $(LIBS) $(INCPATH) -o $(NAME)

$(SOURCES):
	g++  $(INCPATH) $(CFLAGS) $(DEBUG) -c src/$(@:.o=.cpp) -o $@

$(SVROBOTS):
	g++  $(INCPATH) $(CFLAGS) $(DEBUG) -c src/vrobots/$(@:.o=.cpp) -o $@

debug: DEBUG=-D DEBUG

debug: all

clean:
	rm -rf *.o
	rm -f $(NAME)
