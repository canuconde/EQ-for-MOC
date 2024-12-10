TARGET_EXEC := eq4moc
BUILD_DIR := ./build
SRC_DIRS := ./src
INCLUDE_DIR  := ./include
MAN_DIR := ./man

CXX = g++
# CXXFLAGS = -g -std=c++17 -Wall -W -Werror -pedantic
CXXFLAGS = -O2 -std=c++17
INCLUDE = -Iinclude -I/usr/local/include -I/usr/include -I $(INCLUDE_DIR)
LDFLAGS = "-lncurses"
#LDLIBS = "-lncurses"

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS := $(filter-out $(SRC_DIRS)/test.cpp, $(SRCS))

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

install:
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 655 $(BUILD_DIR)/$(TARGET_EXEC) $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)$(PREFIX)/share/man/man1/
	install -m 644 $(MAN_DIR)/eq4moc.1 $(DESTDIR)$(PREFIX)/share/man/man1/



.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
