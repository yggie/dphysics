# paths
MODULES   := memory demo
BIN_BASE  := bin
SRC_BASE  := src
BIN_PATH  := $(addprefix $(BIN_BASE)/, $(MODULES)) $(BIN_BASE)
SRC_PATH  := $(addprefix $(SRC_BASE)/, $(MODULES)) $(SRC_BASE)

# compiler options
CC        := g++
INC_FLAGS := -Iinclude
DFLAGS    := -g -DDEBUG
CFLAGS    := -std=c++11 -Wall -Wextra $(INC_FLAGS) $(DFLAGS)
LIBS      := -lGL -lglut

# project files
SOURCES       := $(foreach dir, $(SRC_PATH), $(wildcard $(dir)/*.cpp))
OBJECTS       := $(patsubst $(SRC_BASE)/%.cpp, $(BIN_BASE)/%.o, $(SOURCES))
EXECUTABLE    := demotest

# phony rules
.PHONY : clean all debug

# file search paths
vpath %.cpp $(SRC_PATH)

# default rule to execute
all : $(EXECUTABLE)

# checks if the bin directory exists
$(BIN_PATH) :
	@mkdir -p $(BIN_PATH)

# creates the executable to run
$(EXECUTABLE) : $(OBJECTS)
	$(CC) $^ $(LIBS) -o $@

# includes the list of dependencies which are already made
-include $(OBJECTS:.o=.d)

define make-goals
$(1)/%.o : %.cpp | $(BIN_PATH)
	$(CC) $(CFLAGS) -c $$< -o $$@
	$(CC) $(CFLAGS) -MM $$< -MF $(1)/$$*.d -MT $$@
endef

# creates rules to compile files and make dependency lists
$(foreach dir, $(BIN_PATH), $(eval $(call make-goals, $(dir))))

#debug : echo_files all

#echo_files :
#	@echo OBJS $(OBJECTS)
#	@echo SRC  $(SOURCES)

clean :
	@rm -rf $(EXECUTABLE) $(BIN_BASE)
	@echo removed $(BIN_PATH)/
	@find . -name "*~" -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*.d" -exec rm {} \;
	@echo removed *.o, *.d, *\~ files

