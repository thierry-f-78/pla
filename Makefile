PKG_CONFIG = $(shell which pkg-config)
INC_CAIRO =
LIB_CAIRO =

CFLAGS = -Wall -g -O2
LDLIBS = -lm

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
  LDLIBS += -l$(LIB_CAIRO)
else
  ifneq ($(PKG_CONFIG),)
    LDLIBS += $(shell $(PKG_CONFIG) --libs-only-l cairo)
    LDFLAGS += $(shell $(PKG_CONFIG) --libs-only-L cairo)
  else
    LDLIBS += -lcairo
  endif
endif

OBJS = render.o render_txt.o pla.o utils.o main.o load.o utf8.o 

pla: $(OBJS)

clean: 
	rm -f pla $(OBJS)
