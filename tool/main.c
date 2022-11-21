#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include <libdisplay-info/edid.h>
#include <libdisplay-info/info.h>

#include "schema.h"

static struct edid *
get_edid(const struct di_edid *edid)
{
	const struct di_edid_vendor_product *vendor_product;
	char manufacturer[4];
	struct edid *wire_edid;
	struct edid_vendor_product *wire_vendor_product;

	vendor_product = di_edid_get_vendor_product(edid);

	snprintf(manufacturer, sizeof(manufacturer), "%.3s", vendor_product->manufacturer);

	wire_vendor_product = malloc(sizeof(*wire_vendor_product));
	*wire_vendor_product = (struct edid_vendor_product) {
		.manufacturer = strdup(manufacturer),
		.product = vendor_product->product,
		.serial = vendor_product->serial,
		.model_year = vendor_product->model_year,
		.manufacture_week = vendor_product->manufacture_week,
		.manufacture_year = vendor_product->manufacture_year,
	};

	wire_edid = malloc(sizeof(*wire_edid));
	*wire_edid = (struct edid) {
		.version = di_edid_get_version(edid),
		.revision = di_edid_get_revision(edid),
		.vendor_product = wire_vendor_product,
	};

	return wire_edid;
}

static struct display_info *
get_info(const struct di_info *info)
{
	const struct di_edid *edid;
	struct edid *wire_edid = NULL;
	struct display_info *wire_info;

	edid = di_info_get_edid(info);
	if (edid != NULL)
		wire_edid = get_edid(edid);

	wire_info = malloc(sizeof(*wire_info));
	*wire_info = (struct display_info) {
		.edid = wire_edid,
	};

	return wire_info;
}

static const char usage[] =
	"usage: libdisplay-info <edid>\n";

static const struct option long_options[] = {
	{ "help", no_argument, 0, 'h' },
	{ 0, 0, 0, 0 }
};

int
main(int argc, char *argv[])
{
	FILE *in;
	static uint8_t raw[32 * 1024];
	size_t size = 0;
	struct di_info *info;
	int opt;
	struct jsch_encoder *enc;
	struct display_info *wire_info;

	in = stdin;
	while (1) {
		int option_index = 0;
		opt = getopt_long(argc, argv, "h", long_options, &option_index);
		if (opt == -1)
			break;

		fprintf(stderr, "%s", usage);
		return opt == 'h' ? 0 : 1;
	}

	if (argc > 1) {
		in = fopen(argv[1], "r");
		if (!in) {
			perror("failed to open input file");
			return 1;
		}
	}

	while (!feof(in)) {
		size += fread(&raw[size], 1, sizeof(raw) - size, in);
		if (ferror(in)) {
			perror("fread failed");
			return 1;
		} else if (size >= sizeof(raw)) {
			fprintf(stderr, "input too large\n");
			return 1;
		}
	}

	fclose(in);

	info = di_info_parse_edid(raw, size);
	if (!info) {
		perror("di_edid_parse failed");
		return 1;
	}

	wire_info = get_info(info);

	enc = jsch_encoder_create(stdout);
	display_info_encode(wire_info, enc);
	jsch_encoder_destroy(enc);
	printf("\n");

	display_info_finish(wire_info);
	free(wire_info);
	di_info_destroy(info);
	return 0;
}
