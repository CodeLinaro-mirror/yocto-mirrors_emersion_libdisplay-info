let appElt = document.getElementById('app');
let statusElt = document.getElementById('status');
let fileInput = document.getElementById('file-input');
let outputElt = document.getElementById('output');

function handleFileChange() {
	if (fileInput.files.length === 0) {
		return;
	}
	let file = fileInput.files[0];
	let reader = new FileReader();
	reader.addEventListener('load', () => {
		let filename = 'edid';
		FS.writeFile(filename, new Uint8Array(reader.result));

		outputElt.innerHTML = '';

		let argv = ['di-edid-decode', filename];

		let ptrSize = 4;
		let cArgv = Module._malloc(argv.length * ptrSize);
		for (let i = 0; i < argv.length; i++) {
			let str = argv[i];
			let cStr = Module._malloc(str.length + 1);
			Module.stringToUTF8(str, cStr, str.length + 1);
			Module.setValue(cArgv + i * ptrSize, cStr, '*');
		}

		let ret = Module._main(argv.length, cArgv);
		console.log('di-edid-decode exited with code ' + ret);

		for (let i = 0; i < argv.length; i++) {
			let cStr = Module.getValue(cArgv + i * ptrSize, '*');
			Module._free(cStr);
		}
		Module._free(cArgv);
	});
	reader.readAsArrayBuffer(file);
}

fileInput.addEventListener('change', handleFileChange);

window.Module = {
	noInitialRun: true,
	setStatus: (text) => {
		console.log(text || 'Done');
		statusElt.innerText = text;
		statusElt.style.display = text ? 'block' : 'none';
	},
	print: (text) => {
		outputElt.appendChild(document.createTextNode(text + '\n'));
	},
	printErr: (text) => {
		let elt = document.createElement('span');
		elt.className = 'error';
		elt.innerText = text + '\n';
		outputElt.appendChild(elt);
	},
	onRuntimeInitialized: () => {
		app.style.display = 'block';
		handleFileChange();
	},
	onAbort: (text) => {
		Module.setStatus('Abort: ' + text);
	},
};

window.addEventListener('error', (event) => {
	Module.setStatus('Error: ' + event.message);
});
