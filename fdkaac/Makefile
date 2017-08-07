GCC = gcc 
CXX = g++
CXXFLAG = -Wall -g

FDKAAC_LDLIBS = lib/libfdk-aac.a
FDKAAC_INCS = -Iinclude/
FDKAAC_DEPS =  include/fdk-aac/machine_type.h include/fdk-aac/aacenc_lib.h include/fdk-aac/aacdecoder_lib.h include/fdk-aac/FDK_audio.h

ENC_OBJS = fdkaac_enc.o enc_test.o wavreader.o
DEC_OBJS = fdkaac_dec.o dec_test.o wavwriter.o

all: enc_test dec_test

enc_test: $(ENC_OBJS)
	$(CXX) -o enc_test $(CXXFLAG) $(ENC_OBJS) $(FDKAAC_LDLIBS)

fdkaac_enc.o: $(FDKAAC_DEPS) fdkaac_enc.cpp fdkaac_enc.h
	$(CXX) -c $(CXXFLAG) $(FDKAAC_INCS) -o fdkaac_enc.o fdkaac_enc.cpp

enc_test.o: enc_test.cpp fdkaac_enc.h 
	$(CXX) -c $(CXXFLAG) enc_test.cpp -o enc_test.o

wavreader.o: wavreader.c wavreader.h
	$(CXX) -c $(CXXFLAG) wavreader.c -o wavreader.o

dec_test: $(DEC_OBJS)
	$(CXX) -o dec_test $(CXXFLAG) $(DEC_OBJS) $(FDKAAC_LDLIBS)

fdkaac_dec.o: $(FDKAAC_DEPS) fdkaac_dec.cpp fdkaac_dec.h
	$(CXX) -c $(CXXFLAG) $(FDKAAC_INCS) -o fdkaac_dec.o fdkaac_dec.cpp

dec_test.o: dec_test.cpp fdkaac_dec.h
	$(CXX) -c $(CXXFLAG) dec_test.cpp -o dec_test.o

wavwriter.o: wavwriter.c wavwriter.h
	$(CXX) -c $(CXXFLAG) wavwriter.c -o wavwriter.o

clean :
	rm enc_test $(ENC_OBJS) dec_test $(DEC_OBJS)
