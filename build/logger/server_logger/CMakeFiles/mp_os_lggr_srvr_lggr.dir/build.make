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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/vioviooo/Desktop/twink/mp_os

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/vioviooo/Desktop/twink/mp_os/build

# Include any dependencies generated for this target.
include logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/compiler_depend.make

# Include the progress variables for this target.
include logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/progress.make

# Include the compile flags for this target's objects.
include logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/flags.make

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/flags.make
logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o: /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger.cpp
logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/vioviooo/Desktop/twink/mp_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o -MF CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o.d -o CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o -c /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger.cpp

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.i"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger.cpp > CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.i

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.s"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger.cpp -o CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.s

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/flags.make
logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o: /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger_builder.cpp
logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/vioviooo/Desktop/twink/mp_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o -MF CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o.d -o CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o -c /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger_builder.cpp

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.i"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger_builder.cpp > CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.i

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.s"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger/src/server_logger_builder.cpp -o CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.s

# Object files for target mp_os_lggr_srvr_lggr
mp_os_lggr_srvr_lggr_OBJECTS = \
"CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o" \
"CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o"

# External object files for target mp_os_lggr_srvr_lggr
mp_os_lggr_srvr_lggr_EXTERNAL_OBJECTS =

logger/server_logger/libmp_os_lggr_srvr_lggr.a: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger.cpp.o
logger/server_logger/libmp_os_lggr_srvr_lggr.a: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/src/server_logger_builder.cpp.o
logger/server_logger/libmp_os_lggr_srvr_lggr.a: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/build.make
logger/server_logger/libmp_os_lggr_srvr_lggr.a: logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/vioviooo/Desktop/twink/mp_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libmp_os_lggr_srvr_lggr.a"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_lggr_srvr_lggr.dir/cmake_clean_target.cmake
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mp_os_lggr_srvr_lggr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/build: logger/server_logger/libmp_os_lggr_srvr_lggr.a
.PHONY : logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/build

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/clean:
	cd /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_lggr_srvr_lggr.dir/cmake_clean.cmake
.PHONY : logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/clean

logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/depend:
	cd /Users/vioviooo/Desktop/twink/mp_os/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vioviooo/Desktop/twink/mp_os /Users/vioviooo/Desktop/twink/mp_os/logger/server_logger /Users/vioviooo/Desktop/twink/mp_os/build /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger /Users/vioviooo/Desktop/twink/mp_os/build/logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : logger/server_logger/CMakeFiles/mp_os_lggr_srvr_lggr.dir/depend

