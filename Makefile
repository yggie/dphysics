# compiler options
CC        := g++
BIN_BASE  := bin
SRC_BASE  := src
INC_FLAGS := -Iinclude -Ilibs/glm
DFLAGS    := -g -DNDEBUG
CFLAGS    := -std=c++11 -Wall -Wextra $(INC_FLAGS) $(DFLAGS)
LIBS      := -lGL -lGLU -lglut

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#   REACT LIBRARY (not yet a real library)
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
REACT_MODULES     := memory
REACT_BASE_PATH   := $(SRC_BASE)/react
REACT_PATH        := $(addprefix $(REACT_BASE_PATH)/, $(REACT_MODULES)) $(REACT_BASE_PATH)
REACT_SOURCES     := $(foreach dir, $(REACT_PATH), $(wildcard $(dir)/*.cpp))
REACT_OBJECTS     := $(patsubst $(SRC_BASE)/%.cpp, $(BIN_BASE)/%.o, $(REACT_SOURCES))

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#   REACT DEMO APPLICATION
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
DEMO_PATH         := $(SRC_BASE)/demo
DEMO_SOURCES      := $(wildcard $(DEMO_PATH)/*.cpp)
DEMO_OBJECTS      := $(patsubst $(SRC_BASE)/%.cpp, $(BIN_BASE)/%.o, $(DEMO_SOURCES))
DEMO_LIBS         := -lGL -lGLU -lglut
DEMO_EXECUTABLE   := demo

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#   RAY TRACING APPLICATION
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # ##
RAY_PATH          := $(SRC_BASE)/ray
RAY_SOURCES       := $(wildcard $(RAY_PATH)/*.cpp)
RAY_OBJECTS       := $(patsubst $(SRC_BASE)/%.cpp, $(BIN_BASE)/%.o, $(RAY_SOURCES))
RAY_LIBS          := -lGL -lglut
RAY_EXECUTABLE    := ray

ALL_SOURCE_PATHS  := $(REACT_PATH) $(DEMO_PATH) $(RAY_PATH)
BIN_PATH          := $(patsubst $(SRC_BASE)/%, $(BIN_BASE)/%, $(ALL_SOURCE_PATHS))

define make-goals
$(1)/%.o : %.cpp | $(BIN_PATH)
	$(CC) $(CFLAGS) -c $$< -o $$@
	$(CC) $(CFLAGS) -MM $$< -MF $(1)/$$*.d -MT $$@
endef
$(foreach dir, $(BIN_PATH), $(eval $(call make-goals, $(dir))))

# phony rules
.PHONY : clean all
# file search paths
vpath %.cpp $(ALL_SOURCE_PATHS)

# default rule to execute
all : $(DEMO_EXECUTABLE) $(RAY_EXECUTABLE)

# creates the bin path if necessary
$(BIN_PATH) :
	@mkdir -p $(BIN_PATH)

# creates the executable to run
$(DEMO_EXECUTABLE) : $(REACT_OBJECTS) $(DEMO_OBJECTS)
	$(CC) $^ $(DEMO_LIBS) -o $@

$(RAY_EXECUTABLE) : $(REACT_OBJECTS) $(RAY_OBJECTS)
	$(CC) $^ $(RAY_LIBS) -o $@

# includes the list of dependencies which are already made
-include $(foreach dir, $(BIN_PATH), $(wildcard *.d))

#debug : echo_files all

#echo_files :
#	@echo OBJS $(OBJECTS)
#	@echo SRC  $(SOURCES)

clean :
	@rm -rf $(DEMO_EXECUTABLE) $(RAY_EXECUTABLE) $(BIN_BASE)
	@echo removed $(BIN_PATH)/
	@find . -name "*~" -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*.d" -exec rm {} \;
	@echo removed *.o, *.d, *\~ files

