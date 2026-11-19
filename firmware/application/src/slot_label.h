#ifndef SLOT_LABEL_H
#define SLOT_LABEL_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SLOT_LABEL_MAX_LENGTH 8

/* NTAG213 user pages 4–6. The label is printable ASCII, padded with zeroes. */
typedef struct {
    uint8_t magic[4];
    uint8_t text[SLOT_LABEL_MAX_LENGTH];
} slot_label_record_t;

static inline size_t slot_label_decode(const slot_label_record_t *record,
                                       uint8_t label[SLOT_LABEL_MAX_LENGTH]) {
    if (memcmp(record->magic, "LAB1", sizeof(record->magic)) != 0) {
        return 0;
    }
    size_t length = 0;
    for (size_t i = 0; i < sizeof(record->text); i++) {
        uint8_t ch = record->text[i];
        if (ch == 0) {
            continue;
        }
        if (length != i || ch < 0x20 || ch > 0x7e) {
            return 0;
        }
        length++;
    }
    memcpy(label, record->text, length);
    return length;
}

#endif
