#ifndef SLOT_BLE_POLICY_H
#define SLOT_BLE_POLICY_H

#include <stdbool.h>

// Only the user-visible eighth slot is allowed to advertise over BLE.
typedef enum {
    ADV_OFF,
    ADV_GUI,
    ADV_SLOT_BEACON,
} advertising_phase_t;

static inline advertising_phase_t slot_ble_next_phase(advertising_phase_t current,
                                                      bool connected,
                                                      bool slot8_selected,
                                                      bool beacon_valid,
                                                      bool elapsed) {
    if (!slot8_selected) {
        return ADV_OFF;  // Stop all BLE advertising, including GUI discovery.
    }
    if (connected) {
        return beacon_valid ? ADV_SLOT_BEACON : ADV_OFF;
    }
    if (!beacon_valid || current == ADV_OFF) {
        return ADV_GUI;  // Keep the GUI discoverable on slot 8 during setup.
    }
    if (!elapsed) {
        return current;
    }
    return current == ADV_GUI ? ADV_SLOT_BEACON : ADV_GUI;
}

#endif
