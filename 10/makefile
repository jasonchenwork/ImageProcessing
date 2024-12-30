# 編譯器與選項
CXX = g++
CXXFLAGS = -fopenmp -g -I./include -std=c++11 -Wall -Wextra -fdiagnostics-color=always 
LDFLAGS = -fopenmp
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
TARGET = build/main

# 編譯與連結
all: build_dir $(TARGET)

build_dir:
	mkdir -p build

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

build/%.o: src/%.cpp| build_dir
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理建置檔案
clean:
	rm -rf build
