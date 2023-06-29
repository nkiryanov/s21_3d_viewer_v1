CMAKE_ROOT := .
BUILD_DIR := ../build
INSTALL_DIR := ../_install_dir

COVERAGE_REPORT_DIR := $(BUILD_DIR)/coverage
COVERAGE_REPORT := $(BUILD_DIR)/coverage.info
GCOV_TOOL := $(shell which gcov)

ifeq ($(shell uname), Darwin)
	GCOV_TOOL := /usr/bin/gcov
endif


SRC := $(shell find $(CMAKE_ROOT) \( -name "*.h" -o -name "*.hpp" \) -not -path "$(CMAKE_ROOT)/extern/*")
HEADERS := $(shell find $(CMAKE_ROOT) \( -name "*.h" -o -name "*.hpp" \) -not -path "$(CMAKE_ROOT)/extern/*")

DCMAKE_BUILD_TYPE := Debug

.PHONY: lint fmt build test tests echo clean install uninstall coverage_report gcov_report

all: install

lint:
	@clang-format --style=Google --Werror --dry-run $(SRC) $(HEADERS) && echo "Clagn-format passed"
	@cppcheck $(SRC) $(HEADERS)

fmt:
	@clang-format --style=Google --Werror -i $(SRC) $(HEADERS) && echo "Formatted"

build:
	cmake -S $(CMAKE_ROOT) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=${DCMAKE_BUILD_TYPE}
	cmake --build $(BUILD_DIR)

install: DCMAKE_BUILD_TYPE := Release
install: build
	cmake --build $(BUILD_DIR) --target install

uninstall:
	rm -rf $(INSTALL_DIR)

test: tests
tests: build
	make -C $(BUILD_DIR) test

coverage_report: tests
	lcov --capture --directory $(BUILD_DIR)/src/ --output-file=$(COVERAGE_REPORT) --gcov-tool $(GCOV_TOOL)

gcov_report: coverage_report
	genhtml $(COVERAGE_REPORT) --output-directory $(COVERAGE_REPORT_DIR)
	open $(COVERAGE_REPORT_DIR)/index.html

dvi: build
	@echo
	@echo "Trying to create docs with Doxygen"
	@echo "Please be sure it installed"
	@echo
	cd $(BUILD_DIR) && make docs
	cd $(BUILD_DIR) && open ./docs/html/index.html

dist:
	mkdir -p $(INSTALL_DIR)/dist
	tar cvzf $(INSTALL_DIR)/dist/archive.tgz \
	app cmake docs include src tests CMakeLists.txt Makefile

echo:
	@echo "HEADERS = $(HEADERS)"
	@echo "SRC = $(SRC)"

clean: uninstall
	rm -rf $(BUILD_DIR)

build_dir:
	mkdir -p $(BUILD_DIR)
