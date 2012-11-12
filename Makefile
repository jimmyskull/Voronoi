##############################################################
#			   CMake Project Wrapper Makefile			   #
############################################################## 

SHELL := /bin/bash
RM	:= rm -rf

all: ./build/Makefile
	@ $(MAKE) --no-print-directory -C build

./build/Makefile: ./build ./lib
	@ (cd build >/dev/null 2>&1 && cmake ..)

./build:
	@ ./configure

./lib:
	@ mkdir lib

clean:
	@- (cd build >/dev/null 2>&1 && cmake .. >/dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) ./build/Makefile
	@- $(RM) ./build/src
	@- $(RM) ./build/test
	@- $(RM) ./build/CMake*
	@- $(RM) ./build/cmake.*
	@- $(RM) ./build/*.cmake
	@- $(RM) ./build/*.txt
	@- $(RM) ./build/thirdparty
	@- $(RM) -R ./build
	@- $(RM) ./docs/*.html
	@- $(RM) ./docs/*.css
	@- $(RM) ./docs/*.png
	@- $(RM) ./docs/*.jpg
	@- $(RM) ./docs/*.gif
	@- $(RM) ./docs/*.tiff
	@- $(RM) ./docs/*.php
	@- $(RM) ./docs/search
	@- $(RM) ./docs/installdox

distclean:
	@- $(RM) -R build
	@- $(RM) -R bin
	@- $(RM) -R lib
	@- $(RM) -R thirdparty/gmock/gtest/CMakeFiles/

ifeq ($(findstring distclean,$(MAKECMDGOALS)),)

	$(MAKECMDGOALS): ./build/Makefile
	@ $(MAKE) -C build $(MAKECMDGOALS)

endif
