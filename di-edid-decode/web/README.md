# Webapp

To build `di-edid-decode` as a webapp, install Emscripten and run:

    meson setup build-wasm/ --cross-file di-edid-decode/web/wasm.txt
    ninja -C build-wasm/

Then start a web server serving static files in `build-wasm/di-edid-decode/`,
for instance:

    python -m http.server --directory build-wasm/di-edid-decode/
