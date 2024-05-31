.PHONY: all clean

CXX=cl
CXXFLAGS=/TP

COM_CLIENT=com-client.exe
COM_SERVER=com-server.dll

all: $(COM_CLIENT) $(COM_SERVER)

$(COM_CLIENT): Client.cpp
	$(CXX) /EHsc $(CXXFLAGS) /Fe$(COM_CLIENT) $^ /link Ole32.lib

$(COM_SERVER): Server.cpp comtest.cpp
	$(CXX) /EHsc $(CXXFLAGS) /LD $^ /link /def:server.def /out:$(COM_SERVER)

clean:
	del /Q $(COM_CLIENT) 2>nul
	del /Q $(COM_SERVER) 2>nul
	del /Q *.exp 2>nul
	del /Q *.lib 2>nul
	del /Q *.obj 2>nul
