all: uWave gestureRecognizer

HDRS = uWave.h I2Cdev.h MPU6050.h GestureRecognizer.h Sampler.h AccelerationData.h AccelerationDataStore.h Gesture.h DataQuantizer.h ScheduledAction.h Config.h 

COMMON_OBJS = I2Cdev.o MPU6050.o
GESTURERECOGNIZER_OBJS = GestureRecognizer.o Sampler.o AccelerationDataStore.o DataQuantizer.o Gesture.o ScheduledAction.o GestureRecognizerApp.o
UWAVE_OBJS = uWave.o uWaveApp.o

CXXFLAGS = -DDMP_FIFO_RATE=9 -Wall -g -O2 

$(GESTURERECOGNIZER_OBJS) $(UWAVE_OBJS) $(COMMON_OBJS) : $(HDRS)

uWave: $(UWAVE_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $^ -lm

gestureRecognizer: $(GESTURERECOGNIZER_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(UWAVE_OBJS) $(GESTURERECOGNIZER_OBJS) $(COMMON_OBJS) uWave gestureRecognizer

