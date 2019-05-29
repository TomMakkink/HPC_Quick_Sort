CC = mpicc
BIN = qsort
SRCDIR = src
OBJDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
HDRS = $(wildcard $(SRCDIR)/*.h)
OBJS = $(addprefix $(OBJDIR)/, $(patsubst $(SRCDIR)/%.c, %.o, $(SRCS)))
CFLAGS += -fPIC -g -Wall -I$(SRCDIR) -fopenmp -std=c99 -lm
VPATH = $(SRCDIR)

.SUFFIXES:
.SUFFIXES: .o .c

.PHONY: all clean

all: $(BIN)

clean:
	rm -rf $(OBJDIR) $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJS): | $(OBJDIR)

$(OBJDIR)/%.o: %.c $(HDRS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR):
	mkdir $(OBJDIR)


