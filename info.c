#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edid.h"
#include "info.h"

struct di_info *
di_info_parse_edid(const void *data, size_t size)
{
	struct di_edid *edid;
	struct di_info *info;

	edid = di_edid_parse(data, size);
	if (!edid)
		return NULL;

	info = calloc(1, sizeof(*info));
	if (!info) {
		di_edid_destroy(edid);
		return NULL;
	}

	info->edid = edid;

	return info;
}

void
di_info_destroy(struct di_info *info)
{
	di_edid_destroy(info->edid);
	free(info);
}

const struct di_edid *
di_info_get_edid(const struct di_info *info)
{
	return info->edid;
}

char *
di_info_get_product_name(const struct di_info *info)
{
	char name[256], product_code[32], serial_code[32];
	const char *product, *serial;
	const struct di_edid_vendor_product *edid_vendor_product;
	const struct di_edid_display_descriptor *const *display_descs;
	size_t i;

	edid_vendor_product = di_edid_get_vendor_product(info->edid);

	snprintf(product_code, sizeof(product_code), "0x%X" PRIu16,
		 edid_vendor_product->product);
	if (edid_vendor_product->serial != 0) {
		snprintf(serial_code, sizeof(serial), " 0x%X" PRIu32,
			 edid_vendor_product->serial);
	} else {
		serial_code[0] = '\0';
	}

	product = product_code;
	serial = serial_code;
	display_descs = di_edid_get_display_descriptors(info->edid);
	for (i = 0; display_descs[i] != NULL; i++) {
		const struct di_edid_display_descriptor *desc = display_descs[i];
		switch (di_edid_display_descriptor_get_tag(desc)) {
		case DI_EDID_DISPLAY_DESCRIPTOR_PRODUCT_SERIAL:
			serial = di_edid_display_descriptor_get_string(desc);
			break;
		case DI_EDID_DISPLAY_DESCRIPTOR_PRODUCT_NAME:
			product = di_edid_display_descriptor_get_string(desc);
			break;
		default:
			break; /* Ignore */
		}
	}

	snprintf(name, sizeof(name), "%.3s %s%s",
		 edid_vendor_product->manufacturer, product, serial);

	return strdup(name);
}
