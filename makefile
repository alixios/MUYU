CC=arm-linux-gcc
CONFIG=-I./inc -L./lib -lfont -ljpeg -lm
T=./bin/main

SRCS=$(wildcard ./src/*.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

$(T):$(OBJS)
	$(CC) -o $@ $^ $(CONFIG)
	scp  $(T) root@192.168.11.111:/alan

%.o:%.c
	@$(CC) -c $< -o $@ $(CONFIG)


.PHONY: clean
clean:
	-rm -rf $(OBJS)