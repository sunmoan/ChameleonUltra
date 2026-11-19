/* cc -std=c11 -Wall -Wextra -Werror tests/slot_ibeacon_test.c -o /tmp/test-slot-ibeacon && /tmp/test-slot-ibeacon */
#include <assert.h>
#include "../src/slot_ibeacon.h"

int main(void) {
    _Static_assert(sizeof(slot_ibeacon_record_t) == 32, "NTAG213 pages 4-11");
    _Static_assert(SLOT_IBEACON_ADV_LENGTH == 30, "Legacy iBeacon advertising size");
    slot_ibeacon_record_t record = {
        .advertising_data = {
            0x02, 0x01, 0x06, 0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15,
            0x7c, 0x9a, 0x8f, 0x42, 0x3d, 0xe6, 0x4a, 0x91,
            0xbd, 0x75, 0x2c, 0x61, 0xf4, 0xe8, 0x90, 0x13,
            0x00, 0x01, 0x00, 0x02, 0xc5,
        },
        .padding = {0, 0},
    };
    uint8_t actual[SLOT_IBEACON_ADV_LENGTH] = {0};
    assert(slot_ibeacon_encode(&record, actual));
    assert(memcmp(actual, record.advertising_data, sizeof(actual)) == 0);

    /* UUID, Major, Minor and Tx Power must be taken from NFC, not constants. */
    record.advertising_data[9] = 0x12;
    record.advertising_data[24] = 0x34;
    record.advertising_data[25] = 0x56;
    record.advertising_data[26] = 0x78;
    record.advertising_data[27] = 0x9a;
    record.advertising_data[28] = 0xbc;
    record.advertising_data[29] = 0xde;
    assert(slot_ibeacon_encode(&record, actual));
    assert(memcmp(actual, record.advertising_data, sizeof(actual)) == 0);
    assert(actual[9] == 0x12 && actual[24] == 0x34);
    assert(actual[25] == 0x56 && actual[26] == 0x78);
    assert(actual[27] == 0x9a && actual[28] == 0xbc && actual[29] == 0xde);

    /* Reject malformed AD framing and nonzero padding. */
    record.advertising_data[4] = 0x00;
    assert(!slot_ibeacon_encode(&record, actual));
    record.advertising_data[4] = 0xff;
    record.advertising_data[0] = 0x03;
    assert(!slot_ibeacon_encode(&record, actual));
    record.advertising_data[0] = 0x02;
    record.padding[1] = 0x01;
    assert(!slot_ibeacon_encode(&record, actual));
    record.padding[1] = 0x00;
    assert(slot_ibeacon_encode(&record, actual));
    assert(!slot_ibeacon_encode(NULL, actual));
    assert(!slot_ibeacon_encode(&record, NULL));
    return 0;
}
