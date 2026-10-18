O ?= build
BUILDTYPE ?= release
CXX_COMPILER = g++
CXX_FLAGS = -Werror -Wall -Wfloat-equal -Iinc

ifeq ($(BUILDTYPE), debug)
  CXX_FLAGS += -O0 -g
else ifeq ($(BUILDTYPE), release)
  CXX_FLAGS += -O3
else
  $(error Unknown BUILDTYPE: ${BUILDTYPE})
endif

build-execution-time:
	mkdir -p $(O)
	$(CXX_COMPILER) $(CXX_FLAGS) -o $(O)/execution_time \
		$(CURDIR)/src/core/graham_scan.h \
		$(CURDIR)/src/core/graham_scan.cpp \
		$(CURDIR)/src/core/estimate.h \
		$(CURDIR)/src/core/estimate.cpp \
		$(CURDIR)/src/execution_time.cpp

build-validation:
	mkdir -p $(O)
	$(CXX_COMPILER) $(CXX_FLAGS) -o $(O)/validation \
		$(CURDIR)/src/core/estimate.h \
		$(CURDIR)/src/core/estimate.cpp \
		$(CURDIR)/src/core/graham_scan.h \
		$(CURDIR)/src/core/graham_scan.cpp \
		$(CURDIR)/src/validation.cpp

build-hull-sizes:
	mkdir -p $(O)
	$(CXX_COMPILER) $(CXX_FLAGS) -o $(O)/hull_sizes \
		$(CURDIR)/src/core/estimate.h \
		$(CURDIR)/src/core/estimate.cpp \
		$(CURDIR)/src/core/graham_scan.h \
		$(CURDIR)/src/core/graham_scan.cpp \
		$(CURDIR)/src/hull_sizes.cpp

build-all: build-execution-time build-validation build-hull-sizes

distclean:
	rm -rf $(O) *.pdf

