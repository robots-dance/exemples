TEST_SERVICE=test-service.exe

CXX=cl
CXXFLAGS=/TP

$(TEST_SERVICE): test-service.cpp
	$(CXX) $(CXXFLAGS) $< /link advapi32.lib

clean:
	del /Q $(TEST_SERVICE) 2>nul
	del /Q *.obj 2>nul
