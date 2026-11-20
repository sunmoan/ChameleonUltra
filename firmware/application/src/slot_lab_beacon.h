#ifndef SLOT_LAB_BEACON_H
#define SLOT_LAB_BEACON_H

#include <stdint.h>
#include <string.h>

#define SLOT_LAB_BEACON_FIELDS_LENGTH 5U
#define SLOT_LAB_BEACON_ADV_LENGTH 30U

/* Own lab UUID only: 7c9a8f42-3de6-4a91-bd75-2c61f4e89013.
 * The host can modify Major, Minor and calibrated RSSI, not the UUID or
 * the iBeacon framing. This does not persist in the NTAG213 dump. */
static inline void slot_lab_beacon_build(
        uint8_t out[SLOT_LAB_BEACON_ADV_LENGTH],
        const uint8_t fields[SLOT_LAB_BEACON_FIELDS_LENGTH]) {
    static const uint8_t fixed_prefix[25] = {
        0x02, 0x01, 0x06, 0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15,
        0x7c, 0x9a, 0x8f, 0x42, 0x3d, 0xe6, 0x4a, 0x91,
        0xbd, 0x75, 0x2c, 0x61, 0xf4, 0xe8, 0x90, 0x13,
    };
    memcpy(out, fixed_prefix, sizeof(fixed_prefix));
    memcpy(out + sizeof(fixed_prefix), fields, SLOT_LAB_BEACON_FIELDS_LENGTH);
}

#endif
