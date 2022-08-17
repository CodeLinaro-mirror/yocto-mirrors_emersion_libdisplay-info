di_edid_decode = executable(
	'di-edid-decode',
	[
		'cta.c',
		'displayid.c',
		'edid.c',
		'main.c',
	],
	dependencies: [di_dep, math],
	install: true,
)

if target_machine.system() == 'emscripten'
	fs = import('fs')

	foreach filename : ['index.html', 'main.js', 'style.css']
		fs.copyfile('web' / filename)
	endforeach
endif
