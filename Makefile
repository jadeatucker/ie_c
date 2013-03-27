CC = gcc
CFLAGS = -lz -Wall -g -ansi

ODIR = obj
SDIR = src
INC = -Iinc
BUILD = build

OBJS = $(addprefix $(ODIR)/, ie_c.o ie_c_bif.o)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 

keytest: $(OBJS)
	$(CC) keytest.c -o $(BUILD)/$@ $(OBJS) $(INC) $(CFLAGS)

unbifc: $(OBJS)
	$(CC) unbifc.c -o $(BUILD)/$@ $(OBJS) $(INC) $(CFLAGS)

setup:
	mkdir -p build obj

clean:
	rm -rf build/ obj/
