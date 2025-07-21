#include <libdisplay-info/cta-vic.h>

extern const struct di_cta_vic_video_format _di_cta_vic_video_formats[];
extern const size_t _di_cta_vic_video_formats_len;

const struct di_cta_vic_video_format *
di_cta_vic_video_format_from_vic(uint8_t vic)
{
	if (vic > _di_cta_vic_video_formats_len ||
	    _di_cta_vic_video_formats[vic].vic == 0)
		return NULL;
	return &_di_cta_vic_video_formats[vic];
}
