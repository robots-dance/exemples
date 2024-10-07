CC=cl
CFLAGS+=/O1 /GL- /GR- /MT /TP /GS-
LINKOPT=/subsystem:windows /NODEFAULTLIB

PROG=test
$(PROG): main.c
	$(CC) $(CFLAGS) /Fe$@.exe $^ /link gdi32.lib user32.lib \
		kernel32.lib $(LINKOPT)

clean:
	del /Q *.exe 2>nul
	del /Q *.obj 2>nul
