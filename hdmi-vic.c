#include <stddef.h>

#include <libdisplay-info/hdmi-vic.h>

/**
 * HDMI video format table. 8.2.3.1 section of the 1.4b HDMI spec.
 */
static const struct di_hdmi_vic_video_format _di_hdmi_vic_video_formats[] = {
	{
		.vic = 1,
		.h_active = 3840,
		.v_active = 2160,
		.pixel_clock_hz = 297000000,
		.h_front = 176,
		.h_sync = 88,
		.h_back = 296,
		.v_front = 8,
		.v_sync = 10,
		.v_back = 72,
	},
	{
		.vic = 2,
		.h_active = 3840,
		.v_active = 2160,
		.pixel_clock_hz = 297000000,
		.h_front = 1056,
		.h_sync = 88,
		.h_back = 296,
		.v_front = 8,
		.v_sync = 10,
		.v_back = 72,
	},
	{
		.vic = 3,
		.h_active = 3840,
		.v_active = 2160,
		.pixel_clock_hz = 297000000,
		.h_front = 1276,
		.h_sync = 88,
		.h_back = 296,
		.v_front = 8,
		.v_sync = 10,
		.v_back = 72,
	},
	{
		.vic = 4,
		.h_active = 4096,
		.v_active = 2160,
		.pixel_clock_hz = 297000000,
		.h_front = 1020,
		.h_sync = 88,
		.h_back = 296,
		.v_front = 8,
		.v_sync = 10,
		.v_back = 72,
	},
};

static const size_t _di_hdmi_vic_video_formats_len =
	sizeof(_di_hdmi_vic_video_formats) / sizeof(_di_hdmi_vic_video_formats[0]);

const struct di_hdmi_vic_video_format *
di_hdmi_vic_video_format_from_vic(uint8_t vic)
{
	size_t i;

	for (i = 0; i < _di_hdmi_vic_video_formats_len; i++) {
		if (_di_hdmi_vic_video_formats[i].vic == vic)
			return &_di_hdmi_vic_video_formats[i];
	}

	return NULL;
}
