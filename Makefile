SOURCE_DIR = src
SOURCES = $(wildcard *.cpp) $(wildcard src/*/*.cpp)
BUILD_DIR = bin
TEST_FILE = test/Input.txt
GXX = g++

ifeq ($(OS),Windows_NT) 
	detected_OS := Windows
else
	detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(detected_OS),Windows)
	BUILD_FILE = app.exe
endif

ifeq ($(detected_OS),Linux)
	BUILD_FILE = app.out
endif

test: build
	${BUILD_DIR}/${BUILD_FILE} ${TEST_FILE}
.PHONY: test

build: |buildDir
	${GXX} ${SOURCES} -o ${BUILD_DIR}/${BUILD_FILE}
.PHONY: build


clean:
	rm -r ${BUILD_DIR}
	rm listFile.txt
.PHONY: clean


buildDir:
	test -d ${BUILD_DIR} || mkdir ${BUILD_DIR}

.PHONY: buildDir