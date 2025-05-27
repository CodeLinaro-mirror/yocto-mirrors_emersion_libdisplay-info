#!/bin/sh -eux

REF_EDID_DECODE="${REF_EDID_DECODE:-edid-decode}"

workdir="$(mktemp -d)"
cleanup() {
	rm -rf "$workdir"
}
trap cleanup EXIT

for edid in "$@"; do
	"$REF_EDID_DECODE" --skip-hex-dump --check --skip-sha <"$edid" >"$workdir/ref" || [ $? = 254 ]

	# ensure the committed and generated reference are the same
	diff -u "$workdir/ref" "${edid%.edid}.ref"
done
