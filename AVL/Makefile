CXX := g++
INCLUDE_PATHS := ./include/
CXXFLAGS := -c -g -Wall $(INCLUDE_PATHS:%=-I%)
TARGET := workspace/exec
CPP_FILES := $(shell find src/ -name \*.cpp)
OBJ := $(patsubst src/%.cpp,obj/%.o,$(CPP_FILES))

GEN_CPP := data_generator.cpp
GEN_TAR := generator

FILEPATH := data/datafile

obj/%.o : src/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $< -o $@

$(TARGET) : $(OBJ)
	@mkdir -p $(dir $@)
	@$(CXX) $^ -o $@

$(FILEPATH) :
	@mkdir -p $(dir $@)

run : $(TARGET)
	@$(CXX) $(GEN_CPP) -o $(GEN_TAR)
	@./$< $(FILEPATH)

debug :
	@echo $(INCLUDE_PATHS)
	@echo $(shell find $(INCLUDE_PATHS) -name \*.h)
	@echo $(CPP_FILES)
	@echo $(OBJ)
	@echo $(dir $(FILEPATH))

clean : 
	@rm -rf obj workspace

.PHONY : debug clean run