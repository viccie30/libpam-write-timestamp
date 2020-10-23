prefix ?= /usr/local
exec_prefix ?= $(prefix)
libdir ?= $(exec_prefix)/lib
pamdir ?= $(libdir)/security

INSTALL ?= install

CFLAGS += -fPIC
LDFLAGS += -shared

all: pam_write_timestamp.so

pam_write_timestamp.so: pam_write_timestamp.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ "$(pamdir)/pam_timestamp.so" -o $@

install: all
	$(INSTALL) -d -m 755 "$(DESTDIR)$(pamdir)"
	$(INSTALL) -m 644 pam_write_timestamp.so "$(DESTDIR)$(pamdir)"

clean:
	rm -rf pam_write_timestamp.so

.PHONY: all install clean
