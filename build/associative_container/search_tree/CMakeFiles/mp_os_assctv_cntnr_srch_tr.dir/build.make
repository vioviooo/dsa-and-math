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
include associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/compiler_depend.make

# Include the progress variables for this target.
include associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/progress.make

# Include the compile flags for this target's objects.
include associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/flags.make

# Object files for target mp_os_assctv_cntnr_srch_tr
mp_os_assctv_cntnr_srch_tr_OBJECTS =

# External object files for target mp_os_assctv_cntnr_srch_tr
mp_os_assctv_cntnr_srch_tr_EXTERNAL_OBJECTS =

associative_container/search_tree/libmp_os_assctv_cntnr_srch_tr.a: associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/build.make
associative_container/search_tree/libmp_os_assctv_cntnr_srch_tr.a: associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/vioviooo/Desktop/twink/mp_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX static library libmp_os_assctv_cntnr_srch_tr.a"
	cd /Users/vioviooo/Desktop/twink/mp_os/build/associative_container/search_tree && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/cmake_clean_target.cmake
	cd /Users/vioviooo/Desktop/twink/mp_os/build/associative_container/search_tree && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/build: associative_container/search_tree/libmp_os_assctv_cntnr_srch_tr.a
.PHONY : associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/build

associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/clean:
	cd /Users/vioviooo/Desktop/twink/mp_os/build/associative_container/search_tree && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/cmake_clean.cmake
.PHONY : associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/clean

associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/depend:
	cd /Users/vioviooo/Desktop/twink/mp_os/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vioviooo/Desktop/twink/mp_os /Users/vioviooo/Desktop/twink/mp_os/associative_container/search_tree /Users/vioviooo/Desktop/twink/mp_os/build /Users/vioviooo/Desktop/twink/mp_os/build/associative_container/search_tree /Users/vioviooo/Desktop/twink/mp_os/build/associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : associative_container/search_tree/CMakeFiles/mp_os_assctv_cntnr_srch_tr.dir/depend

