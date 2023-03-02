project(
	'edid-decode',
	'cpp',
)

run_command(
	'make', '-C', meson.current_source_dir(),
	check: true,
)

meson.override_find_program('edid-decode', files('edid-decode'))
