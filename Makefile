#mgbaird
SHELL := /bin/bash

# program name
TARGET = bee


#### compile stuff  #################################################

DIR        = $(CURDIR)
TEST_DIR   = examples
SRC_DIR    = source
OBJ_DIR    = objects
HEADER_DIR = header

# compiler options
CC      = gcc
CFLAGS  = -g -Wall -Wextra 
LIBS    = -lm
INCLUDE = -I $(HEADER_DIR) 

default: $(TARGET)
all: default

SOURCE  = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCE))
HEADERS = $(wildcard $(HEADER_DIR)/*.h)


$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADERS) 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

	chmod +x ${TARGET}  


# clean 
clean:
	rm -f $(OBJECTS) $(TARGET)

#tests
error1: ${TARGET}
	cat $(TEST_DIR)/error1.b
error2: ${TARGET}
	cat $(TEST_DIR)/error2.b
error3: ${TARGET}
	cat $(TEST_DIR)/error3.b
error4: ${TARGET}
	cat $(TEST_DIR)/error4.b
error5: ${TARGET}
	cat $(TEST_DIR)/error5.b
arrays: ${TARGET}
	cat $(TEST_DIR)/arrays.b
conditionals: ${TARGET}
	cat $(TEST_DIR)/conditionals.b
recursion: ${TARGET}
	cat $(TEST_DIR)/recursion.b
iteration: ${TARGET}
	cat $(TEST_DIR)/iteration.b
lambda: ${TARGET}
	cat $(TEST_DIR)/lambda.b
functions: ${TARGET}
	cat $(TEST_DIR)/functions.b
objects: ${TARGET}
	cat $(TEST_DIR)/objects.b	
command: ${TARGET}
	cat $(TEST_DIR)/command.b
file: ${TARGET}
	cat $(TEST_DIR)/file.b
problem: ${TARGET}
	cat $(TEST_DIR)/file.b	

example: ${TARGET}
	cat $(TEST_DIR)/example.b

#testsx
error1x: ${TARGET}
	./${TARGET} $(TEST_DIR)/error1.b
error2x: ${TARGET}
	./${TARGET} $(TEST_DIR)/error2.b
error3x: ${TARGET}
	./${TARGET} $(TEST_DIR)/error3.b
error4x: ${TARGET}
	./${TARGET} $(TEST_DIR)/error4.b
error5x: ${TARGET}
	./${TARGET} $(TEST_DIR)/error5.b
arraysx: ${TARGET}
	./${TARGET} $(TEST_DIR)/arrays.b
conditionalsx: ${TARGET}
	./${TARGET} $(TEST_DIR)/conditionals.b
recursionx: ${TARGET}
	./${TARGET} $(TEST_DIR)/recursion.b
iterationx: ${TARGET}
	./${TARGET} $(TEST_DIR)/iteration.b
lambdax: ${TARGET}
	./${TARGET} $(TEST_DIR)/lambda.b
functionsx: ${TARGET}
	./${TARGET} $(TEST_DIR)/functions.b
commandx: ${TARGET}
	./${TARGET} $(TEST_DIR)/command.b turtle elephant
filex: ${TARGET}
	./${TARGET} $(TEST_DIR)/file.b
objectsx: ${TARGET}
	./${TARGET} $(TEST_DIR)/objects.b
problemx: ${TARGET}
	./${TARGET} $(TEST_DIR)/file.b


examplex: ${TARGET}
	./${TARGET} $(TEST_DIR)/example.b