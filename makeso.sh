gcc -o libHduWM256x86.so WatermarkAPI.o consts_values.o libwm.o -I/usr/local/include/opencv4 -fPIC -L./lib -lopencv_imgproc -lopencv_core -lstdc++ -lm -lz -ldl -lrt -lpthread -shared

