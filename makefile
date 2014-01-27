CC = g++
LIBS = freetype2 glew libxml++-2.6 openal #freealut
CFLAGS = -g -c `pkg-config --cflags $(LIBS)`
LDFLAGS = -Wall -lGL -lpng -lglfw -lBox2D -lxml2 `pkg-config --libs $(LIBS)`

BUILD_DIR = bin
SOURCE_DIR = sources
OBJECT_DIR = objects

SOURCE_DIRS = $(sort $(dir $(wildcard $(SOURCE_DIR)/ $(SOURCE_DIR)/*/ $(SOURCE_DIR)/*/*/)))
OBJECT_DIRS = $(patsubst $(SOURCE_DIR)%, $(OBJECT_DIR)%, $(SOURCE_DIRS))

SOURCE_FILES = $(filter %.cpp, $(wildcard $(addsuffix *.cpp, $(SOURCE_DIRS))))
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

TARGET = $(BUILD_DIR)/ce

.PHONY: build rebuild clean

build: $(TARGET)
	@cp -r res/* $(BUILD_DIR)/
	@echo $(TARGET) successfully built!

rebuild: clean build

$(TARGET): $(OBJECT_DIRS) $(BUILD_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECT_DIRS) $(BUILD_DIR):
	@mkdir $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BUILD_DIR)
