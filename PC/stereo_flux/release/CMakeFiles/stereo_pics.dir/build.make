# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/stereo-2022/stereo-2022/StereoVision/stereo_flux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release

# Include any dependencies generated for this target.
include CMakeFiles/stereo_pics.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/stereo_pics.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stereo_pics.dir/flags.make

CMakeFiles/stereo_pics.dir/main.cpp.o: CMakeFiles/stereo_pics.dir/flags.make
CMakeFiles/stereo_pics.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/stereo_pics.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/stereo_pics.dir/main.cpp.o -c /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/main.cpp

CMakeFiles/stereo_pics.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stereo_pics.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/main.cpp > CMakeFiles/stereo_pics.dir/main.cpp.i

CMakeFiles/stereo_pics.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stereo_pics.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/main.cpp -o CMakeFiles/stereo_pics.dir/main.cpp.s

CMakeFiles/stereo_pics.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/stereo_pics.dir/main.cpp.o.requires

CMakeFiles/stereo_pics.dir/main.cpp.o.provides: CMakeFiles/stereo_pics.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/stereo_pics.dir/build.make CMakeFiles/stereo_pics.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/stereo_pics.dir/main.cpp.o.provides

CMakeFiles/stereo_pics.dir/main.cpp.o.provides.build: CMakeFiles/stereo_pics.dir/main.cpp.o


# Object files for target stereo_pics
stereo_pics_OBJECTS = \
"CMakeFiles/stereo_pics.dir/main.cpp.o"

# External object files for target stereo_pics
stereo_pics_EXTERNAL_OBJECTS =

stereo_pics: CMakeFiles/stereo_pics.dir/main.cpp.o
stereo_pics: CMakeFiles/stereo_pics.dir/build.make
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_dnn.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_gapi.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_video.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.1.1
stereo_pics: /usr/lib/x86_64-linux-gnu/libopencv_core.so.4.1.1
stereo_pics: CMakeFiles/stereo_pics.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stereo_pics"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stereo_pics.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stereo_pics.dir/build: stereo_pics

.PHONY : CMakeFiles/stereo_pics.dir/build

CMakeFiles/stereo_pics.dir/requires: CMakeFiles/stereo_pics.dir/main.cpp.o.requires

.PHONY : CMakeFiles/stereo_pics.dir/requires

CMakeFiles/stereo_pics.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stereo_pics.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stereo_pics.dir/clean

CMakeFiles/stereo_pics.dir/depend:
	cd /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stereo-2022/stereo-2022/StereoVision/stereo_flux /home/stereo-2022/stereo-2022/StereoVision/stereo_flux /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release /home/stereo-2022/stereo-2022/StereoVision/stereo_flux/release/CMakeFiles/stereo_pics.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stereo_pics.dir/depend
