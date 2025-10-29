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

uint8_t
di_cta_vic_video_format_to_vic(const struct di_cta_vic_video_format *format)
{
	size_t i;

	for (i = 0; i < _di_cta_vic_video_formats_len; i++) {
		const struct di_cta_vic_video_format *candidate = &_di_cta_vic_video_formats[i];

		if (candidate->vic != 0 &&
		    candidate->h_active == format->h_active &&
		    candidate->v_active == format->v_active &&
		    candidate->h_sync == format->h_sync &&
		    candidate->v_sync == format->v_sync &&
		    candidate->h_back == format->h_back &&
		    candidate->v_back == format->v_back &&
		    candidate->h_sync_polarity == format->h_sync_polarity &&
		    candidate->v_sync_polarity == format->v_sync_polarity &&
		    candidate->pixel_clock_hz == format->pixel_clock_hz &&
		    candidate->interlaced == format->interlaced &&
		    candidate->picture_aspect_ratio == format->picture_aspect_ratio)
			return candidate->vic;
	}

	return 0;
}
