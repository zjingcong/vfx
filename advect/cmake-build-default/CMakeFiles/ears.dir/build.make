# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /home/jingcoz/clion-2016.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/jingcoz/clion-2016.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jingcoz/workspace/clion_projects/advect

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jingcoz/workspace/clion_projects/advect/cmake-build-default

# Include any dependencies generated for this target.
include CMakeFiles/ears.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ears.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ears.dir/flags.make

CMakeFiles/ears.dir/ears.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/ears.C.o: ../ears.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ears.dir/ears.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/ears.C.o -c /home/jingcoz/workspace/clion_projects/advect/ears.C

CMakeFiles/ears.dir/ears.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/ears.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/ears.C > CMakeFiles/ears.dir/ears.C.i

CMakeFiles/ears.dir/ears.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/ears.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/ears.C -o CMakeFiles/ears.dir/ears.C.s

CMakeFiles/ears.dir/ears.C.o.requires:

.PHONY : CMakeFiles/ears.dir/ears.C.o.requires

CMakeFiles/ears.dir/ears.C.o.provides: CMakeFiles/ears.dir/ears.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/ears.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/ears.C.o.provides

CMakeFiles/ears.dir/ears.C.o.provides.build: CMakeFiles/ears.dir/ears.C.o


CMakeFiles/ears.dir/src/Camera.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Camera.C.o: ../src/Camera.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ears.dir/src/Camera.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Camera.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Camera.C

CMakeFiles/ears.dir/src/Camera.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Camera.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Camera.C > CMakeFiles/ears.dir/src/Camera.C.i

CMakeFiles/ears.dir/src/Camera.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Camera.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Camera.C -o CMakeFiles/ears.dir/src/Camera.C.s

CMakeFiles/ears.dir/src/Camera.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Camera.C.o.requires

CMakeFiles/ears.dir/src/Camera.C.o.provides: CMakeFiles/ears.dir/src/Camera.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Camera.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Camera.C.o.provides

CMakeFiles/ears.dir/src/Camera.C.o.provides.build: CMakeFiles/ears.dir/src/Camera.C.o


CMakeFiles/ears.dir/src/Grid.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Grid.C.o: ../src/Grid.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ears.dir/src/Grid.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Grid.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Grid.C

CMakeFiles/ears.dir/src/Grid.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Grid.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Grid.C > CMakeFiles/ears.dir/src/Grid.C.i

CMakeFiles/ears.dir/src/Grid.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Grid.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Grid.C -o CMakeFiles/ears.dir/src/Grid.C.s

CMakeFiles/ears.dir/src/Grid.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Grid.C.o.requires

CMakeFiles/ears.dir/src/Grid.C.o.provides: CMakeFiles/ears.dir/src/Grid.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Grid.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Grid.C.o.provides

CMakeFiles/ears.dir/src/Grid.C.o.provides.build: CMakeFiles/ears.dir/src/Grid.C.o


CMakeFiles/ears.dir/src/Image.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Image.C.o: ../src/Image.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ears.dir/src/Image.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Image.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Image.C

CMakeFiles/ears.dir/src/Image.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Image.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Image.C > CMakeFiles/ears.dir/src/Image.C.i

CMakeFiles/ears.dir/src/Image.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Image.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Image.C -o CMakeFiles/ears.dir/src/Image.C.s

CMakeFiles/ears.dir/src/Image.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Image.C.o.requires

CMakeFiles/ears.dir/src/Image.C.o.provides: CMakeFiles/ears.dir/src/Image.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Image.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Image.C.o.provides

CMakeFiles/ears.dir/src/Image.C.o.provides.build: CMakeFiles/ears.dir/src/Image.C.o


CMakeFiles/ears.dir/src/Levelsets.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Levelsets.C.o: ../src/Levelsets.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ears.dir/src/Levelsets.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Levelsets.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Levelsets.C

CMakeFiles/ears.dir/src/Levelsets.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Levelsets.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Levelsets.C > CMakeFiles/ears.dir/src/Levelsets.C.i

CMakeFiles/ears.dir/src/Levelsets.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Levelsets.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Levelsets.C -o CMakeFiles/ears.dir/src/Levelsets.C.s

CMakeFiles/ears.dir/src/Levelsets.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Levelsets.C.o.requires

CMakeFiles/ears.dir/src/Levelsets.C.o.provides: CMakeFiles/ears.dir/src/Levelsets.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Levelsets.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Levelsets.C.o.provides

CMakeFiles/ears.dir/src/Levelsets.C.o.provides.build: CMakeFiles/ears.dir/src/Levelsets.C.o


CMakeFiles/ears.dir/src/Lighting.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Lighting.C.o: ../src/Lighting.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ears.dir/src/Lighting.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Lighting.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Lighting.C

CMakeFiles/ears.dir/src/Lighting.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Lighting.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Lighting.C > CMakeFiles/ears.dir/src/Lighting.C.i

CMakeFiles/ears.dir/src/Lighting.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Lighting.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Lighting.C -o CMakeFiles/ears.dir/src/Lighting.C.s

CMakeFiles/ears.dir/src/Lighting.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Lighting.C.o.requires

CMakeFiles/ears.dir/src/Lighting.C.o.provides: CMakeFiles/ears.dir/src/Lighting.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Lighting.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Lighting.C.o.provides

CMakeFiles/ears.dir/src/Lighting.C.o.provides.build: CMakeFiles/ears.dir/src/Lighting.C.o


CMakeFiles/ears.dir/src/Matrix.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Matrix.C.o: ../src/Matrix.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ears.dir/src/Matrix.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Matrix.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Matrix.C

CMakeFiles/ears.dir/src/Matrix.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Matrix.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Matrix.C > CMakeFiles/ears.dir/src/Matrix.C.i

CMakeFiles/ears.dir/src/Matrix.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Matrix.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Matrix.C -o CMakeFiles/ears.dir/src/Matrix.C.s

CMakeFiles/ears.dir/src/Matrix.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Matrix.C.o.requires

CMakeFiles/ears.dir/src/Matrix.C.o.provides: CMakeFiles/ears.dir/src/Matrix.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Matrix.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Matrix.C.o.provides

CMakeFiles/ears.dir/src/Matrix.C.o.provides.build: CMakeFiles/ears.dir/src/Matrix.C.o


CMakeFiles/ears.dir/src/Noise.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Noise.C.o: ../src/Noise.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/ears.dir/src/Noise.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Noise.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Noise.C

CMakeFiles/ears.dir/src/Noise.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Noise.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Noise.C > CMakeFiles/ears.dir/src/Noise.C.i

CMakeFiles/ears.dir/src/Noise.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Noise.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Noise.C -o CMakeFiles/ears.dir/src/Noise.C.s

CMakeFiles/ears.dir/src/Noise.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Noise.C.o.requires

CMakeFiles/ears.dir/src/Noise.C.o.provides: CMakeFiles/ears.dir/src/Noise.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Noise.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Noise.C.o.provides

CMakeFiles/ears.dir/src/Noise.C.o.provides.build: CMakeFiles/ears.dir/src/Noise.C.o


CMakeFiles/ears.dir/src/OIIOFiles.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/OIIOFiles.C.o: ../src/OIIOFiles.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/ears.dir/src/OIIOFiles.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/OIIOFiles.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/OIIOFiles.C

CMakeFiles/ears.dir/src/OIIOFiles.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/OIIOFiles.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/OIIOFiles.C > CMakeFiles/ears.dir/src/OIIOFiles.C.i

CMakeFiles/ears.dir/src/OIIOFiles.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/OIIOFiles.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/OIIOFiles.C -o CMakeFiles/ears.dir/src/OIIOFiles.C.s

CMakeFiles/ears.dir/src/OIIOFiles.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/OIIOFiles.C.o.requires

CMakeFiles/ears.dir/src/OIIOFiles.C.o.provides: CMakeFiles/ears.dir/src/OIIOFiles.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/OIIOFiles.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/OIIOFiles.C.o.provides

CMakeFiles/ears.dir/src/OIIOFiles.C.o.provides.build: CMakeFiles/ears.dir/src/OIIOFiles.C.o


CMakeFiles/ears.dir/src/PerlinNoise.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/PerlinNoise.C.o: ../src/PerlinNoise.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/ears.dir/src/PerlinNoise.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/PerlinNoise.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/PerlinNoise.C

CMakeFiles/ears.dir/src/PerlinNoise.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/PerlinNoise.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/PerlinNoise.C > CMakeFiles/ears.dir/src/PerlinNoise.C.i

CMakeFiles/ears.dir/src/PerlinNoise.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/PerlinNoise.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/PerlinNoise.C -o CMakeFiles/ears.dir/src/PerlinNoise.C.s

CMakeFiles/ears.dir/src/PerlinNoise.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/PerlinNoise.C.o.requires

CMakeFiles/ears.dir/src/PerlinNoise.C.o.provides: CMakeFiles/ears.dir/src/PerlinNoise.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/PerlinNoise.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/PerlinNoise.C.o.provides

CMakeFiles/ears.dir/src/PerlinNoise.C.o.provides.build: CMakeFiles/ears.dir/src/PerlinNoise.C.o


CMakeFiles/ears.dir/src/PolyModel.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/PolyModel.C.o: ../src/PolyModel.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/ears.dir/src/PolyModel.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/PolyModel.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/PolyModel.C

CMakeFiles/ears.dir/src/PolyModel.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/PolyModel.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/PolyModel.C > CMakeFiles/ears.dir/src/PolyModel.C.i

CMakeFiles/ears.dir/src/PolyModel.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/PolyModel.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/PolyModel.C -o CMakeFiles/ears.dir/src/PolyModel.C.s

CMakeFiles/ears.dir/src/PolyModel.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/PolyModel.C.o.requires

CMakeFiles/ears.dir/src/PolyModel.C.o.provides: CMakeFiles/ears.dir/src/PolyModel.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/PolyModel.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/PolyModel.C.o.provides

CMakeFiles/ears.dir/src/PolyModel.C.o.provides.build: CMakeFiles/ears.dir/src/PolyModel.C.o


CMakeFiles/ears.dir/src/Pyroclastic.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Pyroclastic.C.o: ../src/Pyroclastic.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/ears.dir/src/Pyroclastic.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Pyroclastic.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Pyroclastic.C

CMakeFiles/ears.dir/src/Pyroclastic.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Pyroclastic.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Pyroclastic.C > CMakeFiles/ears.dir/src/Pyroclastic.C.i

CMakeFiles/ears.dir/src/Pyroclastic.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Pyroclastic.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Pyroclastic.C -o CMakeFiles/ears.dir/src/Pyroclastic.C.s

CMakeFiles/ears.dir/src/Pyroclastic.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Pyroclastic.C.o.requires

CMakeFiles/ears.dir/src/Pyroclastic.C.o.provides: CMakeFiles/ears.dir/src/Pyroclastic.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Pyroclastic.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Pyroclastic.C.o.provides

CMakeFiles/ears.dir/src/Pyroclastic.C.o.provides.build: CMakeFiles/ears.dir/src/Pyroclastic.C.o


CMakeFiles/ears.dir/src/Renderer.C.o: CMakeFiles/ears.dir/flags.make
CMakeFiles/ears.dir/src/Renderer.C.o: ../src/Renderer.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/ears.dir/src/Renderer.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ears.dir/src/Renderer.C.o -c /home/jingcoz/workspace/clion_projects/advect/src/Renderer.C

CMakeFiles/ears.dir/src/Renderer.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ears.dir/src/Renderer.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/advect/src/Renderer.C > CMakeFiles/ears.dir/src/Renderer.C.i

CMakeFiles/ears.dir/src/Renderer.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ears.dir/src/Renderer.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/advect/src/Renderer.C -o CMakeFiles/ears.dir/src/Renderer.C.s

CMakeFiles/ears.dir/src/Renderer.C.o.requires:

.PHONY : CMakeFiles/ears.dir/src/Renderer.C.o.requires

CMakeFiles/ears.dir/src/Renderer.C.o.provides: CMakeFiles/ears.dir/src/Renderer.C.o.requires
	$(MAKE) -f CMakeFiles/ears.dir/build.make CMakeFiles/ears.dir/src/Renderer.C.o.provides.build
.PHONY : CMakeFiles/ears.dir/src/Renderer.C.o.provides

CMakeFiles/ears.dir/src/Renderer.C.o.provides.build: CMakeFiles/ears.dir/src/Renderer.C.o


# Object files for target ears
ears_OBJECTS = \
"CMakeFiles/ears.dir/ears.C.o" \
"CMakeFiles/ears.dir/src/Camera.C.o" \
"CMakeFiles/ears.dir/src/Grid.C.o" \
"CMakeFiles/ears.dir/src/Image.C.o" \
"CMakeFiles/ears.dir/src/Levelsets.C.o" \
"CMakeFiles/ears.dir/src/Lighting.C.o" \
"CMakeFiles/ears.dir/src/Matrix.C.o" \
"CMakeFiles/ears.dir/src/Noise.C.o" \
"CMakeFiles/ears.dir/src/OIIOFiles.C.o" \
"CMakeFiles/ears.dir/src/PerlinNoise.C.o" \
"CMakeFiles/ears.dir/src/PolyModel.C.o" \
"CMakeFiles/ears.dir/src/Pyroclastic.C.o" \
"CMakeFiles/ears.dir/src/Renderer.C.o"

# External object files for target ears
ears_EXTERNAL_OBJECTS =

ears: CMakeFiles/ears.dir/ears.C.o
ears: CMakeFiles/ears.dir/src/Camera.C.o
ears: CMakeFiles/ears.dir/src/Grid.C.o
ears: CMakeFiles/ears.dir/src/Image.C.o
ears: CMakeFiles/ears.dir/src/Levelsets.C.o
ears: CMakeFiles/ears.dir/src/Lighting.C.o
ears: CMakeFiles/ears.dir/src/Matrix.C.o
ears: CMakeFiles/ears.dir/src/Noise.C.o
ears: CMakeFiles/ears.dir/src/OIIOFiles.C.o
ears: CMakeFiles/ears.dir/src/PerlinNoise.C.o
ears: CMakeFiles/ears.dir/src/PolyModel.C.o
ears: CMakeFiles/ears.dir/src/Pyroclastic.C.o
ears: CMakeFiles/ears.dir/src/Renderer.C.o
ears: CMakeFiles/ears.dir/build.make
ears: CMakeFiles/ears.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable ears"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ears.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ears.dir/build: ears

.PHONY : CMakeFiles/ears.dir/build

CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/ears.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Camera.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Grid.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Image.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Levelsets.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Lighting.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Matrix.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Noise.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/OIIOFiles.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/PerlinNoise.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/PolyModel.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Pyroclastic.C.o.requires
CMakeFiles/ears.dir/requires: CMakeFiles/ears.dir/src/Renderer.C.o.requires

.PHONY : CMakeFiles/ears.dir/requires

CMakeFiles/ears.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ears.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ears.dir/clean

CMakeFiles/ears.dir/depend:
	cd /home/jingcoz/workspace/clion_projects/advect/cmake-build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jingcoz/workspace/clion_projects/advect /home/jingcoz/workspace/clion_projects/advect /home/jingcoz/workspace/clion_projects/advect/cmake-build-default /home/jingcoz/workspace/clion_projects/advect/cmake-build-default /home/jingcoz/workspace/clion_projects/advect/cmake-build-default/CMakeFiles/ears.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ears.dir/depend
