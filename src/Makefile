CMAKE_ROOT := .
BUILD_DIR := ../build

SRC := $(shell find $(CMAKE_ROOT) -name "*.c" -o -name "*.cpp")
HEADERS := $(shell find $(CMAKE_ROOT) -name "*.h" -o -name "*.hpp")

INSTALL_DIR := $(BUILD_DIR)/_install_dir

COVERAGE_REPORT := $(BUILD_DIR)/coverage.info
COVERAGE_REPORT_DIR := $(BUILD_DIR)/coverage

DCMAKE_BUILD_TYPE := Debug

.PHONY: lint fmt build test tests echo clean install uninstall coverage_report gcov_report

all: install

lint:
	@clang-format --style=Google --Werror --dry-run $(SRC) $(HEADERS) && echo "Clagn-format passed"
	@cppcheck $(SRC) $(HEADERS)

fmt:
	@clang-format --style=Google --Werror -i $(SRC) $(HEADERS) && echo "Formatted"

build: build_dir
	cmake -S $(CMAKE_ROOT) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=${DCMAKE_BUILD_TYPE}
	cd $(BUILD_DIR) && cmake --build .

install: DCMAKE_BUILD_TYPE := Release
install: build
	cd $(BUILD_DIR) && cmake --install .

uninstall:
	rm -rf $(INSTALL_DIR)

test: tests
tests: build
	make -C $(BUILD_DIR) test

valgrind_tests: tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(TESTS_EXEC)

coverage_report: tests
	lcov --capture --directory $(BUILD_DIR)/src/ --output-file=$(COVERAGE_REPORT)

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
	@echo "HEADERS = ${HEADERS}"

clean: uninstall
	rm -rf $(BUILD_DIR)/*

build_dir:
	mkdir -p $(BUILD_DIR)
