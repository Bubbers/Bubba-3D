# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/johan/Programming/bubba3d/Bubba-3D

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/johan/Programming/bubba3d/Bubba-3D/build

# Include any dependencies generated for this target.
include CMakeFiles/test2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test2.dir/flags.make

CMakeFiles/test2.dir/src/objects/Scene.cpp.o: CMakeFiles/test2.dir/flags.make
CMakeFiles/test2.dir/src/objects/Scene.cpp.o: ../src/objects/Scene.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/johan/Programming/bubba3d/Bubba-3D/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/test2.dir/src/objects/Scene.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test2.dir/src/objects/Scene.cpp.o -c /home/johan/Programming/bubba3d/Bubba-3D/src/objects/Scene.cpp

CMakeFiles/test2.dir/src/objects/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test2.dir/src/objects/Scene.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/johan/Programming/bubba3d/Bubba-3D/src/objects/Scene.cpp > CMakeFiles/test2.dir/src/objects/Scene.cpp.i

CMakeFiles/test2.dir/src/objects/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test2.dir/src/objects/Scene.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/johan/Programming/bubba3d/Bubba-3D/src/objects/Scene.cpp -o CMakeFiles/test2.dir/src/objects/Scene.cpp.s

CMakeFiles/test2.dir/src/objects/Scene.cpp.o.requires:
.PHONY : CMakeFiles/test2.dir/src/objects/Scene.cpp.o.requires

CMakeFiles/test2.dir/src/objects/Scene.cpp.o.provides: CMakeFiles/test2.dir/src/objects/Scene.cpp.o.requires
	$(MAKE) -f CMakeFiles/test2.dir/build.make CMakeFiles/test2.dir/src/objects/Scene.cpp.o.provides.build
.PHONY : CMakeFiles/test2.dir/src/objects/Scene.cpp.o.provides

CMakeFiles/test2.dir/src/objects/Scene.cpp.o.provides.build: CMakeFiles/test2.dir/src/objects/Scene.cpp.o

# Object files for target test2
test2_OBJECTS = \
"CMakeFiles/test2.dir/src/objects/Scene.cpp.o"

# External object files for target test2
test2_EXTERNAL_OBJECTS =

test2: CMakeFiles/test2.dir/src/objects/Scene.cpp.o
test2: CMakeFiles/test2.dir/build.make
test2: CMakeFiles/test2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable test2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test2.dir/build: test2
.PHONY : CMakeFiles/test2.dir/build

CMakeFiles/test2.dir/requires: CMakeFiles/test2.dir/src/objects/Scene.cpp.o.requires
.PHONY : CMakeFiles/test2.dir/requires

CMakeFiles/test2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test2.dir/clean

CMakeFiles/test2.dir/depend:
	cd /home/johan/Programming/bubba3d/Bubba-3D/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/johan/Programming/bubba3d/Bubba-3D /home/johan/Programming/bubba3d/Bubba-3D /home/johan/Programming/bubba3d/Bubba-3D/build /home/johan/Programming/bubba3d/Bubba-3D/build /home/johan/Programming/bubba3d/Bubba-3D/build/CMakeFiles/test2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test2.dir/depend

