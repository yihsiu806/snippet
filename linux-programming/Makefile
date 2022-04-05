SRCPATH := $(shell pwd)
DESTPATH := $(SRCPATH)/out
EXEC := $(patsubst %.c,$(DESTPATH)/%,$(wildcard *.c))

all: $(EXEC)

$(EXEC): $(DESTPATH)/%: $(SRCPATH)/%.c
	@mkdir -p $(DESTPATH)
	gcc -o $@ $<

clean:
	rm -rf $(EXEC)

.PHONY: all clean