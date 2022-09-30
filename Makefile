all: clean web desktop

web: web_reload web_build

desktop: desktop_reload desktop_build

clean: web_clean desktop_clean

desktop_clean:
	rm -rf cmake-build-desktop

desktop_reload:
	cmake -Bcmake-build-desktop

desktop_build:
	cmake --build cmake-build-desktop -j 4

web_clean:
	rm -rf cmake-build-web

web_reload:
	emcmake cmake -Bcmake-build-web -DPLATFORM=Web

web_clean_js:
	rm cmake-build-web/adore2d.js

web_build: web_reload
	cmake --build cmake-build-web -j 4

vendor_zip:
	zip -r vendor vendor.zip

vendor_unzip:
	unzip vendor.zip

run: desktop
	./cmake-build-desktop/adore2d examples/lua/main.lua