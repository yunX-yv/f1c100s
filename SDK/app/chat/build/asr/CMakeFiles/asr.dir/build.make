# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/xiao/project/f1c100s/SDK/app/chat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiao/project/f1c100s/SDK/app/chat/build

# Include any dependencies generated for this target.
include asr/CMakeFiles/asr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include asr/CMakeFiles/asr.dir/compiler_depend.make

# Include the progress variables for this target.
include asr/CMakeFiles/asr.dir/progress.make

# Include the compile flags for this target's objects.
include asr/CMakeFiles/asr.dir/flags.make

asr/CMakeFiles/asr.dir/asr.cpp.o: asr/CMakeFiles/asr.dir/flags.make
asr/CMakeFiles/asr.dir/asr.cpp.o: /home/xiao/project/f1c100s/SDK/app/chat/asr/asr.cpp
asr/CMakeFiles/asr.dir/asr.cpp.o: asr/CMakeFiles/asr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/xiao/project/f1c100s/SDK/app/chat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object asr/CMakeFiles/asr.dir/asr.cpp.o"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && /home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT asr/CMakeFiles/asr.dir/asr.cpp.o -MF CMakeFiles/asr.dir/asr.cpp.o.d -o CMakeFiles/asr.dir/asr.cpp.o -c /home/xiao/project/f1c100s/SDK/app/chat/asr/asr.cpp

asr/CMakeFiles/asr.dir/asr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/asr.dir/asr.cpp.i"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && /home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiao/project/f1c100s/SDK/app/chat/asr/asr.cpp > CMakeFiles/asr.dir/asr.cpp.i

asr/CMakeFiles/asr.dir/asr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/asr.dir/asr.cpp.s"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && /home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiao/project/f1c100s/SDK/app/chat/asr/asr.cpp -o CMakeFiles/asr.dir/asr.cpp.s

asr/CMakeFiles/asr.dir/common.cpp.o: asr/CMakeFiles/asr.dir/flags.make
asr/CMakeFiles/asr.dir/common.cpp.o: /home/xiao/project/f1c100s/SDK/app/chat/asr/common.cpp
asr/CMakeFiles/asr.dir/common.cpp.o: asr/CMakeFiles/asr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/xiao/project/f1c100s/SDK/app/chat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object asr/CMakeFiles/asr.dir/common.cpp.o"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && /home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT asr/CMakeFiles/asr.dir/common.cpp.o -MF CMakeFiles/asr.dir/common.cpp.o.d -o CMakeFiles/asr.dir/common.cpp.o -c /home/xiao/project/f1c100s/SDK/app/chat/asr/common.cpp

asr/CMakeFiles/asr.dir/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/asr.dir/common.cpp.i"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && /home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiao/project/f1c100s/SDK/app/chat/asr/common.cpp > CMakeFiles/asr.dir/common.cpp.i

asr/CMakeFiles/asr.dir/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/asr.dir/common.cpp.s"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && /home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiao/project/f1c100s/SDK/app/chat/asr/common.cpp -o CMakeFiles/asr.dir/common.cpp.s

# Object files for target asr
asr_OBJECTS = \
"CMakeFiles/asr.dir/asr.cpp.o" \
"CMakeFiles/asr.dir/common.cpp.o"

# External object files for target asr
asr_EXTERNAL_OBJECTS =

asr/libasr.a: asr/CMakeFiles/asr.dir/asr.cpp.o
asr/libasr.a: asr/CMakeFiles/asr.dir/common.cpp.o
asr/libasr.a: asr/CMakeFiles/asr.dir/build.make
asr/libasr.a: asr/CMakeFiles/asr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/xiao/project/f1c100s/SDK/app/chat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libasr.a"
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && $(CMAKE_COMMAND) -P CMakeFiles/asr.dir/cmake_clean_target.cmake
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/asr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
asr/CMakeFiles/asr.dir/build: asr/libasr.a
.PHONY : asr/CMakeFiles/asr.dir/build

asr/CMakeFiles/asr.dir/clean:
	cd /home/xiao/project/f1c100s/SDK/app/chat/build/asr && $(CMAKE_COMMAND) -P CMakeFiles/asr.dir/cmake_clean.cmake
.PHONY : asr/CMakeFiles/asr.dir/clean

asr/CMakeFiles/asr.dir/depend:
	cd /home/xiao/project/f1c100s/SDK/app/chat/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiao/project/f1c100s/SDK/app/chat /home/xiao/project/f1c100s/SDK/app/chat/asr /home/xiao/project/f1c100s/SDK/app/chat/build /home/xiao/project/f1c100s/SDK/app/chat/build/asr /home/xiao/project/f1c100s/SDK/app/chat/build/asr/CMakeFiles/asr.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : asr/CMakeFiles/asr.dir/depend

