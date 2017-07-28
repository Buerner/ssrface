
CC = g++
CFLAGS = -Wall -std=c++11 -O3

OS := $(shell uname)

ifeq ($(OS),Darwin)
    CFLAGS +=  -mmacosx-version-min=10.10
endif

SEARCH_PATHS = -I./include

OBJ_DIR = build/.obj/
BUILD_DIR = build/
DOC_DIR = doc/

LIB_NAME = libssrface.a

SRC := $(wildcard src/*.cpp)
SRC := $(filter-out src/main.cpp, $(SRC))
OBJ = $(addprefix $(OBJ_DIR), $(notdir $(SRC:.cpp=.o) ) )


tester: lib
	$(CC) $(CFLAGS) $(SEARCH_PATHS) tester/main.cpp -Lbuild/ -lssrface -o $(BUILD_DIR)ssrface_tester

.PHONY: lib
lib: mk_build_dir $(OBJ)
	bash -c "ar -rvs $(BUILD_DIR)$(LIB_NAME) $(OBJ)"

# Generic rule to create .o files from .cpp files
$(OBJ_DIR)%.o: src/%.cpp
	$(CC) $(CFLAGS) $(SEARCH_PATHS) -c $< -o $@

.PHONY: mk_build_dir
mk_build_dir:
	mkdir -p $(OBJ_DIR)

.PHONY: doc
doc:
	doxygen

.PHONY: clean
clean:
	rm -rf $(OBJ) $(BUILD_DIR)*
	rm -rf $(DOC_DIR)


