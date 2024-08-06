gcc -shared -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -I/usr/local/include/opencv4 -o libwmLibjni.so wmLibjni.cpp  -L./lib -lHduWM256x86 -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_core -llibjpeg-turbo -llibpng -llibtiff -lzlib

