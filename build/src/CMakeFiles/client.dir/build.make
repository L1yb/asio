# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/liyb/asio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/liyb/asio/build

# Include any dependencies generated for this target.
include src/CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/client.dir/flags.make

src/CMakeFiles/client.dir/client.cpp.o: src/CMakeFiles/client.dir/flags.make
src/CMakeFiles/client.dir/client.cpp.o: ../src/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/liyb/asio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/client.dir/client.cpp.o"
	cd /home/liyb/asio/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/client.cpp.o -c /home/liyb/asio/src/client.cpp

src/CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	cd /home/liyb/asio/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/liyb/asio/src/client.cpp > CMakeFiles/client.dir/client.cpp.i

src/CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	cd /home/liyb/asio/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/liyb/asio/src/client.cpp -o CMakeFiles/client.dir/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

bin/client: src/CMakeFiles/client.dir/client.cpp.o
bin/client: src/CMakeFiles/client.dir/build.make
bin/client: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
bin/client: src/CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/liyb/asio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/client"
	cd /home/liyb/asio/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/client.dir/build: bin/client

.PHONY : src/CMakeFiles/client.dir/build

src/CMakeFiles/client.dir/clean:
	cd /home/liyb/asio/build/src && $(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/client.dir/clean

src/CMakeFiles/client.dir/depend:
	cd /home/liyb/asio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liyb/asio /home/liyb/asio/src /home/liyb/asio/build /home/liyb/asio/build/src /home/liyb/asio/build/src/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/client.dir/depend

