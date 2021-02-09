SOURCE_DIR = src
SOURCES = $(wildcard *.cpp) $(wildcard src/*/*.cpp)
BUILD_DIR = bin
TEST_FILE_DIR = test
OUTPUT_FILE = Output
MEMTEST = valgrind
GXX = g++


default: build
.PHONY: default

ifeq ($(OS),Windows_NT) 
DETECTED_OS := Windows
else
DETECTED_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(DETECTED_OS),Windows)
BUILD_FILE = app.exe
TEST_FILES = ${shell dir ${TEST_FILE_DIR}}
endif

ifeq ($(DETECTED_OS),Linux)

BUILD_FILE = app.out
TEST_FILES = ${shell ls ${TEST_FILE_DIR}}


memtest: 
	${MEMTEST} ${BUILD_DIR}/${BUILD_FILE} ${BASIC_TEST_FILE}

endif


test: build
	echo "Running Tests"
	$(foreach var,$(TEST_FILES),${BUILD_DIR}/${BUILD_FILE} ${TEST_FILE_DIR}/${var};)
.PHONY: test

build: |buildDir
	${GXX} ${SOURCES} -o ${BUILD_DIR}/${BUILD_FILE}
.PHONY: build


clean:
	rm -rf ${BUILD_DIR} 
	rm -f ${OUTPUT_FILE}*
.PHONY: clean


buildDir:
	test -d ${BUILD_DIR} || mkdir ${BUILD_DIR}

.PHONY: buildDir

