prefix := /usr/local
exec_prefix := $(prefix)
libdir := $(exec_prefix)/lib
pamdir := $(libdir)/security

INSTALL ?= install

all: pam_write_timestamp.so

pam_write_timestamp.so: pam_write_timestamp.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -DPAM_DIRECTORY="\"$(pamdir)\"" -fPIC -shared $^ -lpam -ldl -o $@

install: all
	$(INSTALL) -d -m 755 "$(DESTDIR)$(pamdir)"
	$(INSTALL) -m 644 pam_write_timestamp.so "$(DESTDIR)$(pamdir)"

clean:
	rm -rf pam_write_timestamp.so

.PHONY: all install clean
