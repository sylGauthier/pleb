BUILD := build
DEPS :=
PREFIX :=

SOURCES := $(wildcard *.c)
OBJECTS := $(addprefix $(BUILD)/,$(notdir $(SOURCES:.c=.o)))

.PHONY: all
all: $(BUILD) $(BUILD)/pleb

$(BUILD):
	mkdir -p $@

$(BUILD)/pleb: $(OBJECTS)
	gcc -g -o $@ $(OBJECTS)

$(BUILD)/%.o:%.c
	gcc -g -o $@ -Wall -c $<

.PHONY: clean
clean:
	rm -rf $(BUILD)
