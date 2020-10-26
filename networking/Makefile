CC=gcc
# CC=clang
CXX=g++
# CXX=clang

CCFLAGS=-Wall -Werror -pedantic
CXXFLAGS=-std=c++11 -Wall -Werror -pedantic
INCLUDE_DIR=tcp
BUILD_DIR=build

.PHONY: default buildDirectory clean cObjects cppObjects allObjects

default: buildDirectory ExampleTCPServer ExampleTCPClient

buildDirectory:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/connection.c.o: $(INCLUDE_DIR)/connection.c
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c -o $@ $^

$(BUILD_DIR)/connection.cpp.o: $(INCLUDE_DIR)/connection.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $^

$(BUILD_DIR)/server.c.o: $(INCLUDE_DIR)/server.c
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c -o $@ $^

$(BUILD_DIR)/server.cpp.o: $(INCLUDE_DIR)/server.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $^

$(BUILD_DIR)/client.c.o: $(INCLUDE_DIR)/client.c
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c -o $@ $^

$(BUILD_DIR)/client.cpp.o: $(INCLUDE_DIR)/client.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $^

ExampleTCPServer: ExampleTCPServer.cpp $(BUILD_DIR)/server.cpp.o $(BUILD_DIR)/connection.cpp.o
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

ExampleTCPClient: ExampleTCPClient.cpp $(BUILD_DIR)/client.cpp.o $(BUILD_DIR)/connection.cpp.o
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

cObjects: buildDirectory $(BUILD_DIR)/connection.c.o $(BUILD_DIR)/server.c.o $(BUILD_DIR)/client.c.o

cppObjects: buildDirectory $(BUILD_DIR)/connection.cpp.o $(BUILD_DIR)/server.cpp.o $(BUILD_DIR)/client.cpp.o

allObjects: cObjects cppObjects

clean:
	@find . -executable ! -type d -exec rm -f '{}' \;
	@find . -name "*.o" -exec rm -f '{}' \;
	@find . -name "*.a" -exec rm -f '{}' \;
	@rm -Rf $(BUILD_DIR)
