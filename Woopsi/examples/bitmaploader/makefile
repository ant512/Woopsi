# Woopsi Makefile 											ex:set ts=4 sw=4:
# builds the Woopsi demo application, and should be sufficient for most
# needs - options are switched on/off with USE_ constants.

# set the texts that appear in the loader menus
TEXT1 		:= Demo Project
TEXT2 		:= using Woopsi
TEXT3 		:= woopsi.sourceforge.net

# 4-bit-deep bitmap file to use as icon in loader menus
ICON 		:= $(CURDIR)/../../../libwoopsi/icon/logo.bmp

#-------------------------------------------------------------------------------
# TARGET is the name of the output file
# BUILD is the directory where object files & intermediate files will be placed
# RELEASE is where the binary files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DEFINES is a set of -Dsym[=value] settings to pass to the compilers
#-------------------------------------------------------------------------------

TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
RELEASE     :=  Release
SOURCES		:=	source ../../libwoopsi/src ../../libwoopsi/src/fonts data ../../bonus/classes
INCLUDES	:=	include ../../libwoopsi/include ../../libwoopsi/include/fonts
DEFINES		:=

#-------------------------------------------------------------------------------
# If USE_WOOPSI is defined, compile/link against Woopsi
# If USE_FAT is defined, compile/link against libfat
#-------------------------------------------------------------------------------
#USE_WOOPSI	= 1
USE_FAT		= 1

# we do *not* do -o thing - WinterMute points out that this stops our
# resultant .nds from working on some loaders.  By default, ndstool puts boot
# code into the wifi-logospace and since its not visible anywhere else, its
# downright churlish for us to overwrite it.  If you want to, put it back, but
# its not the default.
# LOGO		:= -o $(CURDIR)/../data/logo_wifi.bmp

#===============================================================================
# options for code generation - unlikely you'll want to change this
#===============================================================================

ARCH		:=	-mthumb-interwork

# uncomment to generate debugging information
#MINUSG		:=	-g

# baseline flags for 
CFLAGS		:=	$(MINUSG) $(ARCH) \
				-Wall -O2 \
				-mcpu=arm9tdmi -mtune=arm9tdmi -fomit-frame-pointer \
				-ffast-math

CFLAGS		+=	-DARM9
ASFLAGS		:=	$(MINUSG) $(ARCH)
LDFLAGS		:=	$(MINUSG) $(ARCH) -mno-fpu

# enable dead-code stripping
CFLAGS		+=	-ffunction-sections -fdata-sections
LDFLAGS		+=	-Wl,--gc-sections

# link map (optional)
# LDFLAGS		+=	-Wl,-Map,$(OUTPUT).map

#===============================================================================
# no real need to edit anything past this point unless you need to add 
# additional rules for different file extensions
#===============================================================================

# disable all default rules
.SUFFIXES:

# ensure that we can see the compiler toolset
PATH 		:= $(DEVKITARM)/bin:$(PATH)

ifneq ($(BUILD),$(notdir $(CURDIR)))

# from here on, any symbols we want to propagate into the real build must be
# exported - all the lines above the 'ifneq' are evaluated in both cases...

# make sure selected options are visible in the recursive call...
#export USE_WOOPSI
#export USE_FAT

# directory name is used to construct the target.nds filename
export OUTPUT	:=	$(CURDIR)/$(RELEASE)/$(TARGET)

# name the GNU toolset
PREFIX			:=	arm-eabi-
export CC		:=	$(PREFIX)gcc
export CXX		:=	$(PREFIX)g++
export AR		:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy
export LD		:=	$(CXX)

# scan source directory for all possible source files (things we can turn into .o) 
CFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PCXFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pcx)))
BINFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))
PALFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pal)))
RAWFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.raw)))
MAPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.map)))
JPEGFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.jpg)))
MODFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.mod)))
GIFFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.gif)))
BMPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bmp)))

# build list of .o filenames 
export OFILES	:=	$(MAPFILES:.map=.o) $(RAWFILES:.raw=.o) $(PALFILES:.pal=.o)\
					$(BINFILES:.bin=.o) $(PCXFILES:.pcx=.o) $(JPEGFILES:.jpg=.o)\
					$(MODFILES:.mod=.o) $(GIFFILES:.gif=.o) $(BMPFILES:.bmp=.o)\
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
#-------------------------------------------------------------------------------
# standard magic to run the build from a build directory - this works because
# we poke all the source directories into VPATH
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

.PHONY: $(BUILD) clean rebuild
 
$(BUILD): $(CURDIR)/$(RELEASE)
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/makefile

$(CURDIR)/$(RELEASE):
	@-mkdir -p $(CURDIR)/$(RELEASE)
 
clean:
	@echo Cleaning... $(TARGET)
	@rm -fr $(BUILD) *.elf *.*ds* $(RELEASE)/*.*ds*
 
rebuild: clean $(BUILD)

#-------------------------------------------------------------------------------

else	# executing in the build directory
 
#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------

$(OUTPUT).ds.gba	: 	$(OUTPUT).nds
$(OUTPUT).nds		: 	$(OFILES)
 
#-------------------------------------------------------------------------------
# every directory mentioned in INCLUDES and LIBDIRS is expected to have a corresponding
# ./include directory which needs to be passed with -I to the C(++) compiler
#-------------------------------------------------------------------------------
LIBDIRS		+= $(DEVKITPRO)/libnds

# add all directories specified in INCLUDES and SOURCES
_INCS		+= $(foreach dir,$(INCLUDES),-I$(CURDIR)/../$(dir))
_INCS		+= $(foreach dir,$(SOURCES),-I$(CURDIR)/../$(dir))

# add all directories specified in LIBDIRS
_INCS		+= $(foreach dir,$(LIBDIRS),-I$(dir)/include)
_INCS		+= $(foreach dir,$(LIBDIRS),-I$(dir)/include/nds)

# and the build directory
_INCS		+= -I$(CURDIR)/$(BUILD)

#-------------------------------------------------------------------------------
# every directory mentioned in LIBDIRS is expected to have a corresponding
# ./lib directory which needs to be passed with -L to the linker
#-------------------------------------------------------------------------------
_LPATHS		:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

#-------------------------------------------------------------------------------
# look at the various options that have been selected and switch in the
# appropriate -D, -I, -l and -L options
#-------------------------------------------------------------------------------

_DEFS		= $(DEFINES)

# WIFI
# MIKMOD?

# use Woopsi
ifneq ($(USE_WOOPSI),)
_INCS		+= -I$(DEVKITPRO)/libwoopsi/include
_LIBS		+= -L$(DEVKITPRO)/libwoopsi/lib -lwoopsi
endif

# use FAT
ifneq ($(USE_FAT),)
_DEFS		+= -DUSE_FAT 
_INCS		+= -I$(DEVKITPRO)/libfat/include
_LIBS		+= -L$(DEVKITPRO)/libfat/lib -lfat 
endif

# and of course we need libnds
_LIBS		+= -lnds9

# this makes it easier to type the ndstool command line
_BANNER 		:= -b $(ICON) "$(TEXT1);$(TEXT2);$(TEXT3)"

#-------------------------------------------------------------------------------
# rule to build the alternate loader versions - unneeded on R4, etc
#
%.ds.gba: %.nds
	@dsbuild $< 
	@echo Built: $(notdir $@)
	@cp $(CURDIR)/../$(RELEASE)/$(notdir $@) ../$(RELEASE)/$(notdir $(OUTPUT)).sc.nds 
	@echo Built: $(notdir $(OUTPUT)).sc.nds

#-------------------------------------------------------------------------------
# rule to build a .nds binary - forces the link every time because we don't have proper
# dependencies in here...
%.nds: $(OFILES)
	@echo Linking...
	@$(LD) $(LDFLAGS) -specs=ds_arm9.specs $(OFILES) $(_LPATHS) $(_LIBS) -o $(TARGET).elf
	@$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	@ndstool -c $@ -9 $(TARGET).bin $(_ARM7BIN) $(LOGO) $(_BANNER)
	@echo Built: $(notdir $(OUTPUT)).nds
 
#-------------------------------------------------------------------------------
# rule to compile C++
#
%.o : %.cpp
	@echo Compiling $(notdir $<)
	@$(CXX) -MMD -MF $*.d -MP $(CFLAGS) $(_DEFS) $(_INCS) -c $< -o$@
 
#-------------------------------------------------------------------------------
# rule to compile C
#
%.o : %.c
	@echo Compiling $(notdir $<)
	@$(CC) -MMD -MF $*.d -MP $(CFLAGS) $(_DEFS) $(_INCS) -c $< -o$@
 
#-------------------------------------------------------------------------------
# rule to compile Assembler
#
%.o : %.s
	@echo Assembling $(notdir $<)
	@$(CC) -MMD -MF $*.d -MP $(ASFLAGS) $(_DEFS) $(_INCS) -c $< -o$@

#-------------------------------------------------------------------------------
# utility function to convert arbitrary data file to .o
#
define bin2o
	cp $(<) $(*).tmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
		--rename-section .data=.rodata \
		--redefine-sym _binary_$*_tmp_start=$*\
		--redefine-sym _binary_$*_tmp_end=$*_end\
		--redefine-sym _binary_$*_tmp_size=$*_size\
		$(*).tmp $(@)
	echo "extern const u8" $(*)"[];" > $(*).h
	echo "extern const u32" $(*)_size[]";" >> $(*).h
	rm $(*).tmp
endef
 
#-------------------------------------------------------------------------------
%.o	:	%.pcx
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)
 
#-------------------------------------------------------------------------------
%.o	:	%.bin
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)
 
#-------------------------------------------------------------------------------
%.o	:	%.raw
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)
 
#-------------------------------------------------------------------------------
%.o	:	%.pal
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)
 
#-------------------------------------------------------------------------------
%.o	:	%.map
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.o	:	%.mdl
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.o	:	%.jpg
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.o	:	%.mod
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.o	:	%.gif
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.o	:	%.bmp
#-------------------------------------------------------------------------------
	@echo Converting $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
# now include all the dependency files, one per object file - we assume that the
# .d file is next to the .o file, which is what our C/C++/Assembler rules are
# set up to do...
#
DEPENDS	:= $(OFILES:.o=.d)
-include $(DEPENDS) 

endif

################################################################################
# This makefile the following environment variables
#
# $(DEVKITPRO) points to your devkitPro environment
# $(DEVKITARM) points to the ARM toolset
#
# and
#
# Woopsi is installed in $(DEVKITPRO)/libwoopsi
# libfat is installed in $(DEVKITPRO)/libfat (instead of $(DEVKITPRO)/libnds)
# 
# In the future, this will support libefs, wifi, etc using similiar directory
# structures.
################################################################################
