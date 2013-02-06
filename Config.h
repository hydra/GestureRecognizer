
#ifndef CONFIG_H
#define CONFIG_H

//GestureRecognizer
#define MAX_GESTURES 32
#define SAMPLE_FREQUENCY_HZ 100

//Averager
#define SAMPLE_WINDOW ((SAMPLE_FREQUENCY_HZ / 25) * 2)
#define SAMPLE_STEP (SAMPLE_WINDOW / 2)

//AccelerationDataStore
#define MAX_ACCELERATION_ITEMS ((SAMPLE_FREQUENCY_HZ / 25) * SAMPLE_STEP)

//#define USE_DATA_QUANTIZER

#endif
