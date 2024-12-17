CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g
LDFLAGS= -lm
EXEC= ppm2jpeg
TESTS = bitstream_test compressionACDC_test compressionMCUs_test creatMCUs_test dct_test quantification_test zig_zag_test

all: $(EXEC)

ppm2jpeg: obj/downsampling.o obj/bitstream.o obj/compressionMCUs.o obj/ecrit_marqueurs.o obj/read_PGM.o obj/read_PPM.o obj/recupere_parametres.o obj/helper.o obj/RGB2YCbCr.o obj/ppm2jpeg.o obj/compressionACDC.o obj/htables.o obj/dct.o obj/zig_zag.o obj/quantification.o
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o : src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

bitstream_test: obj/bitstream_test.o obj/bitstream.o obj/compressionACDC.o obj/htables.o
	$(CC) -o $@ $^ $(LDFLAGS)

compressionACDC_test: obj/compressionACDC_test.o obj/compressionACDC.o obj/htables.o
	$(CC) -o $@ $^ $(LDFLAGS)

compressionMCUs_test: obj/compressionMCUs_test.o obj/compressionMCUs.o obj/bitstream.o obj/compressionACDC.o obj/htables.o obj/dct.o obj/zig_zag.o obj/quantification.o
	$(CC) -o $@ $^ $(LDFLAGS)

creatMCUs_test: obj/creatMCUs_test.o obj/creatMCUs.o
	$(CC) -o $@ $^ $(LDFLAGS)

dct_test: obj/dct_test.o obj/dct.o
	$(CC) -o $@ $^ $(LDFLAGS)

quantification_test: obj/quantification_test.o obj/quantification.o
	$(CC) -o $@ $^ $(LDFLAGS)

zig_zag_test: obj/zig_zag_test.o obj/zig_zag.o
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%_test.o : src/fct_test/%_test.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -f obj/*.o *.test *.jpg $(EXEC) $(TESTS)