/* cc -std=c11 -Wall -Wextra -Werror tests/slot_ibeacon_test.c -o /tmp/test-slot-ibeacon && /tmp/test-slot-ibeacon */
#include <assert.h>
#include "../src/slot_ibeacon.h"

int main(void) {
    _Static_assert(sizeof(slot_ibeacon_record_t) == 12, "NTAG213 pages 4-6");
    _Static_assert(SLOT_IBEACON_ADV_LENGTH == 30, "Legacy iBeacon advertising size");
    slot_ibeacon_record_t record = {
        .magic = {'I', 'B', 'C', '1'},
        .major = {0x00, 0x01},
        .minor = {0x00, 0x02},
        .measured_power = 0xc5,
        .reserved = {0, 0, 0},
    };
    uint8_t actual[SLOT_IBEACON_ADV_LENGTH] = {0};
    static const uint8_t expected[SLOT_IBEACON_ADV_LENGTH] = {
        0x02, 0x01, 0x06, 0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15,
        0x7c, 0x9a, 0x8f, 0x42, 0x3d, 0xe6, 0x4a, 0x91,
        0xbd, 0x75, 0x2c, 0x61, 0xf4, 0xe8, 0x90, 0x13,
        0x00, 0x01, 0x00, 0x02, 0xc5,
    };
    assert(slot_ibeacon_encode(&record, actual));
    assert(memcmp(actual, expected, sizeof(expected)) == 0);
    record.magic[3] = '2';
    assert(!slot_ibeacon_encode(&record, actual));
    record.magic[3] = '1';
    record.reserved[0] = 0x01;
    assert(!slot_ibeacon_encode(&record, actual));
    record.reserved[0] = 0;
    record.major[0] = 0xff;
    record.minor[1] = 0x80;
    assert(slot_ibeacon_encode(&record, actual));
    assert(actual[25] == 0xff && actual[28] == 0x80);
    return 0;
}
