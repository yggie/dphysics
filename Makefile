.PHONY : clean all

INCLUDE_PATH=-Iinclude
BIN_PATH=bin
SRC_PATH=src
CC=g++
CFLAGS=-std=c++11 -Wall -Wextra $(INCLUDE_PATH)
LIBS=-lGL -lglut
SOURCES=$(shell find $(SRC_PATH)/ -type f -name '*.cpp')
OBJECTS=$(patsubst $(SRC_PATH)/%.cpp, %.o, $(SOURCES))
EXECUTABLE=demo

vpath %.o $(BIN_PATH)/

# default rule to execute
all : build .depend $(EXECUTABLE)

build :
	@mkdir -p $(BIN_PATH)

# creates a list of dependencies in a separate file (.depend) for the source files
.depend : $(SOURCES)
	@rm -f .depend
	$(CC) $(CFLAGS) -MM $^ -MF .depend

# includes the list of dependencies
-include .depend

# creates the executable to run
$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(addprefix $(BIN_PATH)/, $(OBJECTS)) $(LIBS) -o $@

%.o : $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $(BIN_PATH)/$@

clean :
	rm -rf $(EXECUTABLE) bin
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;

