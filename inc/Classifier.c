

#include <stdint.h>
#include "../inc/Classifier.h"

// All given
#define SIDEMIN    100   // smallest side distance to the wall in mm
#define SIDEMAX    400   // largest side distance to wall in mm
#define CENTERMIN  100   // min distance to the wall in the front
#define CENTEROPEN 600   // distance to the wall between open/blocked
#define IRMIN      50    // min possible reading of IR sensor
#define IRMAX      800   // max possible reading of IR sensor

scenario_t Classify(int32_t left_mm, int32_t center_mm, int32_t right_mm) {

    // Check if IR sensor readings are invalid first
    if ((left_mm   < IRMIN) || (left_mm   > IRMAX) ||
        (center_mm < IRMIN) || (center_mm > IRMAX) ||
        (right_mm  < IRMIN) || (right_mm  > IRMAX)) {
        return ClassificationError;
    }

    if (center_mm < CENTEROPEN) {

        if ((left_mm < SIDEMAX) && (right_mm < SIDEMAX)) {
            return Blocked;
        }
        else if ((left_mm < SIDEMAX) && (right_mm >= SIDEMAX)) {
            return RightTurn;
        }
        else if ((left_mm >= SIDEMAX) && (right_mm < SIDEMAX)) {
            return LeftTurn;
        }
        else {
            return TeeJoint;
        }

    }

    else {

        if ((left_mm < SIDEMAX) && (right_mm < SIDEMAX)) {
            return Straight;
        }
        else if ((left_mm < SIDEMAX) && (right_mm >= SIDEMAX)) {
            return RightJoint;
        }
        else {
            return LeftJoint;
        }
    }
}

