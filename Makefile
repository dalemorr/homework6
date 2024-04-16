CC = g++
CFLAGS = -g -Wall
TARGET = main
SRC = src
OUT = build

all: $(TARGET)

$(TARGET): $(SRC)/$(TARGET).cpp
	$(CC) $(CFLAGS) -o $(OUT)/$(TARGET) $(SRC)/$(TARGET).cpp

clean:
	$(RM) $(TARGET)
