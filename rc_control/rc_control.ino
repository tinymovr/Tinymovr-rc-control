#include <PulsePosition.h>
#include <PulsePositionIMXRT.h>

#include <InterpolationLib.h>

byte rxPin = 5;
PulsePositionInput myInput(FALLING);

uint8_t channel_mode = 2; // set to mode switch channel
uint8_t channel_throttle = 3; // set to throttle channel

uint8_t current_mode = 0; // 0=velocity or 1=position
int32_t current_pos = 0; // current position


// Setup the interpolator
const int numValues = 2;
double xValues[10] = {   1000,  2000 }; // RC signal range
double vel_yValues[10] = { -100000, 100000 }; // Motor desired velocity range
double pos_yValues[10] = { -20000, 20000 }; // Motor desired position range

void setup() {
  myInput.begin(rxPin);
    //                                                                                                                                                                                                                     Serial.begin(115200);
    Serial2.begin(115200);
    delay(100);
    // Immediately set Tinymovr to closed loop control state
    Serial2.print(".A\n");
    delay(10);
}

void loop() {
    
    float mValue = myInput.read(channel_mode);
    if (mValue < 1300 && 1 == current_mode) {
        current_mode = 0;
    }
    else if (mValue > 1700 && 0 == current_mode) {
        // When mode changes to position, request and
        // store the current position immediately, then
        // change mode
        Serial2.print(".p\n");
        current_pos = parseInt();
        current_mode = 1;
    }
    
    float throttleValue = myInput.read(channel_throttle);

    // If value is within range, print it out to serial as a command
    if (throttleValue >= 1000 && throttleValue <= 2000) {
        if (0 == current_mode) {
            int32_t vel_out = (int32_t)Interpolation::Linear(xValues, vel_yValues, numValues, throttleValue, false);
            Serial2.printf(".V%d\n", vel_out);
        } else if (1 == current_mode) {
            int32_t pos_out = (int32_t)Interpolation::Linear(xValues, pos_yValues, numValues, throttleValue, false);
            Serial2.printf(".P%d\n", current_pos + pos_out);
        }
        
    }

    // 100Hz = 10ms delay
    delay(10);
}
