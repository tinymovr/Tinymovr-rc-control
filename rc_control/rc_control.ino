#include <PulsePosition.h>
#include <PulsePositionIMXRT.h>

#include <InterpolationLib.h>

byte rxPin = 5;
PulsePositionInput myInput(FALLING);

// Setup the interpolator
const int numValues = 2;
double xValues[10] = {   1000,  2000 }; // RC signal range
double yValues[10] = { -100000, 100000 }; // Motor desired velocity range
//double yValues[10] = { -300000, 300000 }; // More aggressive limits

void setup() {
  myInput.begin(rxPin);
    //                                                                                                                                                                                                                     Serial.begin(115200);
    Serial2.begin(115200);
    delay(100);
    // Immediately set Tinymovr to closed loop control state
    //Serial.print(".A\n");
    Serial2.print(".A\n");
    delay(10);
}

void loop() {
    // Read value from channel (0 in this case)
    uint8_t channel = 3;
    float xValue = myInput.read(channel);

    // If value is within range, print it out to serial as a command
    if (xValue >= 1000 && xValue <= 2000) {
        int32_t vel_out = (int32_t)Interpolation::Linear(xValues, yValues, numValues, xValue, false);
        //Serial.printf(".V%d\n", vel_out);
        Serial2.printf(".V%d\n", vel_out);
    }

    // 100Hz = 10ms delay
    delay(10);
}
