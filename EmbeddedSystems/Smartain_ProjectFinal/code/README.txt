**************************************************************
This is the instructions for running the code for "Smartain"
**************************************************************
Step 1 : make file

cd ./code/km
make clean
make
cd ./code/ul
make clean
make
cd ./code/led
make clean
make
**************************************************************
Step 2 : Open minicom

minicom
**************************************************************
Step 3 : Calibrate LCD screen

cd /usr/lib
	ln -s /media/card/lib/libQtCore.so.4 libQtCore.so.4
	ln -s /media/card/lib/libQtGui.so.4 libQtGui.so.4
	ln -s /media/card/lib/libQtNetwork.so.4 libQtNetwork.so.4
	ln -s /media/card/lib/ld-uClibc.so.0 ld-uClibc.so.0
	ln -s /media/card/lib/libc.so.0 libc.so.0
	ln -s /media/card/lib/libm.so.0 libm.so.0
	ln -s /media/card/lib/libstdc\+\+.so.6 libstdc\+\+.so.6
	export QWS_MOUSE_PROTO='tslib:/dev/input/touchscreen0'
	export TSLIB_CONFFILE=/etc/ts.conf
	export TSLIB_PLUGINDIR=/usr/lib
	export TSLIB_TSDEVICE=/dev/input/event0
	export TSLIB_FBDEVICE=/dev/fb0
	export TSLIB_CONSOLEDEVICE=/dev/tty
	export QT_QWS_FONTDIR=/media/card/lib/fonts
	export TSLIB_PLUGINDIR=/usr/lib/ts
	ts_calibrate
**************************************************************
Step 4 : Run our code

cd ~
mknod /dev/mygpio c 61 0
insmod mygpio.ko
./light &
./qt -qws
**************************************************************
Step 5: Start playing !
**************************************************************
Notice!
Necessary lib files: fonts libQtCore.so.4 libQtGui.so.4 libQtNetwork.so.4 ld-uClibc.so.0 libc.so.0 libm.so.0 libstdc++.so.6
should be included inside the Library
