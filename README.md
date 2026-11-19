![logo](docs/images/ultra-logo.png)

![ultra picture](docs/images/ultra-overview.png)

# ChameleonUltra Authorized Distributors

Lyon, France: [Lab401](https://lab401.com/)

Santa Ana, United States: [Hackerwarehouse](https://hackerwarehouse.com/)

Hastings, UK: [KSEC](https://labs.ksec.co.uk/product/proxgrind-chameleon-ultra/)

Montreal, Canada: [TechSecurityTools](https://techsecuritytools.com/product/chameleon-ultra/)

Shenzhen, China: [Sneaktechnology](https://sneaktechnology.com)

Guangdong, China: [MTools Tec](https://shop.mtoolstec.com/)

Lazada One, Singapore: [Aliexpress by RRG](https://proxgrind.aliexpress.com/store/1101312023)

# What is it and how to use ?

Read the [available documentation](https://github.com/RfidResearchGroup/ChameleonUltra/wiki).

## Slot 8 BLE iBeacon (30-byte editable advertisement)

This fork reads **all 30 bytes of a standard iBeacon legacy advertising packet**
directly from the active NTAG213 dump in slot 8. No UUID, Major, Minor, or
measured-power values are hardcoded into the transmitted advertisement.
The existing ChameleonUltraGUI is sufficient; no GUI changes are required.

In ChameleonUltraGUI, open **Slot Manager → Slot 8 → Settings**, select
**NTAG213** for HF, enable HF, and edit user pages **4 through 11** of the
HF dump. For example, the following 30-byte beacon uses the lab UUID
`7c9a8f42-3de6-4a91-bd75-2c61f4e89013`, Major 1, Minor 2, and -59 dBm
measured power:

| NTAG213 page | Four hex bytes | Contents |
| --- | --- | --- |
| 4 | `02 01 06 1A` | BLE flags + manufacturer AD length |
| 5 | `FF 4C 00 02` | Manufacturer AD type + Apple company ID + beacon type |
| 6 | `15 7C 9A 8F` | iBeacon length marker + UUID bytes 1-3 |
| 7 | `42 3D E6 4A` | UUID bytes 4-7 |
| 8 | `91 BD 75 2C` | UUID bytes 8-11 |
| 9 | `61 F4 E8 90` | UUID bytes 12-15 |
| 10 | `13 00 01 00` | UUID byte 16 + Major + first Minor byte |
| 11 | `02 C5 00 00` | Last Minor byte + measured power + two zero padding bytes |

The first **30 bytes** (pages 4-10 and the first two bytes of page 11) are
broadcast **verbatim**. The last two bytes of page 11 must be `00 00`
and are not broadcast. The first nine bytes must be the standard iBeacon
framing `02 01 06 1A FF 4C 00 02 15`; malformed framing or nonzero
padding disables beacon advertising. All remaining 21 bytes, including the
full UUID, Major, Minor and measured power, are read from the GUI-edited
dump, not from firmware constants.

Save the dump, select slot 8, and wake the device with a button or USB.
A BLE scanner should recognize the iBeacon with the values entered above.
**All BLE advertising is gated by the selected slot:**
- **Slot 8:** normal connectable GUI discovery is allowed, even when the
  NTAG213 beacon data is not configured yet. With a valid beacon, the single
  advertising set alternates between GUI discovery and nonconnectable
  iBeacon advertising every 500 ms while disconnected. During a GUI
  connection, the nonconnectable beacon is sent about every 100 ms.
- **Slots 1-7:** both iBeacon and normal GUI discovery advertising stop.
  Switching away from slot 8 stops advertising at the next main-loop update,
  without waiting for the 500 ms rotation. An existing BLE connection is not
  forcibly disconnected, but no new BLE connection can be initiated until
  slot 8 is selected again. Use the physical buttons or USB to switch slots
  if the device is not currently advertising.

Disabling HF or changing the standard iBeacon header stops iBeacon
transmission, but normal GUI discovery remains available on slot 8.
Normal device sleep still applies.

**Note:** This is a standard iBeacon encoder, not an arbitrary 30-byte BLE
advertising-data injector: the nine framing bytes must remain standard.
The previous `IBC1` 12-byte slot format is no longer supported.

# Compatible applications

* [ChameleonUltraGUI](https://github.com/GameTec-live/ChameleonUltraGUI)
* [ChameLab](https://github.com/m0n5ter/ChameLab)
* [Chameleon Ultra (Sailfish OS only)](https://sailfishos-chum.github.io/apps/harbour-chameleon-ultra)
* [Chameleon GO (iOS only)](https://apps.apple.com/us/app/id6802015306)
* [Device console for ChameleonUltra (Web)](https://sec1.dk/phreakbyte.html)
* [MTools BLE](https://github.com/RfidResearchGroup/ChameleonUltra/wiki/mtoolsble)
* [Mifare Chameleon Tool (iOS only, Beta)](https://apps.apple.com/it/app/mifare-chameleon-tool/id6761231484)

# Videos

*Beware some of the instructions might have changed since recording, check the current documentation when in doubt!*

* [Downloading and compiling the official CLI](https://www.youtube.com/watch?v=VGpAeitNXH0)
* [Downloading ChameleonUltraGUI](https://www.youtube.com/watch?v=rHH7iqbX3nY)
* [ChameleonUltraGUI features overview](https://www.youtube.com/watch?v=YqE8wyVSse4)
* [Using ChameleonUltraGUI and the Chameleon Ultra](https://www.youtube.com/watch?v=9jtKNJ5-kVY)
* [MTools BLE - How to clone a card with ChameleonUltra](https://youtu.be/IvH-xtdW1Wk?si=4exqgAAeJ-kxU3aN)

# Official channels

Where do you find the community?
* [RFID Hacking community discord server](https://t.ly/d4_C)
  * Software/chameleon-dev for firmware and clients development discussions
  * Devices/chameleon-ultra for usage discussions
* [GameTec_live discord server](https://discord.gg/DJ2A4wxncK)

Other notable projects:
* [Fantasi](https://fantasi.cloud)
  * [Discord](https://fantasi.cloud/discord)
* [chameleon-ultra.js](https://github.com/taichunmin/chameleon-ultra.js/) 

###### Searching for the docs repo? Find it [here](https://github.com/RfidResearchGroup/ChameleonUltraDocs)
