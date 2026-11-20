/* cc -std=c11 -Wall -Wextra -Werror tests/slot_lab_beacon_test.c -o /tmp/test-lab-beacon && /tmp/test-lab-beacon */
#include <assert.h>
#include "../src/slot_lab_beacon.h"

int main(void) {
    _Static_assert(SLOT_LAB_BEACON_ADV_LENGTH == 30, "Legacy iBeacon size");
    const uint8_t fields[5] = {0x00, 0x01, 0x00, 0x02, 0xc5};
    const uint8_t expected[30] = {
        0x02, 0x01, 0x06, 0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15,
        0x7c, 0x9a, 0x8f, 0x42, 0x3d, 0xe6, 0x4a, 0x91,
        0xbd, 0x75, 0x2c, 0x61, 0xf4, 0xe8, 0x90, 0x13,
        0x00, 0x01, 0x00, 0x02, 0xc5,
    };
    uint8_t actual[30];
    slot_lab_beacon_build(actual, fields);
    assert(memcmp(actual, expected, sizeof(expected)) == 0);
    uint8_t more_fields[5] = {0xfe, 0xdc, 0x12, 0x34, 0xb0};
    slot_lab_beacon_build(actual, more_fields);
    assert(memcmp(actual, expected, 25) == 0); // Immutable test UUID and header.
    assert(memcmp(actual + 25, more_fields, 5) == 0);
    return 0;
}
