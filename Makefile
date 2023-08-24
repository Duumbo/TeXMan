CC = cargo
CFLAGS = --release --jobs=-1

all:
	$(CC) build $(CFLAGS)

install:
	ls $(SOURCE)
	cp $(SOURCE)target/release/texman $(DESTDIR)usr/bin/texman

check:
	$(CC) test $(CFLAGS)
