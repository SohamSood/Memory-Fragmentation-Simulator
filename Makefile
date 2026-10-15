CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = memory_simulator
SRC_DIR = src
TEST_DIR = tests

all: $(TARGET)

$(TARGET): $(SRC_DIR)/main.c $(SRC_DIR)/allocator.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(SRC_DIR)/main.c $(SRC_DIR)/allocator.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET) $(TEST_DIR)/test_allocator.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(TEST_DIR)/test_allocator.c $(SRC_DIR)/allocator.c -o $(TEST_DIR)/test_allocator
	./$(TEST_DIR)/test_allocator

clean:
	rm -f $(TARGET) $(TEST_DIR)/test_allocator
