LDFLAGS = -lcairo -lpixman-1 -lm
CFLAGS = -Wall -g -O0 -I/usr/include/cairo
OBJS = render.o pla.o utils.o main.o load.o

pla: $(OBJS)
	$(CC) -o pla $(LDFLAGS) $(OBJS)

clean: 
	rm -f pla $(OBJS)
