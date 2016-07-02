GCC = gcc 
CXX = g++
CXXFLAG = -Wall -g

FDKAAC_LDLIBS = lib/libfdk-aac.a
FDKAAC_INCS = -Iinclude/
FDKAAC_DEPS =  include/fdk-aac/machine_type.h include/fdk-aac/aacenc_lib.h include/fdk-aac/FDK_audio.h

OBJS = fdkaac_enc.o enc_test.o wavreader.o

enc_test: $(OBJS)
	$(CXX) -o enc_test $(CXXFLAG) $(OBJS) $(FDKAAC_LDLIBS)

fdkaac_enc.o: $(FDKAAC_DEPS) fdkaac_enc.cpp fdkaac_enc.h
	$(CXX) -c $(CXXFLAG) $(FDKAAC_INCS) -o fdkaac_enc.o fdkaac_enc.cpp

enc_test.o: enc_test.cpp fdkaac_enc.h 
	$(CXX) -c $(CXXFLAG) enc_test.cpp -o enc_test.o

wavreader.o: wavreader.c wavreader.h
	$(CXX) -c $(CXXFLAG) wavreader.c -o wavreader.o

clean :
	rm enc_test $(OBJS)
