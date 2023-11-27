EXEC_NAME = c-serve

SRCDIR = src
BINDIR = bin
OBJDIR = obj

# Path: Source files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

OPT = -Wall -Wextra -Werror -pedantic -std=c17 -g
LIBS = 
CFLAGS = $(OPT) $(LIBS)
CC := clang

.PHONE: all setup clean

all: setup $(OBJS)
	@echo "Building final executable..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(BINDIR)/$(EXEC_NAME)

setup:
	@if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

clean:
	@if [ -d $(BINDIR) ]; then rm -rf $(BINDIR); fi
	@if [ -d $(OBJDIR) ]; then rm -rf $(OBJDIR); fi


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Building object file: $@"
	@$(CC) $(CFLAGS) -c $< -o $@
