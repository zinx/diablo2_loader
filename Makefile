
MOD := loader
UTIL := ../util

CFLAGS := -Wall -O3 -fomit-frame-pointer -I$(UTIL)

SOURCES := $(MOD).c
OBJS := $(subst .c,.o,$(SOURCES))

all: $(MOD).dll

$(MOD).dll: $(OBJS) $(UTIL)/util.lib
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(MOD).dll *.o

-include $(addprefix .,$(subst .c,.d,$(SOURCES)))
.%.d: %.c
	$(CC) $(CFLAGS) -o $@ -MM $<
