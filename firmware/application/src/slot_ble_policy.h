#ifndef SLOT_BLE_POLICY_H
#define SLOT_BLE_POLICY_H

#include <stdbool.h>

// The ordinary BLE UART/GUI discovery service stays available on slots 1-7.
// Slot 8 replaces GUI discovery with a nonconnectable iBeacon when configured.
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
    (void)current;
    (void)elapsed;
    if (slot8_selected && beacon_valid) {
        return ADV_SLOT_BEACON;
    }
    // When slot 8 has no valid beacon, GUI discovery remains available for
    // configuration. Connected BLE links do not need connectable advertising.
    return connected ? ADV_OFF : ADV_GUI;
}

#endif
