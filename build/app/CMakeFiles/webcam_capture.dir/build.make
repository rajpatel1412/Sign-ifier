# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/psalniko/cmpt433/work/Sign-ifier/build

# Include any dependencies generated for this target.
include app/CMakeFiles/webcam_capture.dir/depend.make

# Include the progress variables for this target.
include app/CMakeFiles/webcam_capture.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/webcam_capture.dir/flags.make

app/CMakeFiles/webcam_capture.dir/src/capture.c.o: app/CMakeFiles/webcam_capture.dir/flags.make
app/CMakeFiles/webcam_capture.dir/src/capture.c.o: /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/capture.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/psalniko/cmpt433/work/Sign-ifier/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object app/CMakeFiles/webcam_capture.dir/src/capture.c.o"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/webcam_capture.dir/src/capture.c.o -c /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/capture.c

app/CMakeFiles/webcam_capture.dir/src/capture.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/webcam_capture.dir/src/capture.c.i"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/capture.c > CMakeFiles/webcam_capture.dir/src/capture.c.i

app/CMakeFiles/webcam_capture.dir/src/capture.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/webcam_capture.dir/src/capture.c.s"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/capture.c -o CMakeFiles/webcam_capture.dir/src/capture.c.s

app/CMakeFiles/webcam_capture.dir/src/main.c.o: app/CMakeFiles/webcam_capture.dir/flags.make
app/CMakeFiles/webcam_capture.dir/src/main.c.o: /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/psalniko/cmpt433/work/Sign-ifier/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object app/CMakeFiles/webcam_capture.dir/src/main.c.o"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/webcam_capture.dir/src/main.c.o -c /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/main.c

app/CMakeFiles/webcam_capture.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/webcam_capture.dir/src/main.c.i"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/main.c > CMakeFiles/webcam_capture.dir/src/main.c.i

app/CMakeFiles/webcam_capture.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/webcam_capture.dir/src/main.c.s"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app/src/main.c -o CMakeFiles/webcam_capture.dir/src/main.c.s

# Object files for target webcam_capture
webcam_capture_OBJECTS = \
"CMakeFiles/webcam_capture.dir/src/capture.c.o" \
"CMakeFiles/webcam_capture.dir/src/main.c.o"

# External object files for target webcam_capture
webcam_capture_EXTERNAL_OBJECTS =

app/webcam_capture: app/CMakeFiles/webcam_capture.dir/src/capture.c.o
app/webcam_capture: app/CMakeFiles/webcam_capture.dir/src/main.c.o
app/webcam_capture: app/CMakeFiles/webcam_capture.dir/build.make
app/webcam_capture: app/CMakeFiles/webcam_capture.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/psalniko/cmpt433/work/Sign-ifier/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable webcam_capture"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webcam_capture.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying ARM executable to public NFS directory"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/cmake -E copy /home/psalniko/cmpt433/work/Sign-ifier/build/app/webcam_capture ~/cmpt433/public/myApps/project_test2/webcam_capture
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying server to public NFS directory"
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && /usr/bin/cmake -E copy_directory /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/server $(HOME)/cmpt433/public/myApps/project_test2/server-files/

# Rule to build all files generated by this target.
app/CMakeFiles/webcam_capture.dir/build: app/webcam_capture

.PHONY : app/CMakeFiles/webcam_capture.dir/build

app/CMakeFiles/webcam_capture.dir/clean:
	cd /home/psalniko/cmpt433/work/Sign-ifier/build/app && $(CMAKE_COMMAND) -P CMakeFiles/webcam_capture.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/webcam_capture.dir/clean

app/CMakeFiles/webcam_capture.dir/depend:
	cd /home/psalniko/cmpt433/work/Sign-ifier/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo /home/psalniko/cmpt433/work/Sign-ifier/cmake_demo/app /home/psalniko/cmpt433/work/Sign-ifier/build /home/psalniko/cmpt433/work/Sign-ifier/build/app /home/psalniko/cmpt433/work/Sign-ifier/build/app/CMakeFiles/webcam_capture.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/webcam_capture.dir/depend

