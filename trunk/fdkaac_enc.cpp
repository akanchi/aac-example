#include <iostream>
#include "fdkaac_enc.h"


AacEncoder::AacEncoder()
{
	
}

AacEncoder::~AacEncoder()
{
	aacenc_close();
}

int AacEncoder::aacenc_init(int aot, int channels, int sample_rate, int bitrate)
{
	AACENC_ERROR err = AACENC_OK;

	_h.aot = aot;
	_h.channels = channels;
	_h.sample_rate = sample_rate;
	_h.bitrate = bitrate;

	// AACENC_TRANSMUX
    // Transport type to be used. See ::TRANSPORT_TYPE in FDK_audio.h.
    // Following types can be configured in encoder library:
    //         - 0: raw access units
    //         - 1: ADIF bitstream format
    //         - 2: ADTS bitstream format
    //         - 6: Audio Mux Elements (LATM) with muxConfigPresent = 1
    //         - 7: Audio Mux Elements (LATM) with muxConfigPresent = 0, out of band StreamMuxConfig
    //         - 10: Audio Sync Stream (LOAS)
	int trans_mux = 2; // adts
	int signaling = 0; // Implicit backward compatible signaling (default for ADIF and ADTS)
	int afterburner = 1; // 1 or 0(default)
	// -------------------------------------------------------------------------------
	//  ChannelMode           | ChCfg  | front_El      | side_El  | back_El  | lfe_El
	// -----------------------+--------+---------------+----------+----------+--------
	// MODE_1                 |      1 | SCE           |          |          |
	// MODE_2                 |      2 | CPE           |          |          |
	// MODE_1_2               |      3 | SCE, CPE      |          |          |
	// MODE_1_2_1             |      4 | SCE, CPE      |          | SCE      |
	// MODE_1_2_2             |      5 | SCE, CPE      |          | CPE      |
	// MODE_1_2_2_1           |      6 | SCE, CPE      |          | CPE      | LFE
	// MODE_1_2_2_2_1         |      7 | SCE, CPE, CPE |          | CPE      | LFE
	// -----------------------+--------+---------------+----------+----------+--------
	// MODE_7_1_REAR_SURROUND |      0 | SCE, CPE      |          | CPE, CPE | LFE
	// MODE_7_1_FRONT_CENTER  |      0 | SCE, CPE, CPE |          | CPE      | LFE
	// -------------------------------------------------------------------------------
	//  - SCE: Single Channel Element.
	//  - CPE: Channel Pair.
	//  - LFE: Low Frequency Element.
	CHANNEL_MODE mode = MODE_INVALID;
    switch (channels) {
		case 1: mode = MODE_1;       break;
		case 2: mode = MODE_2;       break;
		case 3: mode = MODE_1_2;     break;
		case 4: mode = MODE_1_2_1;   break;
		case 5: mode = MODE_1_2_2;   break;
		case 6: mode = MODE_1_2_2_1; break;
		default:
			return 1;
	}

	if ((err = aacEncOpen(&_h.enc, 0, channels)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_AOT, aot)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_SAMPLERATE, sample_rate)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_CHANNELMODE, mode)) != AACENC_OK) {
        return err;
    }

    // Input audio data channel ordering scheme:
    //      - 0: MPEG channel ordering (e. g. 5.1: C, L, R, SL, SR, LFE). (default)
    //      - 1: WAVE file format channel ordering (e. g. 5.1: L, R, C, LFE, SL, SR).
    if ((err = aacEncoder_SetParam(_h.enc, AACENC_CHANNELORDER, 1)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_BITRATE, bitrate)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_TRANSMUX, trans_mux)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_SIGNALING_MODE, signaling)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncoder_SetParam(_h.enc, AACENC_AFTERBURNER, afterburner)) != AACENC_OK) {
        return err;
    }

    if ((err = aacEncEncode(_h.enc, NULL, NULL, NULL, NULL)) != AACENC_OK) {
        return err;
    }

    AACENC_InfoStruct info = {0};
    if ((err = aacEncInfo(_h.enc, &info)) != AACENC_OK) {
    	return err;
    }

    _h.frame_size = info.frameLength;

	return err;
}

void AacEncoder::aacenc_close()
{
	aacEncClose(&_h.enc);
}

int AacEncoder::aacenc_encode(char *pcm, int nb_pcm, int nb_samples, char *aac, int &pnb_aac)
{
	AACENC_ERROR err = AACENC_OK;
    INT iidentify = IN_AUDIO_DATA;
    INT oidentify = OUT_BITSTREAM_DATA;
	INT ibuffer_element_size = 2; // 16bits.
	INT ibuffer_size = 2 * _h.channels * nb_samples;
	// The intput pcm must be resampled to fit the encoder,
	// for example, the intput is 2channels but encoder is 1channels,
	// then we should resample the intput pcm to 1channels
	// to make the intput pcm size equals to the encoder calculated size(ibuffer_size).
	// std::cout << ibuffer_size << std::endl;
	if (ibuffer_size != nb_pcm) {
		return -1;
	}

	AACENC_BufDesc ibuf = {0};
	if (nb_pcm > 0) {
		ibuf.numBufs = 1;
		ibuf.bufs = (void**)&pcm;
		ibuf.bufferIdentifiers = &iidentify;
		ibuf.bufSizes = &ibuffer_size;
		ibuf.bufElSizes = &ibuffer_element_size;
	}
	AACENC_InArgs iargs = {0};
	if (nb_pcm > 0) {
		iargs.numInSamples =  _h.channels * nb_samples;
	} else {
		iargs.numInSamples = -1;
	}
	INT obuffer_element_size = 1;
	INT obuffer_size = pnb_aac;
	AACENC_BufDesc obuf = {0};
	obuf.numBufs = 1;
	obuf.bufs = (void**)&aac;
	obuf.bufferIdentifiers = &oidentify;
	obuf.bufSizes = &obuffer_size;
	obuf.bufElSizes = &obuffer_element_size;
	AACENC_OutArgs oargs = {0};
	if ((err = aacEncEncode(_h.enc, &ibuf, &obuf, &iargs, &oargs)) != AACENC_OK) {
		// Flush ok, no bytes to output anymore.
		if (!pcm && err == AACENC_ENCODE_EOF) {
			pnb_aac = 0;
			return AACENC_OK;
		}
		return err;
	}

	pnb_aac = oargs.numOutBytes;
	return err;
}

int AacEncoder::aacenc_frame_size()
{
	return _h.frame_size;
}

int AacEncoder::aacenc_max_output_buffer_size()
{
	int out_buffer_size = 8192;
	if (_h.channels * 768 > out_buffer_size) {
		out_buffer_size = _h.channels * 768;
	}
	
	return out_buffer_size;
}
