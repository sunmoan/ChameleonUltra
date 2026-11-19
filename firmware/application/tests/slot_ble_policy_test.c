/* cc -std=c11 -Wall -Wextra -Werror tests/slot_ble_policy_test.c -o /tmp/test-slot-ble-policy && /tmp/test-slot-ble-policy */
#include <assert.h>
#include "../src/slot_ble_policy.h"

int main(void) {
    // Every normal NFC slot must retain standard GUI advertising when idle.
    for (int phase = ADV_OFF; phase <= ADV_SLOT_BEACON; phase++) {
        for (int beacon_valid = 0; beacon_valid <= 1; beacon_valid++) {
            for (int elapsed = 0; elapsed <= 1; elapsed++) {
                assert(slot_ble_next_phase((advertising_phase_t)phase, false,
                                           false, beacon_valid, elapsed) == ADV_GUI);
                // The GUI connection already exists: no new connectable ads.
                assert(slot_ble_next_phase((advertising_phase_t)phase, true,
                                           false, beacon_valid, elapsed) == ADV_OFF);
            }
        }
    }

    // A valid slot-8 beacon replaces connectable GUI discovery immediately.
    for (int phase = ADV_OFF; phase <= ADV_SLOT_BEACON; phase++) {
        for (int connected = 0; connected <= 1; connected++) {
            for (int elapsed = 0; elapsed <= 1; elapsed++) {
                assert(slot_ble_next_phase((advertising_phase_t)phase, connected,
                                           true, true, elapsed) == ADV_SLOT_BEACON);
            }
        }
    }

    // Empty/unconfigured slot 8 can still be reached by the GUI.
    assert(slot_ble_next_phase(ADV_OFF, false, true, false, false) == ADV_GUI);
    assert(slot_ble_next_phase(ADV_SLOT_BEACON, false, true, false, false) == ADV_GUI);
    assert(slot_ble_next_phase(ADV_SLOT_BEACON, true, true, false, false) == ADV_OFF);

    // Returning to slots 1-7 restores GUI discovery immediately.
    assert(slot_ble_next_phase(ADV_SLOT_BEACON, false, false, true, false) == ADV_GUI);
    return 0;
}
