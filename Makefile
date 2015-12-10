NAME=videoMP

CXX=gcc

TLIBS=
TFLAGS=
TAU_MAKEFILE=/home/draco/sources/tau-2.25/x86_64/lib/Makefile.tau-memory-openmp

DEBUG=-O3
LFLAGS=-fopenmp
CFLAGS=-Wall -fopenmp -pipe
INCPATH=-I/usr/share/qt4/mkspecs/linux-g++-64 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I./src -I./src/vrobots  -I/usr/include/opencv2 -I/usr/include/opencv -I/usr/local/lib -I/usr/local/include
LIBS=-lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lcvblob -lQtGui -lQtCore 

SOURCES= src/Item.cpp src/PluginStack.cpp src/Input.cpp src/RingStack.cpp src/Plugin.cpp src/Slicer.cpp src/ItemSwitch.cpp src/main.cpp src/vrobots/PluginFindBlobs.cpp src/vrobots/PluginColorConversions.cpp src/vrobots/pattern.cpp src/vrobots/FrameSlicer.cpp src/vrobots/practicalsocket.cpp src/vrobots/CaptureFromFile.cpp src/vrobots/FastCapture.cpp src/vrobots/ball.cpp src/vrobots/PluginBlur.cpp src/vrobots/segmentation.cpp src/vrobots/pattern_matching.cpp src/vrobots/PluginDetectBalls.cpp src/vrobots/PluginFindSecondariesBlobs.cpp src/vrobots/colorspace.cpp src/vrobots/Frame.cpp src/vrobots/PluginMorphology.cpp src/vrobots/marker.cpp src/vrobots/team.cpp src/vrobots/PluginNetworking.cpp src/vrobots/homography.cpp src/vrobots/PluginColorSegmentation.cpp src/vrobots/PluginCalibration.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo LINKING
	$(CXX) $(OBJECTS) $(DEBUG) $(LFLAGS) $(LIBS) $(TLIBS) $(INCPATH) -o videoMP

.cpp.o:
	@echo COMPILING $<
	$(CXX) $(INCPATH) $(CFLAGS) $(TFLAGS) $(DEBUG) -c $< -o $@

debug: DEBUG=-g -pg -fno-inline -D DEBUG

debug: all

clean:
	rm -rf $(OBJECTS)
	rm -f $(NAME)

tau: TAU_OPTIONS=-optRevert -optCompInst
include $(TAU_MAKEFILE)

tau: TFLAGS=$(TAU_INCLUDE) $(TAU_DEFS)

tau: TLIBS=$(TAU_LIBS)

tau: CXX=$(TAU_CXX) $(TAU_OPTIONS)

tau: all
