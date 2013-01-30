all: uWave gestureRecognizer

HDRS = uWave.h I2Cdev.h MPU6050.h GestureRecognizer.h Averager.h AccelerationDataStore.h Gesture.h

COMMON_OBJS = I2Cdev.o MPU6050.o
GESTURERECOGNIZER_OBJS = GestureRecognizer.o Averager.o AccelerationDataStore.o Gesture.o GestureRecognizerApp.o
UWAVE_OBJS = uWave.o uWaveApp.o

CXXFLAGS = -DDMP_FIFO_RATE=9 -Wall -g -O2 

$(COMMON_OBJS) $(GESTURERECOGNIZER_OBJS) $(UWAVE_OBJS) : $(HDRS)

uWave: $(COMMON_OBJS) $(UWAVE_OBJS)
	$(CXX) -o $@ $^ -lm

gestureRecognizer: $(COMMON_OBJS) $(GESTURERECOGNIZER_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(COMMON_OBJS) $(UWAVE_OBJS) $(GESTURERECOGNIZER_OBJS) uWave gestureRecognizer

