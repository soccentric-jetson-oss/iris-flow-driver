# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile - top-level orchestrator for iris-flow-driver
# Iris Flow — Vision Pipelining Core for Jetson AGX Orin
#
# Targets: module, lib, test, docker, pkg, static analysis, clean
#

DRV_NAME    := iris-flow
DRV_LIBNAME := iris-flow
DRV_VERSION := $(shell cat VERSION 2>/dev/null || echo "0.1.0")

SRC_DIR     := src
LIB_DIR     := lib
TEST_DIR    := test
BUILD_DIR   := build
DIST_DIR    := dist

CC          ?= gcc
LD          ?= ld
AR          ?= ar
CFLAGS      ?= -O2 -Wall -Wextra
KDIR        ?= /lib/modules/$(shell uname -r)/build
ARCH        ?= $(shell uname -m | sed s/aarch64/arm64/ | sed s/x86_64/x86_64/)
CROSS_COMPILE ?=
DESTDIR     ?=
PREFIX      ?= /usr
INCLUDES    := -I$(LIB_DIR)/include -Iinclude

.PHONY: all module lib test stress clean distclean help

all: module lib test

module:
	$(MAKE) -C $(KDIR) M=$(CURDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

module_clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

LIB_SRC  := $(LIB_DIR)/src/lib$(DRV_LIBNAME).c
LIB_OBJ  := $(BUILD_DIR)/lib$(DRV_LIBNAME).o
LIB_SO   := $(BUILD_DIR)/lib$(DRV_LIBNAME).so
LIB_A    := $(BUILD_DIR)/lib$(DRV_LIBNAME).a
LIB_MAJOR := 1

$(LIB_OBJ): $(LIB_SRC) $(LIB_DIR)/include/lib$(DRV_LIBNAME).h include/iris_flow_uapi.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC $(INCLUDES) -c -o $@ $<

$(LIB_SO): $(LIB_OBJ)
	$(CC) $(LDFLAGS) -shared -Wl,-soname,lib$(DRV_LIBNAME).so.$(LIB_MAJOR) -o $@ $^
	ln -sf lib$(DRV_LIBNAME).so $(BUILD_DIR)/lib$(DRV_LIBNAME).so.$(LIB_MAJOR)

$(LIB_A): $(LIB_OBJ)
	$(AR) rcs $@ $^

lib: $(LIB_SO) $(LIB_A)

TEST_BIN   := $(BUILD_DIR)/$(DRV_LIBNAME)_test
STRESS_BIN := $(BUILD_DIR)/$(DRV_LIBNAME)_stress

$(TEST_BIN): $(TEST_DIR)/$(DRV_LIBNAME)_test.c $(LIB_A)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< -L$(BUILD_DIR) -l$(DRV_LIBNAME) -lpthread

$(STRESS_BIN): $(TEST_DIR)/$(DRV_LIBNAME)_stress.c $(LIB_A)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< -L$(BUILD_DIR) -l$(DRV_LIBNAME) -lpthread

test: $(TEST_BIN)
	LD_LIBRARY_PATH=$(BUILD_DIR) $(TEST_BIN)

stress: $(STRESS_BIN)
	LD_LIBRARY_PATH=$(BUILD_DIR) $(STRESS_BIN)

clean: module_clean
	rm -rf $(BUILD_DIR) $(DIST_DIR) Module.symvers modules.order

distclean: clean
	rm -rf .*.cmd *.o *.ko *.mod *.mod.c

help:
	@echo "Iris Flow Driver $(DRV_VERSION)"
	@echo "Targets: all, module, lib, test, stress, clean, distclean"

# ── Formatting & Linting ────────────────────────────────────────────
CLANG_FILES := src/*.c include/*.h lib/src/*.c lib/include/*.h
format:
	@clang-format -i $(CLANG_FILES) 2>/dev/null || true
format-check:
	@clang-format --dry-run --Werror $(CLANG_FILES) 2>/dev/null || echo "WARNING: clang-format not available"
lint:
	@cppcheck --enable=all --inconclusive --suppress=missingIncludeSystem src/*.c lib/src/*.c test/*.c 2>/dev/null || echo "WARNING: cppcheck not available"
doc:
	@doxygen docs/Doxyfile 2>/dev/null || echo "WARNING: doxygen not available"
analyze: format-check lint
