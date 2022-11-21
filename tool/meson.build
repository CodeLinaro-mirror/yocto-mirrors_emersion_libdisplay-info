libjsonschema = dependency('libjsonschema')
libjsonschema_native = dependency('libjsonschema', native: true)
libjsonschema_tool_path = libjsonschema_native.get_variable(pkgconfig: 'tool', internal: 'tool')
libjsonschema_tool = find_program(libjsonschema_tool_path, native: true)

schema = files('schema.json')

schema_header = custom_target(
	'schema.h',
	command: [libjsonschema_tool, 'gen-header', '@INPUT@'],
	input: schema,
	output: 'schema.h',
	capture: true,
)

schema_code = custom_target(
	'schema.c',
	command: [libjsonschema_tool, 'gen-code', '@INPUT@'],
	input: schema,
	output: 'schema.c',
	capture: true,
)

schema_lib = static_library(
	'schema',
	schema_code,
	c_args: cc.get_supported_arguments(['-Wno-declaration-after-statement']),
	dependencies: libjsonschema,
)

executable(
	'libdisplay-info',
	[
		'main.c',
		schema_header,
	],
	dependencies: [di_dep, libjsonschema],
	link_with: schema_lib,
	install: true,
)
