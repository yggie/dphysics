.PHONY : clean all

INCLUDE_PATH=-Iinclude
BIN_PATH=bin
SRC_PATH=src
CC=g++
CFLAGS=-std=c++11 -Wall -Wextra $(INCLUDE_PATH)
LIBS=-lGL -lglut
SOURCES=$(shell find $(SRC_PATH)/ -type f -name '*.cpp')
OBJECTS=$(patsubst $(SRC_PATH)/%.cpp, %.o, $(SOURCES))
DEPENDENCIES=$(OBJECTS:.o=.d)
EXECUTABLE=demo

vpath %.d $(BIN_PATH)/
vpath %.o $(BIN_PATH)/

# default rule to execute
all : $(DEPENDENCIES) $(EXECUTABLE)

build :
	@mkdir -p $(BIN_PATH)

# creates a list of dependencies in a separate file (.depend) for the source files
%.d : $(SRC_PATH)/%.cpp | build
	@rm -f $(BIN_PATH)/$@
	$(CC) $(CFLAGS) -MM $< -MF $(BIN_PATH)/$@

# includes the list of dependencies
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

