/* Run from firmware/application with cc -std=c11 tests/slot_label_test.c -o /tmp/test-slot-label && /tmp/test-slot-label */
#include <assert.h>
#include "../src/slot_label.h"

int main(void) {
    _Static_assert(sizeof(slot_label_record_t) == 12, "The label occupies pages 4 through 6");
    uint8_t label[SLOT_LABEL_MAX_LENGTH];
    slot_label_record_t record = { .magic = "LAB1", .text = "CM-LAB" };
    assert(slot_label_decode(&record, label) == 6);
    assert(memcmp(label, "CM-LAB", 6) == 0);

    memcpy(record.text, "~123456 ", sizeof(record.text));
    assert(slot_label_decode(&record, label) == sizeof(label));
    assert(memcmp(label, "~123456 ", sizeof(label)) == 0);
    record.text[1] = 0;
    assert(slot_label_decode(&record, label) == 0);  /* Nonzero byte after padding */
    record.text[1] = 0x7f;
    assert(slot_label_decode(&record, label) == 0);

    memset(record.text, 0, sizeof(record.text));
    assert(slot_label_decode(&record, label) == 0);
    record.text[0] = 'X';
    record.magic[3] = '2';
    assert(slot_label_decode(&record, label) == 0);
    return 0;
}
