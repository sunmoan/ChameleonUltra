#ifndef SLOT_IBEACON_H
#define SLOT_IBEACON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SLOT_IBEACON_ADV_LENGTH 30U

/* NTAG213 user pages 4-11: all 30 on-air bytes plus two zero padding bytes.
 * No beacon fields are synthesized or overridden by the firmware. */
typedef struct {
    uint8_t advertising_data[SLOT_IBEACON_ADV_LENGTH];
    uint8_t padding[2];
} slot_ibeacon_record_t;

/* Accept a standard legacy iBeacon advertisement and copy the 30 bytes
 * verbatim. The first 9 bytes are the mandatory iBeacon AD framing; UUID,
 * Major, Minor and measured power all come from the NTAG213 dump. */
static inline bool slot_ibeacon_encode(const slot_ibeacon_record_t *record,
                                       uint8_t advertising_data[SLOT_IBEACON_ADV_LENGTH]) {
    static const uint8_t ibeacon_header[9] = {
        0x02, 0x01, 0x06,                         /* BLE Flags */
        0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15,       /* iBeacon manufacturer AD */
    };
    static const uint8_t zero_padding[2] = {0};

    if (record == NULL || advertising_data == NULL ||
            memcmp(record->advertising_data, ibeacon_header, sizeof(ibeacon_header)) != 0 ||
            memcmp(record->padding, zero_padding, sizeof(zero_padding)) != 0) {
        return false;
    }
    memcpy(advertising_data, record->advertising_data, SLOT_IBEACON_ADV_LENGTH);
    return true;
}

#endif
