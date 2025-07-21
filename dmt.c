#include <libdisplay-info/dmt.h>
#include <libdisplay-info/edid.h>

extern const struct di_dmt_timing _di_dmt_timings[];
extern const size_t _di_dmt_timings_len;

const struct di_dmt_timing *
di_dmt_timing_from_code(uint8_t dmt_id)
{
	size_t i;

	for (i = 0; i < _di_dmt_timings_len; i++) {
		const struct di_dmt_timing *t = &_di_dmt_timings[i];

		if (t->dmt_id == dmt_id)
			return t;
	}

	return NULL;
}

const struct di_dmt_timing *
di_dmt_timing_from_edid_standard_timing(const struct di_edid_standard_timing *t)
{
	int32_t vert_video;
	size_t i;
	const struct di_dmt_timing *dmt;

	vert_video = di_edid_standard_timing_get_vert_video(t);

	for (i = 0; i < _di_dmt_timings_len; i++) {
		dmt = &_di_dmt_timings[i];

		if (dmt->horiz_video == t->horiz_video
		    && dmt->vert_video == vert_video
		    && dmt->refresh_rate_hz == (float)t->refresh_rate_hz
		    && dmt->edid_std_id != 0) {
			return dmt;
		}
	}

	return 0;
}
