CC = g++
CFLAGS = -c -g
LDFLAGS = -Wall -lGL -lpng -lglfw -lBox2D -lxml2

SOURCE_DIR = sources
OBJECT_DIR = objects

SOURCE_DIRS = $(sort $(dir $(wildcard $(SOURCE_DIR)/ $(SOURCE_DIR)/*/ $(SOURCE_DIR)/*/*/)))
OBJECT_DIRS = $(patsubst $(SOURCE_DIR)%, $(OBJECT_DIR)%, $(SOURCE_DIRS))

SOURCE_FILES = $(filter %.cpp, $(wildcard $(addsuffix *.cpp, $(SOURCE_DIRS))))
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

TARGET = ce

.PHONY: build rebuild clean

build: $(TARGET)
	@echo $(TARGET) successfully built!

rebuild: clean build

$(TARGET): $(OBJECT_DIRS) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECT_DIRS):
	@mkdir $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(TARGET)
