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
CMAKE_SOURCE_DIR = /home/jingcoz/workspace/clion_projects/wand

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jingcoz/workspace/clion_projects/wand/cmake-build-default

# Include any dependencies generated for this target.
include CMakeFiles/wand.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wand.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wand.dir/flags.make

CMakeFiles/wand.dir/wand.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/wand.C.o: ../wand.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wand.dir/wand.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/wand.C.o -c /home/jingcoz/workspace/clion_projects/wand/wand.C

CMakeFiles/wand.dir/wand.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/wand.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/wand.C > CMakeFiles/wand.dir/wand.C.i

CMakeFiles/wand.dir/wand.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/wand.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/wand.C -o CMakeFiles/wand.dir/wand.C.s

CMakeFiles/wand.dir/wand.C.o.requires:

.PHONY : CMakeFiles/wand.dir/wand.C.o.requires

CMakeFiles/wand.dir/wand.C.o.provides: CMakeFiles/wand.dir/wand.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/wand.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/wand.C.o.provides

CMakeFiles/wand.dir/wand.C.o.provides.build: CMakeFiles/wand.dir/wand.C.o


CMakeFiles/wand.dir/src/Camera.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Camera.C.o: ../src/Camera.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/wand.dir/src/Camera.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Camera.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Camera.C

CMakeFiles/wand.dir/src/Camera.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Camera.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Camera.C > CMakeFiles/wand.dir/src/Camera.C.i

CMakeFiles/wand.dir/src/Camera.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Camera.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Camera.C -o CMakeFiles/wand.dir/src/Camera.C.s

CMakeFiles/wand.dir/src/Camera.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Camera.C.o.requires

CMakeFiles/wand.dir/src/Camera.C.o.provides: CMakeFiles/wand.dir/src/Camera.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Camera.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Camera.C.o.provides

CMakeFiles/wand.dir/src/Camera.C.o.provides.build: CMakeFiles/wand.dir/src/Camera.C.o


CMakeFiles/wand.dir/src/Grid.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Grid.C.o: ../src/Grid.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/wand.dir/src/Grid.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Grid.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Grid.C

CMakeFiles/wand.dir/src/Grid.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Grid.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Grid.C > CMakeFiles/wand.dir/src/Grid.C.i

CMakeFiles/wand.dir/src/Grid.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Grid.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Grid.C -o CMakeFiles/wand.dir/src/Grid.C.s

CMakeFiles/wand.dir/src/Grid.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Grid.C.o.requires

CMakeFiles/wand.dir/src/Grid.C.o.provides: CMakeFiles/wand.dir/src/Grid.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Grid.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Grid.C.o.provides

CMakeFiles/wand.dir/src/Grid.C.o.provides.build: CMakeFiles/wand.dir/src/Grid.C.o


CMakeFiles/wand.dir/src/Image.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Image.C.o: ../src/Image.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/wand.dir/src/Image.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Image.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Image.C

CMakeFiles/wand.dir/src/Image.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Image.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Image.C > CMakeFiles/wand.dir/src/Image.C.i

CMakeFiles/wand.dir/src/Image.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Image.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Image.C -o CMakeFiles/wand.dir/src/Image.C.s

CMakeFiles/wand.dir/src/Image.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Image.C.o.requires

CMakeFiles/wand.dir/src/Image.C.o.provides: CMakeFiles/wand.dir/src/Image.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Image.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Image.C.o.provides

CMakeFiles/wand.dir/src/Image.C.o.provides.build: CMakeFiles/wand.dir/src/Image.C.o


CMakeFiles/wand.dir/src/Levelsets.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Levelsets.C.o: ../src/Levelsets.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/wand.dir/src/Levelsets.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Levelsets.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Levelsets.C

CMakeFiles/wand.dir/src/Levelsets.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Levelsets.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Levelsets.C > CMakeFiles/wand.dir/src/Levelsets.C.i

CMakeFiles/wand.dir/src/Levelsets.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Levelsets.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Levelsets.C -o CMakeFiles/wand.dir/src/Levelsets.C.s

CMakeFiles/wand.dir/src/Levelsets.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Levelsets.C.o.requires

CMakeFiles/wand.dir/src/Levelsets.C.o.provides: CMakeFiles/wand.dir/src/Levelsets.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Levelsets.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Levelsets.C.o.provides

CMakeFiles/wand.dir/src/Levelsets.C.o.provides.build: CMakeFiles/wand.dir/src/Levelsets.C.o


CMakeFiles/wand.dir/src/Lighting.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Lighting.C.o: ../src/Lighting.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/wand.dir/src/Lighting.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Lighting.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Lighting.C

CMakeFiles/wand.dir/src/Lighting.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Lighting.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Lighting.C > CMakeFiles/wand.dir/src/Lighting.C.i

CMakeFiles/wand.dir/src/Lighting.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Lighting.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Lighting.C -o CMakeFiles/wand.dir/src/Lighting.C.s

CMakeFiles/wand.dir/src/Lighting.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Lighting.C.o.requires

CMakeFiles/wand.dir/src/Lighting.C.o.provides: CMakeFiles/wand.dir/src/Lighting.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Lighting.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Lighting.C.o.provides

CMakeFiles/wand.dir/src/Lighting.C.o.provides.build: CMakeFiles/wand.dir/src/Lighting.C.o


CMakeFiles/wand.dir/src/Matrix.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Matrix.C.o: ../src/Matrix.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/wand.dir/src/Matrix.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Matrix.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Matrix.C

CMakeFiles/wand.dir/src/Matrix.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Matrix.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Matrix.C > CMakeFiles/wand.dir/src/Matrix.C.i

CMakeFiles/wand.dir/src/Matrix.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Matrix.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Matrix.C -o CMakeFiles/wand.dir/src/Matrix.C.s

CMakeFiles/wand.dir/src/Matrix.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Matrix.C.o.requires

CMakeFiles/wand.dir/src/Matrix.C.o.provides: CMakeFiles/wand.dir/src/Matrix.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Matrix.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Matrix.C.o.provides

CMakeFiles/wand.dir/src/Matrix.C.o.provides.build: CMakeFiles/wand.dir/src/Matrix.C.o


CMakeFiles/wand.dir/src/Noise.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Noise.C.o: ../src/Noise.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/wand.dir/src/Noise.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Noise.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Noise.C

CMakeFiles/wand.dir/src/Noise.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Noise.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Noise.C > CMakeFiles/wand.dir/src/Noise.C.i

CMakeFiles/wand.dir/src/Noise.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Noise.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Noise.C -o CMakeFiles/wand.dir/src/Noise.C.s

CMakeFiles/wand.dir/src/Noise.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Noise.C.o.requires

CMakeFiles/wand.dir/src/Noise.C.o.provides: CMakeFiles/wand.dir/src/Noise.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Noise.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Noise.C.o.provides

CMakeFiles/wand.dir/src/Noise.C.o.provides.build: CMakeFiles/wand.dir/src/Noise.C.o


CMakeFiles/wand.dir/src/OIIOFiles.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/OIIOFiles.C.o: ../src/OIIOFiles.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/wand.dir/src/OIIOFiles.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/OIIOFiles.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/OIIOFiles.C

CMakeFiles/wand.dir/src/OIIOFiles.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/OIIOFiles.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/OIIOFiles.C > CMakeFiles/wand.dir/src/OIIOFiles.C.i

CMakeFiles/wand.dir/src/OIIOFiles.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/OIIOFiles.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/OIIOFiles.C -o CMakeFiles/wand.dir/src/OIIOFiles.C.s

CMakeFiles/wand.dir/src/OIIOFiles.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/OIIOFiles.C.o.requires

CMakeFiles/wand.dir/src/OIIOFiles.C.o.provides: CMakeFiles/wand.dir/src/OIIOFiles.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/OIIOFiles.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/OIIOFiles.C.o.provides

CMakeFiles/wand.dir/src/OIIOFiles.C.o.provides.build: CMakeFiles/wand.dir/src/OIIOFiles.C.o


CMakeFiles/wand.dir/src/Particles.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Particles.C.o: ../src/Particles.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/wand.dir/src/Particles.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Particles.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Particles.C

CMakeFiles/wand.dir/src/Particles.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Particles.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Particles.C > CMakeFiles/wand.dir/src/Particles.C.i

CMakeFiles/wand.dir/src/Particles.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Particles.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Particles.C -o CMakeFiles/wand.dir/src/Particles.C.s

CMakeFiles/wand.dir/src/Particles.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Particles.C.o.requires

CMakeFiles/wand.dir/src/Particles.C.o.provides: CMakeFiles/wand.dir/src/Particles.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Particles.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Particles.C.o.provides

CMakeFiles/wand.dir/src/Particles.C.o.provides.build: CMakeFiles/wand.dir/src/Particles.C.o


CMakeFiles/wand.dir/src/PerlinNoise.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/PerlinNoise.C.o: ../src/PerlinNoise.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/wand.dir/src/PerlinNoise.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/PerlinNoise.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/PerlinNoise.C

CMakeFiles/wand.dir/src/PerlinNoise.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/PerlinNoise.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/PerlinNoise.C > CMakeFiles/wand.dir/src/PerlinNoise.C.i

CMakeFiles/wand.dir/src/PerlinNoise.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/PerlinNoise.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/PerlinNoise.C -o CMakeFiles/wand.dir/src/PerlinNoise.C.s

CMakeFiles/wand.dir/src/PerlinNoise.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/PerlinNoise.C.o.requires

CMakeFiles/wand.dir/src/PerlinNoise.C.o.provides: CMakeFiles/wand.dir/src/PerlinNoise.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/PerlinNoise.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/PerlinNoise.C.o.provides

CMakeFiles/wand.dir/src/PerlinNoise.C.o.provides.build: CMakeFiles/wand.dir/src/PerlinNoise.C.o


CMakeFiles/wand.dir/src/PolyModel.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/PolyModel.C.o: ../src/PolyModel.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/wand.dir/src/PolyModel.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/PolyModel.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/PolyModel.C

CMakeFiles/wand.dir/src/PolyModel.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/PolyModel.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/PolyModel.C > CMakeFiles/wand.dir/src/PolyModel.C.i

CMakeFiles/wand.dir/src/PolyModel.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/PolyModel.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/PolyModel.C -o CMakeFiles/wand.dir/src/PolyModel.C.s

CMakeFiles/wand.dir/src/PolyModel.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/PolyModel.C.o.requires

CMakeFiles/wand.dir/src/PolyModel.C.o.provides: CMakeFiles/wand.dir/src/PolyModel.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/PolyModel.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/PolyModel.C.o.provides

CMakeFiles/wand.dir/src/PolyModel.C.o.provides.build: CMakeFiles/wand.dir/src/PolyModel.C.o


CMakeFiles/wand.dir/src/Pyroclastic.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Pyroclastic.C.o: ../src/Pyroclastic.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/wand.dir/src/Pyroclastic.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Pyroclastic.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Pyroclastic.C

CMakeFiles/wand.dir/src/Pyroclastic.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Pyroclastic.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Pyroclastic.C > CMakeFiles/wand.dir/src/Pyroclastic.C.i

CMakeFiles/wand.dir/src/Pyroclastic.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Pyroclastic.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Pyroclastic.C -o CMakeFiles/wand.dir/src/Pyroclastic.C.s

CMakeFiles/wand.dir/src/Pyroclastic.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Pyroclastic.C.o.requires

CMakeFiles/wand.dir/src/Pyroclastic.C.o.provides: CMakeFiles/wand.dir/src/Pyroclastic.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Pyroclastic.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Pyroclastic.C.o.provides

CMakeFiles/wand.dir/src/Pyroclastic.C.o.provides.build: CMakeFiles/wand.dir/src/Pyroclastic.C.o


CMakeFiles/wand.dir/src/Renderer.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Renderer.C.o: ../src/Renderer.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/wand.dir/src/Renderer.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Renderer.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Renderer.C

CMakeFiles/wand.dir/src/Renderer.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Renderer.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Renderer.C > CMakeFiles/wand.dir/src/Renderer.C.i

CMakeFiles/wand.dir/src/Renderer.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Renderer.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Renderer.C -o CMakeFiles/wand.dir/src/Renderer.C.s

CMakeFiles/wand.dir/src/Renderer.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Renderer.C.o.requires

CMakeFiles/wand.dir/src/Renderer.C.o.provides: CMakeFiles/wand.dir/src/Renderer.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Renderer.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Renderer.C.o.provides

CMakeFiles/wand.dir/src/Renderer.C.o.provides.build: CMakeFiles/wand.dir/src/Renderer.C.o


CMakeFiles/wand.dir/src/Wisp.C.o: CMakeFiles/wand.dir/flags.make
CMakeFiles/wand.dir/src/Wisp.C.o: ../src/Wisp.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/wand.dir/src/Wisp.C.o"
	g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wand.dir/src/Wisp.C.o -c /home/jingcoz/workspace/clion_projects/wand/src/Wisp.C

CMakeFiles/wand.dir/src/Wisp.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wand.dir/src/Wisp.C.i"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jingcoz/workspace/clion_projects/wand/src/Wisp.C > CMakeFiles/wand.dir/src/Wisp.C.i

CMakeFiles/wand.dir/src/Wisp.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wand.dir/src/Wisp.C.s"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jingcoz/workspace/clion_projects/wand/src/Wisp.C -o CMakeFiles/wand.dir/src/Wisp.C.s

CMakeFiles/wand.dir/src/Wisp.C.o.requires:

.PHONY : CMakeFiles/wand.dir/src/Wisp.C.o.requires

CMakeFiles/wand.dir/src/Wisp.C.o.provides: CMakeFiles/wand.dir/src/Wisp.C.o.requires
	$(MAKE) -f CMakeFiles/wand.dir/build.make CMakeFiles/wand.dir/src/Wisp.C.o.provides.build
.PHONY : CMakeFiles/wand.dir/src/Wisp.C.o.provides

CMakeFiles/wand.dir/src/Wisp.C.o.provides.build: CMakeFiles/wand.dir/src/Wisp.C.o


# Object files for target wand
wand_OBJECTS = \
"CMakeFiles/wand.dir/wand.C.o" \
"CMakeFiles/wand.dir/src/Camera.C.o" \
"CMakeFiles/wand.dir/src/Grid.C.o" \
"CMakeFiles/wand.dir/src/Image.C.o" \
"CMakeFiles/wand.dir/src/Levelsets.C.o" \
"CMakeFiles/wand.dir/src/Lighting.C.o" \
"CMakeFiles/wand.dir/src/Matrix.C.o" \
"CMakeFiles/wand.dir/src/Noise.C.o" \
"CMakeFiles/wand.dir/src/OIIOFiles.C.o" \
"CMakeFiles/wand.dir/src/Particles.C.o" \
"CMakeFiles/wand.dir/src/PerlinNoise.C.o" \
"CMakeFiles/wand.dir/src/PolyModel.C.o" \
"CMakeFiles/wand.dir/src/Pyroclastic.C.o" \
"CMakeFiles/wand.dir/src/Renderer.C.o" \
"CMakeFiles/wand.dir/src/Wisp.C.o"

# External object files for target wand
wand_EXTERNAL_OBJECTS =

wand: CMakeFiles/wand.dir/wand.C.o
wand: CMakeFiles/wand.dir/src/Camera.C.o
wand: CMakeFiles/wand.dir/src/Grid.C.o
wand: CMakeFiles/wand.dir/src/Image.C.o
wand: CMakeFiles/wand.dir/src/Levelsets.C.o
wand: CMakeFiles/wand.dir/src/Lighting.C.o
wand: CMakeFiles/wand.dir/src/Matrix.C.o
wand: CMakeFiles/wand.dir/src/Noise.C.o
wand: CMakeFiles/wand.dir/src/OIIOFiles.C.o
wand: CMakeFiles/wand.dir/src/Particles.C.o
wand: CMakeFiles/wand.dir/src/PerlinNoise.C.o
wand: CMakeFiles/wand.dir/src/PolyModel.C.o
wand: CMakeFiles/wand.dir/src/Pyroclastic.C.o
wand: CMakeFiles/wand.dir/src/Renderer.C.o
wand: CMakeFiles/wand.dir/src/Wisp.C.o
wand: CMakeFiles/wand.dir/build.make
wand: CMakeFiles/wand.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable wand"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wand.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wand.dir/build: wand

.PHONY : CMakeFiles/wand.dir/build

CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/wand.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Camera.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Grid.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Image.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Levelsets.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Lighting.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Matrix.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Noise.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/OIIOFiles.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Particles.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/PerlinNoise.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/PolyModel.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Pyroclastic.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Renderer.C.o.requires
CMakeFiles/wand.dir/requires: CMakeFiles/wand.dir/src/Wisp.C.o.requires

.PHONY : CMakeFiles/wand.dir/requires

CMakeFiles/wand.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wand.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wand.dir/clean

CMakeFiles/wand.dir/depend:
	cd /home/jingcoz/workspace/clion_projects/wand/cmake-build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jingcoz/workspace/clion_projects/wand /home/jingcoz/workspace/clion_projects/wand /home/jingcoz/workspace/clion_projects/wand/cmake-build-default /home/jingcoz/workspace/clion_projects/wand/cmake-build-default /home/jingcoz/workspace/clion_projects/wand/cmake-build-default/CMakeFiles/wand.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wand.dir/depend

