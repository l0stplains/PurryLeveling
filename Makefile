SHELL := /bin/bash

# ──────────────────────────────────────────────────────────────────────────────
#  Makefile for PurryLeveling
# ──────────────────────────────────────────────────────────────────────────────

CXX := g++
CXXFLAGS_COMMON := -std=c++17 -Wall -Wextra -Wpedantic \
	-Iinclude \
	-I/usr/local/include \
	$(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)
CXXFLAGS_DEBUG := -g -DDEBUG -O0
CXXFLAGS_RELEASE := -O3 -DNDEBUG

# default release build, use DEBUG=1 for debug build
ifeq ($(DEBUG),1)
    CXXFLAGS := $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG)
    BUILD_TYPE := debug
else
    CXXFLAGS := $(CXXFLAGS_COMMON) $(CXXFLAGS_RELEASE)
    BUILD_TYPE := release
endif

# linker: look in /usr/local/lib for ImGui-SFML
LDFLAGS := -L/usr/local/lib -Wl,-rpath=/usr/local/lib \
  $(shell pkg-config --libs sfml-graphics sfml-window sfml-system) \
  -lImGui-SFML

SRCDIR := src
DRVDIR := $(SRCDIR)/drivers
INCDIR := include
BUILDDIR := build/$(BUILD_TYPE)
BINDIR := bin

TARGET := $(BINDIR)/PurryLeveling
ENTRY ?= main

ifeq ($(ENTRY),main)
    MAIN_SRC := $(SRCDIR)/main.cpp
else
    MAIN_SRC := $(DRVDIR)/$(ENTRY).cpp
    ifeq ($(wildcard $(MAIN_SRC)),)
        $(error [ERROR] Entry file '$(MAIN_SRC)' not found.)
    endif
endif

# Sources and objects
LIB_SRCS := $(shell find $(SRCDIR) -type f -name '*.cpp' ! -path '$(DRVDIR)/*' ! -name 'main.cpp')
SRCS := $(MAIN_SRC) $(LIB_SRCS)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

HEADERS := $(shell find $(INCDIR) -type f -name '*.hpp')

COLOR_INFO := \033[1;34m
COLOR_SUCCESS := \033[1;32m
COLOR_WARN := \033[1;33m
COLOR_ERROR := \033[1;31m
COLOR_RESET := \033[0m

.PHONY: all release debug run clean distclean format install help

all: $(TARGET)

release:
	@$(MAKE) DEBUG=0

debug:
	@$(MAKE) DEBUG=1

$(TARGET): $(OBJS)
	@echo -e "$(COLOR_INFO)[INFO]$(COLOR_RESET) Linking $@..."
	@mkdir -p $(BINDIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo -e "$(COLOR_SUCCESS)[SUCCESS]$(COLOR_RESET) Built $@ ($(BUILD_TYPE)) successfully."

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@echo -e "$(COLOR_INFO)[INFO]$(COLOR_RESET) Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@echo -e "$(COLOR_INFO)[INFO]$(COLOR_RESET) Running $(TARGET)..."
	@$(TARGET)

clean:
	@echo -e "$(COLOR_WARN)[INFO]$(COLOR_RESET) Cleaning $(BUILD_TYPE) build..."
	@rm -rf $(BUILDDIR) $(TARGET)
	@echo -e "$(COLOR_SUCCESS)[SUCCESS]$(COLOR_RESET) Clean complete."

distclean:
	@echo -e "$(COLOR_WARN)[INFO]$(COLOR_RESET) Removing all build artifacts..."
	@rm -rf build bin
	@echo -e "$(COLOR_SUCCESS)[SUCCESS]$(COLOR_RESET) Clean complete."

format:
	@echo -e "$(COLOR_INFO)[INFO]$(COLOR_RESET) Formatting all source files with clang-format..."
	@find src include \( -name '*.cpp' -o -name '*.hpp' \) \
	    -exec clang-format -i {} \;
	@echo -e "$(COLOR_SUCCESS)[SUCCESS]$(COLOR_RESET) Code formatted."

help:
	@echo ""
	@echo -e "$(COLOR_INFO)PurryLeveling Makefile$(COLOR_RESET)"
	@echo "────────────────────────────────────"
	@echo " make               → build release with src/main.cpp"
	@echo " make debug         → build debug version"
	@echo " make release       → build release version (default)"
	@echo " make ENTRY=foo     → build using src/drivers/foo.cpp"
	@echo " make run           → build then run (pass ENTRY=foo if needed)"
	@echo " make clean         → remove current build artifacts"
	@echo " make distclean     → remove all build artifacts"
	@echo " make format        → clang-format src/ and include/"
	@echo " make help          → this message"
	@echo ""
	@echo "Examples:"
	@echo " make DEBUG=1 ENTRY=testUI  → debug build using drivers/testUI.cpp"
	@echo " make run ENTRY=benchmark   → build and run drivers/benchmark.cpp"
	@echo ""

