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
include example/chat/CMakeFiles/chat_sample.dir/depend.make

# Include the progress variables for this target.
include example/chat/CMakeFiles/chat_sample.dir/progress.make

# Include the compile flags for this target's objects.
include example/chat/CMakeFiles/chat_sample.dir/flags.make

example/chat/CMakeFiles/chat_sample.dir/sample.cpp.o: example/chat/CMakeFiles/chat_sample.dir/flags.make
example/chat/CMakeFiles/chat_sample.dir/sample.cpp.o: ../example/chat/sample.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/liyb/asio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/chat/CMakeFiles/chat_sample.dir/sample.cpp.o"
	cd /home/liyb/asio/build/example/chat && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat_sample.dir/sample.cpp.o -c /home/liyb/asio/example/chat/sample.cpp

example/chat/CMakeFiles/chat_sample.dir/sample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat_sample.dir/sample.cpp.i"
	cd /home/liyb/asio/build/example/chat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/liyb/asio/example/chat/sample.cpp > CMakeFiles/chat_sample.dir/sample.cpp.i

example/chat/CMakeFiles/chat_sample.dir/sample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat_sample.dir/sample.cpp.s"
	cd /home/liyb/asio/build/example/chat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/liyb/asio/example/chat/sample.cpp -o CMakeFiles/chat_sample.dir/sample.cpp.s

# Object files for target chat_sample
chat_sample_OBJECTS = \
"CMakeFiles/chat_sample.dir/sample.cpp.o"

# External object files for target chat_sample
chat_sample_EXTERNAL_OBJECTS =

bin/examples/chat_sample: example/chat/CMakeFiles/chat_sample.dir/sample.cpp.o
bin/examples/chat_sample: example/chat/CMakeFiles/chat_sample.dir/build.make
bin/examples/chat_sample: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
bin/examples/chat_sample: example/chat/CMakeFiles/chat_sample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/liyb/asio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/examples/chat_sample"
	cd /home/liyb/asio/build/example/chat && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chat_sample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/chat/CMakeFiles/chat_sample.dir/build: bin/examples/chat_sample

.PHONY : example/chat/CMakeFiles/chat_sample.dir/build

example/chat/CMakeFiles/chat_sample.dir/clean:
	cd /home/liyb/asio/build/example/chat && $(CMAKE_COMMAND) -P CMakeFiles/chat_sample.dir/cmake_clean.cmake
.PHONY : example/chat/CMakeFiles/chat_sample.dir/clean

example/chat/CMakeFiles/chat_sample.dir/depend:
	cd /home/liyb/asio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liyb/asio /home/liyb/asio/example/chat /home/liyb/asio/build /home/liyb/asio/build/example/chat /home/liyb/asio/build/example/chat/CMakeFiles/chat_sample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/chat/CMakeFiles/chat_sample.dir/depend

