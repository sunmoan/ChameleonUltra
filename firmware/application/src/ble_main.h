#ifndef BLE_MAIN_H
#define BLE_MAIN_H

#include "ble_bas.h"
#include "ble_gatts.h"
#include "ble_nus.h"
#include "nrfx_saadc.h"

extern uint16_t batt_lvl_in_milli_volts;
extern uint8_t percentage_batt_lvl;

typedef void (*lf_adc_callback_t)(nrf_saadc_value_t *, size_t);

void ble_slave_init(void);
void advertising_start(bool erase_bonds);
void advertising_stop(void);
void advertising_update_slot_ibeacon(bool slot8_selected, const uint8_t *advertising_data, size_t length);
/* USB-only lab controls: 5 bytes are Major (2), Minor (2), Measured Power (1). */
bool ble_usb_lab_beacon_set(const uint8_t fields[5]);
void ble_usb_lab_beacon_clear(void);
void ble_usb_lab_beacon_get(uint8_t status[6]);
void delete_bonds_all(void);
void nus_data_response(uint8_t *p_data, uint16_t length);
bool is_nus_working(void);
void set_ble_connect_key(uint8_t *key);

void register_lf_adc_callback(lf_adc_callback_t cb);
void unregister_lf_adc_callback(void);

#endif
