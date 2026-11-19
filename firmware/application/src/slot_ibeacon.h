#ifndef SLOT_IBEACON_H
#define SLOT_IBEACON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SLOT_IBEACON_ADV_LENGTH 30U

/* NTAG213 pages 4-6: IBC1 marker, major, minor, measured power, padding. */
typedef struct {
    uint8_t magic[4];
    uint8_t major[2];
    uint8_t minor[2];
    uint8_t measured_power;
    uint8_t reserved[3];
} slot_ibeacon_record_t;

/* Produce the complete 30-byte legacy advertising data for a lab-only UUID.
 * The iBeacon prefix and UUID are intentionally fixed; the NFC record controls
 * only the major/minor identifiers and calibrated RSSI at 1 metre. */
static inline bool slot_ibeacon_encode(const slot_ibeacon_record_t *record,
                                       uint8_t advertising_data[SLOT_IBEACON_ADV_LENGTH]) {
    static const uint8_t prefix_and_uuid[] = {
        0x02, 0x01, 0x06,                         /* LE general-discoverable flags */
        0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15,       /* iBeacon manufacturer data */
        0x7c, 0x9a, 0x8f, 0x42, 0x3d, 0xe6, 0x4a, 0x91,
        0xbd, 0x75, 0x2c, 0x61, 0xf4, 0xe8, 0x90, 0x13,
    };
    static const uint8_t zero_padding[3] = {0};

    if (memcmp(record->magic, "IBC1", sizeof(record->magic)) != 0 ||
            memcmp(record->reserved, zero_padding, sizeof(record->reserved)) != 0) {
        return false;
    }
    memcpy(advertising_data, prefix_and_uuid, sizeof(prefix_and_uuid));
    memcpy(advertising_data + 25, record->major, sizeof(record->major));
    memcpy(advertising_data + 27, record->minor, sizeof(record->minor));
    advertising_data[29] = record->measured_power;
    return true;
}

#endif
