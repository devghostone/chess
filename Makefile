BUILD_DIR="build"

setup:
	@git submodule update --init --recursive
	@echo "[✔️] Done setting up submodules"
	@./lib/emsdk/emsdk install latest
	@./lib/emsdk/emsdk activate latest
	@source "./lib/emsdk/emsdk_env.sh"

build:
	zig build -p ${BUILD_DIR}/native
	@echo "[✔️] Done building"

run:
	@zig build -p ${BUILD_DIR}/native run

build-windows:
	@zig build -Dtarget=x86_64-windows -p ${BUILD_DIR}/windows
	@echo "[✔️] Done building for windows"

build-web:
	@zig build -Dtarget=wasm32-emscripten -p ${BUILD_DIR}/web --sysroot "${EMSDK}/upstream/emscripten" --libc libc/web-libc.txt
	@echo "[✔️] Done building for web"

clean:
	@rm -rf zig-cache
	@echo "[✔️] Done removing cache directory"
	@rm -rf build
	@echo "[✔️] Done removing build directory"
	