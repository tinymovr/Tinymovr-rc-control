#include <InterpolationLib.h>
#include <PPMReader.h>

// Initialize a PPMReader on digital pin 3 with 6 expected channels.
byte interruptPin = 3;
byte channelAmount = 6;
PPMReader ppm(interruptPin, channelAmount);

// Setup the interpolator
const int numValues = 2;
double xValues[10] = {   1000,  2000 }; // RC signal range
double yValues[10] = { -50000, 50000 }; // Motor desired velocity range

void setup() {
    Serial.begin(115200);
    delay(100);
    // Immediately set Tinymovr to closed loop control state
    Serial.print(".A\n");
}

void loop() {
    // Read value from channel (0 in this case)
    uint8_t channel = 0;
    double xValue = (double)ppm.latestValidChannelValue(channel, 0);

    // If value is within range, print it out to serial as a command
    if (xValue >= 1000 && xValue <= 2000) {
        int32_t vel_out = (int32_t)Interpolation::Linear(xValues, yValues, numValues, xValue, false);
        Serial.printf(".V%d\n", vel_out);
    }

    // 200Hz = 5ms delay
    delay(5);
}
