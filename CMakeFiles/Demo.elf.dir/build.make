# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /opt/cmake-3.7.2-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.7.2-Linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /DIST/home/lab_espl_stud10/khanh_workspace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /DIST/home/lab_espl_stud10/khanh_workspace

# Include any dependencies generated for this target.
include CMakeFiles/Demo.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Demo.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Demo.elf.dir/flags.make

# Object files for target Demo.elf
Demo_elf_OBJECTS =

# External object files for target Demo.elf
Demo_elf_EXTERNAL_OBJECTS = \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/Button.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/Demo.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/Map.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/StateMachine.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/display.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/ghostBlue.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/ghostMulti.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/ghostMultiReceive.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/ghostOrange.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/ghostPink.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/ghostRed.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/pac_man.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/code/pacmanMulti.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/gdisp_lld_ILI9341.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/ginput_lld_mouse.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/ParTest.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/port.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/timertest.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/system_stm32f4xx.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/ESPL_functions.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/usrlib.dir/Libraries/usr/startup_stm32f429_439xx.S.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/list.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/queue.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/tasks.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/timers.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/portable/MemMang/heap_1.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Demo/Common/Minimal/flash.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/misc.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma2d.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_ltdc.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmutilitieslib.dir/Libraries/STM32F4/Utilities/STM32_EVAL/STM32F429I-Discovery/stm32f429i_discovery_ioe.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmutilitieslib.dir/Libraries/STM32F4/Utilities/STM32_EVAL/STM32F429I-Discovery/stm32f429i_discovery_lcd.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/stmutilitieslib.dir/Libraries/STM32F4/Utilities/STM32_EVAL/STM32F429I-Discovery/stm32f429i_discovery_sdram.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gfx.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdriver/gdriver.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp_fonts.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_encoding.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_font.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_justify.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_scaledfont.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_rlefont.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gevent/gevent.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/ginput/ginput.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/ginput/ginput_mouse.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gos/gos_freertos.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gtimer/gtimer.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gwin/gwin_console.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gwin/gwin.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gwin/gwin_wm.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp_image.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp_image_bmp.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gfile/gfile.c.o" \
"/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gfile/gfile_fs_rom.c.o"

Demo.elf: CMakeFiles/usrlib.dir/code/Button.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/Demo.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/Map.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/StateMachine.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/display.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/ghostBlue.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/ghostMulti.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/ghostMultiReceive.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/ghostOrange.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/ghostPink.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/ghostRed.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/pac_man.c.o
Demo.elf: CMakeFiles/usrlib.dir/code/pacmanMulti.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/gdisp_lld_ILI9341.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/ginput_lld_mouse.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/ParTest.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/port.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/timertest.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/system_stm32f4xx.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/ESPL_functions.c.o
Demo.elf: CMakeFiles/usrlib.dir/Libraries/usr/startup_stm32f429_439xx.S.o
Demo.elf: CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/list.c.o
Demo.elf: CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/queue.c.o
Demo.elf: CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/tasks.c.o
Demo.elf: CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/timers.c.o
Demo.elf: CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Source/portable/MemMang/heap_1.c.o
Demo.elf: CMakeFiles/rtoslib.dir/Libraries/FreeRTOS/FreeRTOS/Demo/Common/Minimal/flash.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/misc.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma2d.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_ltdc.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c.o
Demo.elf: CMakeFiles/stmperipheralslib.dir/Libraries/STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c.o
Demo.elf: CMakeFiles/stmutilitieslib.dir/Libraries/STM32F4/Utilities/STM32_EVAL/STM32F429I-Discovery/stm32f429i_discovery_ioe.c.o
Demo.elf: CMakeFiles/stmutilitieslib.dir/Libraries/STM32F4/Utilities/STM32_EVAL/STM32F429I-Discovery/stm32f429i_discovery_lcd.c.o
Demo.elf: CMakeFiles/stmutilitieslib.dir/Libraries/STM32F4/Utilities/STM32_EVAL/STM32F429I-Discovery/stm32f429i_discovery_sdram.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gfx.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdriver/gdriver.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp_fonts.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_encoding.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_font.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_justify.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_scaledfont.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/mcufont/mf_rlefont.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gevent/gevent.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/ginput/ginput.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/ginput/ginput_mouse.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gos/gos_freertos.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gtimer/gtimer.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gwin/gwin_console.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gwin/gwin.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gwin/gwin_wm.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp_image.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gdisp/gdisp_image_bmp.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gfile/gfile.c.o
Demo.elf: CMakeFiles/ugfxlib.dir/Libraries/ugfx/src/gfile/gfile_fs_rom.c.o
Demo.elf: CMakeFiles/Demo.elf.dir/build.make
Demo.elf: CMakeFiles/Demo.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking C executable Demo.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Demo.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Demo.elf.dir/build: Demo.elf

.PHONY : CMakeFiles/Demo.elf.dir/build

CMakeFiles/Demo.elf.dir/requires:

.PHONY : CMakeFiles/Demo.elf.dir/requires

CMakeFiles/Demo.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Demo.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Demo.elf.dir/clean

CMakeFiles/Demo.elf.dir/depend:
	cd /DIST/home/lab_espl_stud10/khanh_workspace && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /DIST/home/lab_espl_stud10/khanh_workspace /DIST/home/lab_espl_stud10/khanh_workspace /DIST/home/lab_espl_stud10/khanh_workspace /DIST/home/lab_espl_stud10/khanh_workspace /DIST/home/lab_espl_stud10/khanh_workspace/CMakeFiles/Demo.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Demo.elf.dir/depend

