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
CMAKE_SOURCE_DIR = /home/benjamin/cs_345/lab1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/benjamin/cs_345/lab1

# Include any dependencies generated for this target.
include CMakeFiles/my_shell.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/my_shell.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/my_shell.dir/flags.make

CMakeFiles/my_shell.dir/os345.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345.c.o: os345.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345.c.o   -c /home/benjamin/cs_345/lab1/os345.c

CMakeFiles/my_shell.dir/os345.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345.c > CMakeFiles/my_shell.dir/os345.c.i

CMakeFiles/my_shell.dir/os345.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345.c -o CMakeFiles/my_shell.dir/os345.c.s

CMakeFiles/my_shell.dir/os345.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345.c.o.requires

CMakeFiles/my_shell.dir/os345.c.o.provides: CMakeFiles/my_shell.dir/os345.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345.c.o.provides

CMakeFiles/my_shell.dir/os345.c.o.provides.build: CMakeFiles/my_shell.dir/os345.c.o

CMakeFiles/my_shell.dir/os345fat.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345fat.c.o: os345fat.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345fat.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345fat.c.o   -c /home/benjamin/cs_345/lab1/os345fat.c

CMakeFiles/my_shell.dir/os345fat.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345fat.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345fat.c > CMakeFiles/my_shell.dir/os345fat.c.i

CMakeFiles/my_shell.dir/os345fat.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345fat.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345fat.c -o CMakeFiles/my_shell.dir/os345fat.c.s

CMakeFiles/my_shell.dir/os345fat.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345fat.c.o.requires

CMakeFiles/my_shell.dir/os345fat.c.o.provides: CMakeFiles/my_shell.dir/os345fat.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345fat.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345fat.c.o.provides

CMakeFiles/my_shell.dir/os345fat.c.o.provides.build: CMakeFiles/my_shell.dir/os345fat.c.o

CMakeFiles/my_shell.dir/os345lc3.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345lc3.c.o: os345lc3.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345lc3.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345lc3.c.o   -c /home/benjamin/cs_345/lab1/os345lc3.c

CMakeFiles/my_shell.dir/os345lc3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345lc3.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345lc3.c > CMakeFiles/my_shell.dir/os345lc3.c.i

CMakeFiles/my_shell.dir/os345lc3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345lc3.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345lc3.c -o CMakeFiles/my_shell.dir/os345lc3.c.s

CMakeFiles/my_shell.dir/os345lc3.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345lc3.c.o.requires

CMakeFiles/my_shell.dir/os345lc3.c.o.provides: CMakeFiles/my_shell.dir/os345lc3.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345lc3.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345lc3.c.o.provides

CMakeFiles/my_shell.dir/os345lc3.c.o.provides.build: CMakeFiles/my_shell.dir/os345lc3.c.o

CMakeFiles/my_shell.dir/os345p1.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345p1.c.o: os345p1.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345p1.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345p1.c.o   -c /home/benjamin/cs_345/lab1/os345p1.c

CMakeFiles/my_shell.dir/os345p1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345p1.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345p1.c > CMakeFiles/my_shell.dir/os345p1.c.i

CMakeFiles/my_shell.dir/os345p1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345p1.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345p1.c -o CMakeFiles/my_shell.dir/os345p1.c.s

CMakeFiles/my_shell.dir/os345p1.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345p1.c.o.requires

CMakeFiles/my_shell.dir/os345p1.c.o.provides: CMakeFiles/my_shell.dir/os345p1.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345p1.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345p1.c.o.provides

CMakeFiles/my_shell.dir/os345p1.c.o.provides.build: CMakeFiles/my_shell.dir/os345p1.c.o

CMakeFiles/my_shell.dir/os345p4.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345p4.c.o: os345p4.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345p4.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345p4.c.o   -c /home/benjamin/cs_345/lab1/os345p4.c

CMakeFiles/my_shell.dir/os345p4.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345p4.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345p4.c > CMakeFiles/my_shell.dir/os345p4.c.i

CMakeFiles/my_shell.dir/os345p4.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345p4.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345p4.c -o CMakeFiles/my_shell.dir/os345p4.c.s

CMakeFiles/my_shell.dir/os345p4.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345p4.c.o.requires

CMakeFiles/my_shell.dir/os345p4.c.o.provides: CMakeFiles/my_shell.dir/os345p4.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345p4.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345p4.c.o.provides

CMakeFiles/my_shell.dir/os345p4.c.o.provides.build: CMakeFiles/my_shell.dir/os345p4.c.o

CMakeFiles/my_shell.dir/os345park.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345park.c.o: os345park.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345park.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345park.c.o   -c /home/benjamin/cs_345/lab1/os345park.c

CMakeFiles/my_shell.dir/os345park.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345park.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345park.c > CMakeFiles/my_shell.dir/os345park.c.i

CMakeFiles/my_shell.dir/os345park.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345park.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345park.c -o CMakeFiles/my_shell.dir/os345park.c.s

CMakeFiles/my_shell.dir/os345park.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345park.c.o.requires

CMakeFiles/my_shell.dir/os345park.c.o.provides: CMakeFiles/my_shell.dir/os345park.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345park.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345park.c.o.provides

CMakeFiles/my_shell.dir/os345park.c.o.provides.build: CMakeFiles/my_shell.dir/os345park.c.o

CMakeFiles/my_shell.dir/os345signals.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345signals.c.o: os345signals.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345signals.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345signals.c.o   -c /home/benjamin/cs_345/lab1/os345signals.c

CMakeFiles/my_shell.dir/os345signals.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345signals.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345signals.c > CMakeFiles/my_shell.dir/os345signals.c.i

CMakeFiles/my_shell.dir/os345signals.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345signals.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345signals.c -o CMakeFiles/my_shell.dir/os345signals.c.s

CMakeFiles/my_shell.dir/os345signals.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345signals.c.o.requires

CMakeFiles/my_shell.dir/os345signals.c.o.provides: CMakeFiles/my_shell.dir/os345signals.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345signals.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345signals.c.o.provides

CMakeFiles/my_shell.dir/os345signals.c.o.provides.build: CMakeFiles/my_shell.dir/os345signals.c.o

CMakeFiles/my_shell.dir/os345p2.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345p2.c.o: os345p2.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345p2.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345p2.c.o   -c /home/benjamin/cs_345/lab1/os345p2.c

CMakeFiles/my_shell.dir/os345p2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345p2.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345p2.c > CMakeFiles/my_shell.dir/os345p2.c.i

CMakeFiles/my_shell.dir/os345p2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345p2.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345p2.c -o CMakeFiles/my_shell.dir/os345p2.c.s

CMakeFiles/my_shell.dir/os345p2.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345p2.c.o.requires

CMakeFiles/my_shell.dir/os345p2.c.o.provides: CMakeFiles/my_shell.dir/os345p2.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345p2.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345p2.c.o.provides

CMakeFiles/my_shell.dir/os345p2.c.o.provides.build: CMakeFiles/my_shell.dir/os345p2.c.o

CMakeFiles/my_shell.dir/os345p5.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345p5.c.o: os345p5.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345p5.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345p5.c.o   -c /home/benjamin/cs_345/lab1/os345p5.c

CMakeFiles/my_shell.dir/os345p5.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345p5.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345p5.c > CMakeFiles/my_shell.dir/os345p5.c.i

CMakeFiles/my_shell.dir/os345p5.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345p5.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345p5.c -o CMakeFiles/my_shell.dir/os345p5.c.s

CMakeFiles/my_shell.dir/os345p5.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345p5.c.o.requires

CMakeFiles/my_shell.dir/os345p5.c.o.provides: CMakeFiles/my_shell.dir/os345p5.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345p5.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345p5.c.o.provides

CMakeFiles/my_shell.dir/os345p5.c.o.provides.build: CMakeFiles/my_shell.dir/os345p5.c.o

CMakeFiles/my_shell.dir/os345interrupts.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345interrupts.c.o: os345interrupts.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345interrupts.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345interrupts.c.o   -c /home/benjamin/cs_345/lab1/os345interrupts.c

CMakeFiles/my_shell.dir/os345interrupts.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345interrupts.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345interrupts.c > CMakeFiles/my_shell.dir/os345interrupts.c.i

CMakeFiles/my_shell.dir/os345interrupts.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345interrupts.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345interrupts.c -o CMakeFiles/my_shell.dir/os345interrupts.c.s

CMakeFiles/my_shell.dir/os345interrupts.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345interrupts.c.o.requires

CMakeFiles/my_shell.dir/os345interrupts.c.o.provides: CMakeFiles/my_shell.dir/os345interrupts.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345interrupts.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345interrupts.c.o.provides

CMakeFiles/my_shell.dir/os345interrupts.c.o.provides.build: CMakeFiles/my_shell.dir/os345interrupts.c.o

CMakeFiles/my_shell.dir/os345mmu.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345mmu.c.o: os345mmu.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345mmu.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345mmu.c.o   -c /home/benjamin/cs_345/lab1/os345mmu.c

CMakeFiles/my_shell.dir/os345mmu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345mmu.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345mmu.c > CMakeFiles/my_shell.dir/os345mmu.c.i

CMakeFiles/my_shell.dir/os345mmu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345mmu.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345mmu.c -o CMakeFiles/my_shell.dir/os345mmu.c.s

CMakeFiles/my_shell.dir/os345mmu.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345mmu.c.o.requires

CMakeFiles/my_shell.dir/os345mmu.c.o.provides: CMakeFiles/my_shell.dir/os345mmu.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345mmu.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345mmu.c.o.provides

CMakeFiles/my_shell.dir/os345mmu.c.o.provides.build: CMakeFiles/my_shell.dir/os345mmu.c.o

CMakeFiles/my_shell.dir/os345p3.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345p3.c.o: os345p3.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345p3.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345p3.c.o   -c /home/benjamin/cs_345/lab1/os345p3.c

CMakeFiles/my_shell.dir/os345p3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345p3.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345p3.c > CMakeFiles/my_shell.dir/os345p3.c.i

CMakeFiles/my_shell.dir/os345p3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345p3.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345p3.c -o CMakeFiles/my_shell.dir/os345p3.c.s

CMakeFiles/my_shell.dir/os345p3.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345p3.c.o.requires

CMakeFiles/my_shell.dir/os345p3.c.o.provides: CMakeFiles/my_shell.dir/os345p3.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345p3.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345p3.c.o.provides

CMakeFiles/my_shell.dir/os345p3.c.o.provides.build: CMakeFiles/my_shell.dir/os345p3.c.o

CMakeFiles/my_shell.dir/os345p6.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345p6.c.o: os345p6.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345p6.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345p6.c.o   -c /home/benjamin/cs_345/lab1/os345p6.c

CMakeFiles/my_shell.dir/os345p6.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345p6.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345p6.c > CMakeFiles/my_shell.dir/os345p6.c.i

CMakeFiles/my_shell.dir/os345p6.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345p6.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345p6.c -o CMakeFiles/my_shell.dir/os345p6.c.s

CMakeFiles/my_shell.dir/os345p6.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345p6.c.o.requires

CMakeFiles/my_shell.dir/os345p6.c.o.provides: CMakeFiles/my_shell.dir/os345p6.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345p6.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345p6.c.o.provides

CMakeFiles/my_shell.dir/os345p6.c.o.provides.build: CMakeFiles/my_shell.dir/os345p6.c.o

CMakeFiles/my_shell.dir/os345semaphores.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345semaphores.c.o: os345semaphores.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_14)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345semaphores.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345semaphores.c.o   -c /home/benjamin/cs_345/lab1/os345semaphores.c

CMakeFiles/my_shell.dir/os345semaphores.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345semaphores.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345semaphores.c > CMakeFiles/my_shell.dir/os345semaphores.c.i

CMakeFiles/my_shell.dir/os345semaphores.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345semaphores.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345semaphores.c -o CMakeFiles/my_shell.dir/os345semaphores.c.s

CMakeFiles/my_shell.dir/os345semaphores.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345semaphores.c.o.requires

CMakeFiles/my_shell.dir/os345semaphores.c.o.provides: CMakeFiles/my_shell.dir/os345semaphores.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345semaphores.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345semaphores.c.o.provides

CMakeFiles/my_shell.dir/os345semaphores.c.o.provides.build: CMakeFiles/my_shell.dir/os345semaphores.c.o

CMakeFiles/my_shell.dir/os345tasks.c.o: CMakeFiles/my_shell.dir/flags.make
CMakeFiles/my_shell.dir/os345tasks.c.o: os345tasks.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benjamin/cs_345/lab1/CMakeFiles $(CMAKE_PROGRESS_15)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/my_shell.dir/os345tasks.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/my_shell.dir/os345tasks.c.o   -c /home/benjamin/cs_345/lab1/os345tasks.c

CMakeFiles/my_shell.dir/os345tasks.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/my_shell.dir/os345tasks.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/benjamin/cs_345/lab1/os345tasks.c > CMakeFiles/my_shell.dir/os345tasks.c.i

CMakeFiles/my_shell.dir/os345tasks.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/my_shell.dir/os345tasks.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/benjamin/cs_345/lab1/os345tasks.c -o CMakeFiles/my_shell.dir/os345tasks.c.s

CMakeFiles/my_shell.dir/os345tasks.c.o.requires:
.PHONY : CMakeFiles/my_shell.dir/os345tasks.c.o.requires

CMakeFiles/my_shell.dir/os345tasks.c.o.provides: CMakeFiles/my_shell.dir/os345tasks.c.o.requires
	$(MAKE) -f CMakeFiles/my_shell.dir/build.make CMakeFiles/my_shell.dir/os345tasks.c.o.provides.build
.PHONY : CMakeFiles/my_shell.dir/os345tasks.c.o.provides

CMakeFiles/my_shell.dir/os345tasks.c.o.provides.build: CMakeFiles/my_shell.dir/os345tasks.c.o

# Object files for target my_shell
my_shell_OBJECTS = \
"CMakeFiles/my_shell.dir/os345.c.o" \
"CMakeFiles/my_shell.dir/os345fat.c.o" \
"CMakeFiles/my_shell.dir/os345lc3.c.o" \
"CMakeFiles/my_shell.dir/os345p1.c.o" \
"CMakeFiles/my_shell.dir/os345p4.c.o" \
"CMakeFiles/my_shell.dir/os345park.c.o" \
"CMakeFiles/my_shell.dir/os345signals.c.o" \
"CMakeFiles/my_shell.dir/os345p2.c.o" \
"CMakeFiles/my_shell.dir/os345p5.c.o" \
"CMakeFiles/my_shell.dir/os345interrupts.c.o" \
"CMakeFiles/my_shell.dir/os345mmu.c.o" \
"CMakeFiles/my_shell.dir/os345p3.c.o" \
"CMakeFiles/my_shell.dir/os345p6.c.o" \
"CMakeFiles/my_shell.dir/os345semaphores.c.o" \
"CMakeFiles/my_shell.dir/os345tasks.c.o"

# External object files for target my_shell
my_shell_EXTERNAL_OBJECTS =

my_shell: CMakeFiles/my_shell.dir/os345.c.o
my_shell: CMakeFiles/my_shell.dir/os345fat.c.o
my_shell: CMakeFiles/my_shell.dir/os345lc3.c.o
my_shell: CMakeFiles/my_shell.dir/os345p1.c.o
my_shell: CMakeFiles/my_shell.dir/os345p4.c.o
my_shell: CMakeFiles/my_shell.dir/os345park.c.o
my_shell: CMakeFiles/my_shell.dir/os345signals.c.o
my_shell: CMakeFiles/my_shell.dir/os345p2.c.o
my_shell: CMakeFiles/my_shell.dir/os345p5.c.o
my_shell: CMakeFiles/my_shell.dir/os345interrupts.c.o
my_shell: CMakeFiles/my_shell.dir/os345mmu.c.o
my_shell: CMakeFiles/my_shell.dir/os345p3.c.o
my_shell: CMakeFiles/my_shell.dir/os345p6.c.o
my_shell: CMakeFiles/my_shell.dir/os345semaphores.c.o
my_shell: CMakeFiles/my_shell.dir/os345tasks.c.o
my_shell: CMakeFiles/my_shell.dir/build.make
my_shell: CMakeFiles/my_shell.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable my_shell"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_shell.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/my_shell.dir/build: my_shell
.PHONY : CMakeFiles/my_shell.dir/build

CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345fat.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345lc3.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345p1.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345p4.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345park.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345signals.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345p2.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345p5.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345interrupts.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345mmu.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345p3.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345p6.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345semaphores.c.o.requires
CMakeFiles/my_shell.dir/requires: CMakeFiles/my_shell.dir/os345tasks.c.o.requires
.PHONY : CMakeFiles/my_shell.dir/requires

CMakeFiles/my_shell.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/my_shell.dir/cmake_clean.cmake
.PHONY : CMakeFiles/my_shell.dir/clean

CMakeFiles/my_shell.dir/depend:
	cd /home/benjamin/cs_345/lab1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/benjamin/cs_345/lab1 /home/benjamin/cs_345/lab1 /home/benjamin/cs_345/lab1 /home/benjamin/cs_345/lab1 /home/benjamin/cs_345/lab1/CMakeFiles/my_shell.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/my_shell.dir/depend
