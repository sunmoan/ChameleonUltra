/* cc -std=c11 -Wall -Wextra -Werror tests/slot_ble_policy_test.c -o /tmp/test-slot-ble-policy && /tmp/test-slot-ble-policy */
#include <assert.h>
#include "../src/slot_ble_policy.h"

int main(void) {
    // No advertising of any kind is allowed in slots 1 through 7.
    for (int phase = ADV_OFF; phase <= ADV_SLOT_BEACON; phase++) {
        for (int connected = 0; connected <= 1; connected++) {
            for (int beacon_valid = 0; beacon_valid <= 1; beacon_valid++) {
                for (int elapsed = 0; elapsed <= 1; elapsed++) {
                    assert(slot_ble_next_phase((advertising_phase_t)phase, connected,
                                               false, beacon_valid, elapsed) == ADV_OFF);
                }
            }
        }
    }

    // Slot 8 allows normal GUI discovery, even before a valid beacon is saved.
    assert(slot_ble_next_phase(ADV_OFF, false, true, false, false) == ADV_GUI);
    assert(slot_ble_next_phase(ADV_OFF, false, true, true, false) == ADV_GUI);
    assert(slot_ble_next_phase(ADV_GUI, false, true, false, true) == ADV_GUI);

    // With a valid beacon, slot 8 alternates GUI discovery and beacon packets.
    assert(slot_ble_next_phase(ADV_GUI, false, true, true, false) == ADV_GUI);
    assert(slot_ble_next_phase(ADV_GUI, false, true, true, true) == ADV_SLOT_BEACON);
    assert(slot_ble_next_phase(ADV_SLOT_BEACON, false, true, true, true) == ADV_GUI);

    // With an existing GUI connection, only the valid beacon is advertised.
    assert(slot_ble_next_phase(ADV_GUI, true, true, true, false) == ADV_SLOT_BEACON);
    assert(slot_ble_next_phase(ADV_SLOT_BEACON, true, true, false, false) == ADV_OFF);

    // Leaving slot 8 stops advertising immediately, without waiting for the timer.
    assert(slot_ble_next_phase(ADV_SLOT_BEACON, false, false, true, false) == ADV_OFF);
    assert(slot_ble_next_phase(ADV_GUI, false, false, true, false) == ADV_OFF);
    return 0;
}
