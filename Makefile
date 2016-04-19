PKG_CONFIG = $(shell which pkg-config)
INC_CAIRO =
LIB_CAIRO =

CFLAGS = -Wall -g -O2
LDFLAGS = -lm

ifneq ($(INC_CAIRO),)
  CFLAGS += -I$(INC_CAIRO)
else
  ifneq ($(PKG_CONFIG),)
    CFLAGS += $(shell $(PKG_CONFIG) --cflags cairo)
  else
    CFLAGS += -I/usr/include/cairo
  endif
endif

ifneq ($(LIB_CAIRO),)
  LDFLAGS += -l$(LIB_CAIRO)
else
  ifneq ($(PKG_CONFIG),)
    LDFLAGS += $(shell $(PKG_CONFIG) --libs cairo)
  else
    LDFLAGS += -lcairo
  endif
endif

OBJS = render.o render_txt.o pla.o utils.o main.o load.o utf8.o 

pla: $(OBJS)
	$(CC) -o pla $(LDFLAGS) $(OBJS)

clean: 
	rm -f pla $(OBJS)
