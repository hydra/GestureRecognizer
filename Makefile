all: uWave 

HDRS = uWave.h I2Cdev.h MPU6050.h
COMMON_OBJS = uWave.o I2Cdev.o MPU6050.o 
MAIN_OBJS = main.o

CXXFLAGS = -DDMP_FIFO_RATE=9 -Wall -g -O2 

$(COMMON_OBJS) $(MAIN_OBJS) : $(HDRS)

uWave: $(COMMON_OBJS) $(MAIN_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(COMMON_OBJS) $(MAIN_OBJS) uWave

