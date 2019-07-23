#ifndef FDKAAC_DEC_H
#define FDKAAC_DEC_H

extern "C"{
	#include "include/fdk-aac/aacdecoder_lib.h"
}

class aacdec_t
{
public:
	// the decoder handler.
	HANDLE_AACDECODER dec;
	// whether use ADTS mode.
	int is_adts;
	// init until the first frame decoded.
	CStreamInfo *info;
	// the bits of sample, always 16 for fdkaac.
	int sample_bits;
	// total filled bytes.
	unsigned int filled_bytes;
};

class AacDecoder
{
public:
	AacDecoder();
	~AacDecoder();

	int aacdec_init_adts();
	void aacdec_close();
	int aacdec_fill(char *data, int nb_data, int *pnb_left);
	int aacdec_sample_bits();
	int aacdec_pcm_size();
	int aacdec_decode_frame(char *pcm, int nb_pcm, int *pnb_valid);

	int aacdec_sample_rate();
	int aacdec_frame_size();
	int aacdec_num_channels();
	int aacdec_aac_sample_rate();
	int aacdec_profile();
	int aacdec_audio_object_type();
	int aacdec_channel_config();
	int aacdec_bitrate();
	int aacdec_aac_samples_per_frame();
	int aacdec_aac_num_channels();
	int aacdec_extension_audio_object_type();
	int aacdec_extension_sampling_rate();
	int aacdec_num_lost_access_units();
	int aacdec_num_total_bytes();
	int aacdec_num_bad_bytes();
	int aacdec_num_total_access_units();
	int aacdec_num_bad_access_units();

private:
	aacdec_t _h;
	
};

#endif