PROG=braek

braek: braek.c braek.h
	$(CC) -o braek braek.c $$(pkg-config --cflags --libs cairo cairo-png gtk+-2.0)

clean:
	rm -f $(PROG)

install:
	mkdir -p $(CURDIR)/debian/braek/usr/bin/
	mkdir -p $(CURDIR)/debian/braek/usr/share/applications/hildon/
	mkdir -p $(CURDIR)/debian/braek/usr/share/icons/hicolor/scalable/apps/
	cp braek $(CURDIR)/debian/braek/usr/bin/braek
	cp braek.png $(CURDIR)/debian/braek/usr/share/icons/hicolor/scalable/apps/
	cp braek.desktop $(CURDIR)/debian/braek/usr/share/applications/hildon/
