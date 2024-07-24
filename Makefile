#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

PROJECT_NAME := gao

WORKSPACE_NAME := $(notdir $(CURDIR))

# Defina a vers�o padr�o do C++ (ex: make CPP_VERSION=14, make CPP_VERSION=17)
CPP_VERSION ?= 20

# define the mode: Debug or Release (default: Debug) or make MODE=Release, make MODE=Debug
MODE ?= Debug

#########################################################################################

# Flags de compila��o padr�o
COMMON_FLAGS := -std=c++$(CPP_VERSION) -fopenmp -Wall -Wextra

# Flags de compila��o espec�ficas para Debug
DEBUG_FLAGS := -g -ggdb # Adicione aqui as flags espec�ficas para debug

# Flags de compila��o espec�ficas para Release
RELEASE_FLAGS :=  -O2 -DNDEBUG

# Use a vari�vel FLAGS para armazenar as flags a serem usadas
FLAGS := $(COMMON_FLAGS)

#########################################################################################

ifeq ($(MODE),Debug)
	FLAGS += $(DEBUG_FLAGS)
else
	FLAGS += $(RELEASE_FLAGS)
endif

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
# CXXFLAGS	:= -std=c++20 -Wall -Wextra #-g #ggdb   

CXXFLAGS := $(FLAGS)

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= bin

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

ifeq ($(OS),Windows_NT)
OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MODE)/$(MAIN))
MAIN	:= ${PROJECT_NAME}.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MODE)/$(MAIN))
MAIN	:= ${PROJECT_NAME}
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))

# define the C object files
OBJECTS		:= $(SOURCES:.cpp=.o)

# define the dependency output files
DEPS		:= $(OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#
OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MODE)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

	$(MD) $(OUTPUT)/Debug
	$(MD) $(OUTPUT)/Release

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	rm -f $(OUTPUTMAIN)
	rm -f $(OUTPUT)/Debug/$(MAIN)
	rm -f $(OUTPUT)/Release/$(MAIN)
	rm -f $(call FIXPATH,$(OBJECTS))
	rm -f $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!
else
	$(RM) $(OUTPUTMAIN)
	$(RM) $(OUTPUT)/Debug/$(MAIN)
	$(RM) $(OUTPUT)/Release/$(MAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!
endif	

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!
    