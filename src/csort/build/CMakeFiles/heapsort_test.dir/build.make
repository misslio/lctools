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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/src/github_repo/lctools/src/csort

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/src/github_repo/lctools/src/csort/build

# Include any dependencies generated for this target.
include CMakeFiles/heapsort_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/heapsort_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/heapsort_test.dir/flags.make

CMakeFiles/heapsort_test.dir/Heapsort.o: CMakeFiles/heapsort_test.dir/flags.make
CMakeFiles/heapsort_test.dir/Heapsort.o: ../Heapsort.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/src/github_repo/lctools/src/csort/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/heapsort_test.dir/Heapsort.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/heapsort_test.dir/Heapsort.o   -c /mnt/hgfs/src/github_repo/lctools/src/csort/Heapsort.c

CMakeFiles/heapsort_test.dir/Heapsort.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/heapsort_test.dir/Heapsort.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/src/github_repo/lctools/src/csort/Heapsort.c > CMakeFiles/heapsort_test.dir/Heapsort.i

CMakeFiles/heapsort_test.dir/Heapsort.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/heapsort_test.dir/Heapsort.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/src/github_repo/lctools/src/csort/Heapsort.c -o CMakeFiles/heapsort_test.dir/Heapsort.s

CMakeFiles/heapsort_test.dir/Heapsort.o.requires:
.PHONY : CMakeFiles/heapsort_test.dir/Heapsort.o.requires

CMakeFiles/heapsort_test.dir/Heapsort.o.provides: CMakeFiles/heapsort_test.dir/Heapsort.o.requires
	$(MAKE) -f CMakeFiles/heapsort_test.dir/build.make CMakeFiles/heapsort_test.dir/Heapsort.o.provides.build
.PHONY : CMakeFiles/heapsort_test.dir/Heapsort.o.provides

CMakeFiles/heapsort_test.dir/Heapsort.o.provides.build: CMakeFiles/heapsort_test.dir/Heapsort.o

CMakeFiles/heapsort_test.dir/Heapsort_test.o: CMakeFiles/heapsort_test.dir/flags.make
CMakeFiles/heapsort_test.dir/Heapsort_test.o: ../Heapsort_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/src/github_repo/lctools/src/csort/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/heapsort_test.dir/Heapsort_test.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/heapsort_test.dir/Heapsort_test.o   -c /mnt/hgfs/src/github_repo/lctools/src/csort/Heapsort_test.c

CMakeFiles/heapsort_test.dir/Heapsort_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/heapsort_test.dir/Heapsort_test.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/src/github_repo/lctools/src/csort/Heapsort_test.c > CMakeFiles/heapsort_test.dir/Heapsort_test.i

CMakeFiles/heapsort_test.dir/Heapsort_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/heapsort_test.dir/Heapsort_test.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/src/github_repo/lctools/src/csort/Heapsort_test.c -o CMakeFiles/heapsort_test.dir/Heapsort_test.s

CMakeFiles/heapsort_test.dir/Heapsort_test.o.requires:
.PHONY : CMakeFiles/heapsort_test.dir/Heapsort_test.o.requires

CMakeFiles/heapsort_test.dir/Heapsort_test.o.provides: CMakeFiles/heapsort_test.dir/Heapsort_test.o.requires
	$(MAKE) -f CMakeFiles/heapsort_test.dir/build.make CMakeFiles/heapsort_test.dir/Heapsort_test.o.provides.build
.PHONY : CMakeFiles/heapsort_test.dir/Heapsort_test.o.provides

CMakeFiles/heapsort_test.dir/Heapsort_test.o.provides.build: CMakeFiles/heapsort_test.dir/Heapsort_test.o

# Object files for target heapsort_test
heapsort_test_OBJECTS = \
"CMakeFiles/heapsort_test.dir/Heapsort.o" \
"CMakeFiles/heapsort_test.dir/Heapsort_test.o"

# External object files for target heapsort_test
heapsort_test_EXTERNAL_OBJECTS =

bin/heapsort_test: CMakeFiles/heapsort_test.dir/Heapsort.o
bin/heapsort_test: CMakeFiles/heapsort_test.dir/Heapsort_test.o
bin/heapsort_test: CMakeFiles/heapsort_test.dir/build.make
bin/heapsort_test: CMakeFiles/heapsort_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin/heapsort_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/heapsort_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/heapsort_test.dir/build: bin/heapsort_test
.PHONY : CMakeFiles/heapsort_test.dir/build

CMakeFiles/heapsort_test.dir/requires: CMakeFiles/heapsort_test.dir/Heapsort.o.requires
CMakeFiles/heapsort_test.dir/requires: CMakeFiles/heapsort_test.dir/Heapsort_test.o.requires
.PHONY : CMakeFiles/heapsort_test.dir/requires

CMakeFiles/heapsort_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/heapsort_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/heapsort_test.dir/clean

CMakeFiles/heapsort_test.dir/depend:
	cd /mnt/hgfs/src/github_repo/lctools/src/csort/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/src/github_repo/lctools/src/csort /mnt/hgfs/src/github_repo/lctools/src/csort /mnt/hgfs/src/github_repo/lctools/src/csort/build /mnt/hgfs/src/github_repo/lctools/src/csort/build /mnt/hgfs/src/github_repo/lctools/src/csort/build/CMakeFiles/heapsort_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/heapsort_test.dir/depend

