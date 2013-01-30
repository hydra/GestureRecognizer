#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "uWave.h"

#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void generateSamples(void) {

  while(accIndex < MAX_ACC_LEN) {

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    printf("a/g: %6hd %6hd %6hd   %6hd %6hd %6hd\n",ax,ay,az,gx,gy,gz);

    accBuffer[accIndex][0] = ax*10;
    accBuffer[accIndex][1] = ay*10;
    accBuffer[accIndex][2] = (az-1)*10;
    accIndex++;
  }
  printf("\n");

}

void setup() {
    // initialize device
    printf("Initializing I2C devices...\n");
    accelgyro.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");
}


int main (int argc, char *argv[]) {

  setup();

  if (argc > 1 && strcasecmp(argv[1], "RECORD") == 0) {
    printf("recording\n");
    recordFlag = 1;
  }

  beginGesture();
  generateSamples();
  int result = endGesture();
  printf("result: %d\n", result);
	return 0;
}
