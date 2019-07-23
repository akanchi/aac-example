#include "fdkaac_dec.h"

AacDecoder::AacDecoder()
{
	_h.dec = NULL;
}

AacDecoder::~AacDecoder()
{
	aacdec_close();
}

int AacDecoder::aacdec_init_adts()
{
	_h.sample_bits = 16;
	_h.is_adts = 1;
	_h.filled_bytes = 0;

	_h.dec = aacDecoder_Open(TT_MP4_ADTS, 1);
	if (!_h.dec) {
		return -1;
	}

	_h.info = NULL;

	return 0;
}

void AacDecoder::aacdec_close()
{
	if (_h.dec) {
		aacDecoder_Close(_h.dec);
	}
	_h.dec = NULL;
}

int AacDecoder::aacdec_fill(char *data, int nb_data, int *pnb_left)
{
	_h.filled_bytes += nb_data;

	unsigned char *udata = (unsigned char *)data;
	unsigned int unb_data = (unsigned int)nb_data;
	unsigned int unb_left = unb_data;
	AAC_DECODER_ERROR err = aacDecoder_Fill(_h.dec, &udata, &unb_data, &unb_left);
	if (err != AAC_DEC_OK) {
		return err;
	}

	if (pnb_left) {
		*pnb_left = (int)unb_left;
	}

	return 0;
}

int AacDecoder::aacdec_sample_bits()
{
	return _h.sample_bits;
}

int AacDecoder::aacdec_pcm_size()
{
	if (!_h.info) {
		return 0;
	}
	return (int)(_h.info->numChannels * _h.info->frameSize * _h.sample_bits / 8);
}

int AacDecoder::aacdec_decode_frame(char *pcm, int nb_pcm, int *pnb_valid)
{
	// when buffer left bytes not enough, directly return not-enough-bits.
	// we requires atleast 7bytes header for adts.
	if (_h.is_adts && _h.info && _h.filled_bytes - _h.info->numTotalBytes <= 7) {
		return AAC_DEC_NOT_ENOUGH_BITS;
	}

	INT_PCM* upcm = (INT_PCM*)pcm;
	int unb_pcm = (int)nb_pcm;
	AAC_DECODER_ERROR err = aacDecoder_DecodeFrame(_h.dec, upcm, unb_pcm, 0);
	// user should fill more bytes then decode.
	if (err == AAC_DEC_NOT_ENOUGH_BITS) {
		return err;
	}
	if (err != AAC_DEC_OK) {
		return err;
	}

	// when decode ok, retrieve the info.
	if (!_h.info) {
		_h.info = aacDecoder_GetStreamInfo(_h.dec);
	}

	// the actual size of pcm.
	if (pnb_valid) {
		*pnb_valid = aacdec_pcm_size();
	}

	return 0;
}

int AacDecoder::aacdec_sample_rate()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->sampleRate;
}

int AacDecoder::aacdec_frame_size()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->frameSize;
}

int AacDecoder::aacdec_num_channels()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->numChannels;
}

int AacDecoder::aacdec_aac_sample_rate()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->aacSampleRate;
}

int AacDecoder::aacdec_profile()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->profile;
}

int AacDecoder::aacdec_audio_object_type()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->aot;
}

int AacDecoder::aacdec_channel_config()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->channelConfig;
}

int AacDecoder::aacdec_bitrate()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->bitRate;
}

int AacDecoder::aacdec_aac_samples_per_frame()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->aacSamplesPerFrame;
}

int AacDecoder::aacdec_aac_num_channels()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->aacNumChannels;
}

int AacDecoder::aacdec_extension_audio_object_type()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->extAot;
}

int AacDecoder::aacdec_extension_sampling_rate()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->extSamplingRate;
}

int AacDecoder::aacdec_num_lost_access_units()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->numLostAccessUnits;
}

int AacDecoder::aacdec_num_total_bytes()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->numTotalBytes;
}

int AacDecoder::aacdec_num_bad_bytes()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->numBadBytes;
}

int AacDecoder::aacdec_num_total_access_units()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->numTotalAccessUnits;
}

int AacDecoder::aacdec_num_bad_access_units()
{
	if (!_h.info) {
		return 0;
	}
	return _h.info->numBadAccessUnits;
}
