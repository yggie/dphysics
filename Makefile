.PHONY : clean all

# paths
BIN_PATH=bin
SRC_PATH=src

# compiler choice
CC=g++
# compiler include flags
INC_FLAGS=-Iinclude
# compiler debug flags
DFLAGS=-g -DDEBUG
# compiler flags
CFLAGS=-std=c++11 -Wall -Wextra $(INC_FLAGS) $(DFLAGS)
# library options
LIBS=-lGL -lglut

# project files
SOURCES=$(shell find $(SRC_PATH)/ -type f -name '*.cpp')
OBJECTS=$(patsubst $(SRC_PATH)/%.cpp, %.o, $(SOURCES))
DEPENDENCIES=$(OBJECTS:.o=.d)
EXECUTABLE=demo

# file search paths
vpath %.d $(BIN_PATH)/
vpath %.o $(BIN_PATH)/

# default rule to execute
all : $(DEPENDENCIES) $(EXECUTABLE)

debug :
	@echo depn $(DEPENDENCIES)
	@echo objs $(OBJECTS)
	@echo src  $(SOURCES)

build :
	@mkdir -p $(BIN_PATH)

# creates a list of dependencies in a separate file (.depend) for the source files
%.d : $(SRC_PATH)/%.cpp | build
	@rm -f $(BIN_PATH)/$@
	$(CC) $(CFLAGS) -MM $< -MF $(BIN_PATH)/$@

# includes the list of dependencies which are already made
#-include $(shell find $(BIN_PATH)/ -type f -name '*.d')
-include $(addprefix $(BIN_PATH)/, $(DEPENDENCIES))

# creates the executable to run
$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(addprefix $(BIN_PATH)/, $(OBJECTS)) $(LIBS) -o $@

%.o : $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $(BIN_PATH)/$@

clean :
	@rm -rf $(EXECUTABLE) $(BIN_PATH)
	@echo removed $(BIN_PATH)/
	@find . -name "*~" -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*.d" -exec rm {} \;
	@echo removed *.o, *.d, *\~ files

