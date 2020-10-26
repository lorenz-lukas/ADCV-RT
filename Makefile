CXX=g++
CXXFLAGS=-std=c++11
BUILD_DIR=obj

### ADCV

ADCV_INCLUDE=adcv4
COMMON_INCLUDE=Libs/Common
JSON_INCLUDE=Libs/JSON/Cpp
#SEMAPHORES_INCLUDE=Libs/Semaphores
NETWORKING_INCLUDE=networking/tcp

STATIC_DEPS_adcv=$(BUILD_DIR)/adcv.o $(BUILD_DIR)/connection.o $(BUILD_DIR)/server.o $(BUILD_DIR)/jsonLogging.o #$(BUILD_DIR)/semaphores.o

DYNAMIC_DEPS=-lopencv_highgui -lopencv_videoio -lopencv_core -lopencv_calib3d -lopencv_imgproc -lopencv_imgcodecs -lopencv_aruco -pthread -lrt

### FREERTOS

f_SOURCE=Source
f_INCLUDE=Source/include
f_PORTABLE_MEM=Source/portable/MemMang
f_PORTABLE_POSIX=Source/portable/GCC/POSIX

STATIC_DEPS_freeRTOS=$(BUILD_DIR)/croutine.o $(BUILD_DIR)/event_groups.o $(BUILD_DIR)/list.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/tasks.o $(BUILD_DIR)/timers.o $(BUILD_DIR)/heap_3.o $(BUILD_DIR)/port.o
f_LINUX= /usr/include/x86_64-linux-gnu/

### SOURCE

SRC=Project

.PHONY: default buildDirectory clean main

default: buildDirectory main	

buildDirectory:
	@mkdir -p $(BUILD_DIR)	

#############   ADCV
$(BUILD_DIR)/adcv.o: $(ADCV_INCLUDE)/adcv.cpp
	$(CXX) $(CXXFLAGS) -I$(ADCV_INCLUDE) -I$(COMMON_INCLUDE) -c -o $@ $^ 
	#-I$(SEMAPHORES_INCLUDE)

$(BUILD_DIR)/connection.o: $(NETWORKING_INCLUDE)/connection.cpp
	$(CXX) $(CXXFLAGS) -I$(NETWORKING_INCLUDE) -I$(ADCV_INCLUDE) -c -o $@ $^

$(BUILD_DIR)/server.o: $(NETWORKING_INCLUDE)/server.cpp
	$(CXX) $(CXXFLAGS) -I$(NETWORKING_INCLUDE) -I$(ADCV_INCLUDE) -c -o $@ $^

# $(BUILD_DIR)/semaphores.o: $(SEMAPHORES_INCLUDE)/semaphores.cpp
	# $(CXX) $(CXXFLAGS) -I$(SEMAPHORES_INCLUDE) -I$(COMMON_INCLUDE) -c -o $@ $^

$(BUILD_DIR)/jsonLogging.o: $(JSON_INCLUDE)/jsonLogging.cpp
	$(CXX) $(CXXFLAGS) -I$(JSON_INCLUDE) -I$(COMMON_INCLUDE) -c -o $@ $^

############# FREERTOS
$(BUILD_DIR)/croutine.o: $(f_SOURCE)/croutine.c 
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/event_groups.o: $(f_SOURCE)/event_groups.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/list.o: $(f_SOURCE)/list.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/queue.o: $(f_SOURCE)/queue.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/tasks.o: $(f_SOURCE)/tasks.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/timers.o: $(f_SOURCE)/timers.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/heap_3.o: $(f_PORTABLE_MEM)/heap_3.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_PORTABLE_MEM) -I$(f_PORTABLE_POSIX) -c -o $@ $^

$(BUILD_DIR)/port.o: $(f_PORTABLE_POSIX)/port.c
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(f_INCLUDE) -I$(f_PORTABLE_POSIX) -c -o $@ $^

main: $(SRC)/main.cpp $(STATIC_DEPS_adcv) $(STATIC_DEPS_freeRTOS)
	$(CXX) $(CXXFLAGS) -I. -I$(SRC) -I$(ADCV_INCLUDE) -I$(NETWORKING_INCLUDE) -I$(JSON_INCLUDE) -I$(COMMON_INCLUDE) -I$(f_INCLUDE) -I$(f_SOURCE) -I$(f_PORTABLE_MEM) -I$(f_PORTABLE_POSIX) -o $@ $< $(STATIC_DEPS_adcv) $(STATIC_DEPS_freeRTOS) $(DYNAMIC_DEPS)
#-I$(SEMAPHORES_INCLUDE)

clean:
	@find . -maxdepth 1 -executable ! -type d -exec rm -fv '{}' \;
	@rm -Rfv $(BUILD_DIR)
