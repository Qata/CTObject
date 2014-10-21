PREFIX = /usr/local/i686-pc-cygwin/sys-root/usr
CC = i686-pc-cygwin-gcc
AR = i686-pc-cygwin-ar
SRC = CTAllocator.c CTArray.c CTBencode.c CTData.c CTDictionary.c CTError.c CTFunctions.c CTJSON.c CTNetServer.c CTNull.c CTNumber.c CTObject.c CTString.c
OUT = $(SRC:.c=.o)
INC = $(SRC:.c=.h)
NAME = libCTObject.a

all: $(SRC) config.h
	make clean
	$(CC) -c $(SRC) -std=gnu99
	$(AR) rcs lib/$(NAME) $(OUT)
    
clean:
	rm -f lib/libCTObject.a
	rm -f $(OUT)

install:
	mkdir -p $(PREFIX)
	install -c -m 644 lib/libCTObject.a $(PREFIX)/lib
	install -c -m 666 $(INC) $(PREFIX)/include

