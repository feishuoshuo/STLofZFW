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
CMAKE_SOURCE_DIR = /home/zhoufeiwei/Desktop/STLofZFW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhoufeiwei/Desktop/STLofZFW/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_sequence_containers.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_sequence_containers.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_sequence_containers.dir/flags.make

test/CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.o: test/CMakeFiles/test_sequence_containers.dir/flags.make
test/CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.o: ../test/test_STLContainer_unordered_set.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhoufeiwei/Desktop/STLofZFW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.o"
	cd /home/zhoufeiwei/Desktop/STLofZFW/build/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.o -c /home/zhoufeiwei/Desktop/STLofZFW/test/test_STLContainer_unordered_set.cpp

test/CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.i"
	cd /home/zhoufeiwei/Desktop/STLofZFW/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhoufeiwei/Desktop/STLofZFW/test/test_STLContainer_unordered_set.cpp > CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.i

test/CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.s"
	cd /home/zhoufeiwei/Desktop/STLofZFW/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhoufeiwei/Desktop/STLofZFW/test/test_STLContainer_unordered_set.cpp -o CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.s

# Object files for target test_sequence_containers
test_sequence_containers_OBJECTS = \
"CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.o"

# External object files for target test_sequence_containers
test_sequence_containers_EXTERNAL_OBJECTS =

test/test_sequence_containers: test/CMakeFiles/test_sequence_containers.dir/test_STLContainer_unordered_set.cpp.o
test/test_sequence_containers: test/CMakeFiles/test_sequence_containers.dir/build.make
test/test_sequence_containers: /usr/local/lib/libgtest_main.a
test/test_sequence_containers: /usr/local/lib/libgtest.a
test/test_sequence_containers: test/CMakeFiles/test_sequence_containers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhoufeiwei/Desktop/STLofZFW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_sequence_containers"
	cd /home/zhoufeiwei/Desktop/STLofZFW/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_sequence_containers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_sequence_containers.dir/build: test/test_sequence_containers

.PHONY : test/CMakeFiles/test_sequence_containers.dir/build

test/CMakeFiles/test_sequence_containers.dir/clean:
	cd /home/zhoufeiwei/Desktop/STLofZFW/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_sequence_containers.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_sequence_containers.dir/clean

test/CMakeFiles/test_sequence_containers.dir/depend:
	cd /home/zhoufeiwei/Desktop/STLofZFW/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhoufeiwei/Desktop/STLofZFW /home/zhoufeiwei/Desktop/STLofZFW/test /home/zhoufeiwei/Desktop/STLofZFW/build /home/zhoufeiwei/Desktop/STLofZFW/build/test /home/zhoufeiwei/Desktop/STLofZFW/build/test/CMakeFiles/test_sequence_containers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_sequence_containers.dir/depend

