BUILD := build
PREFIX :=
DEPS := libxml-2.0
CFLAGS := -g -Wall -std=c89 $(shell pkg-config --cflags $(DEPS)) -c
LDFLAGS := $(shell pkg-config --libs $(DEPS))

SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix $(BUILD)/,$(notdir $(SOURCES:.c=.o)))

.PHONY: all
all: $(BUILD) $(BUILD)/pleb

$(BUILD):
	mkdir -p $@

$(BUILD)/pleb: $(OBJECTS)
	gcc -g -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILD)/%.o:src/%.c
	gcc -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(BUILD)
