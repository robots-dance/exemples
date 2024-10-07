TEST_DISAS=test-disas.exe

CC=cl

CFLAGS+=/Idistorm/include /TP /EHsc
LDFLAGS+=/LIBPATH:distorm/lib

$(TEST_DISAS): main.cpp
	$(CC) $(CFLAGS) /Fe$@ $^ /link $(LDFLAGS) distorm3.lib

clean:
	del /Q $(TEST_DISAS) 2>nul
	del /Q *.obj 2>nul
