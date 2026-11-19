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

## Slot 8 BLE iBeacon (30-byte legacy advertising)

This fork broadcasts a **standard 30-byte iBeacon advertising payload** using
a dedicated lab UUID (`7c9a8f42-3de6-4a91-bd75-2c61f4e89013`). The UUID is
fixed in the firmware; the existing ChameleonUltraGUI controls the **Major**,
**Minor**, and **measured power** fields through an NTAG213 dump in slot 8.

In ChameleonUltraGUI, open **Slot Manager → Slot 8 → Settings**, choose
**NTAG213** for HF, enable HF, then edit the HF dump's user pages 4–6:

| NTAG213 page | Hex bytes | Meaning |
| --- | --- | --- |
| 4 | `49 42 43 31` | ASCII `IBC1` marker (enables beacon mode) |
| 5 | `00 01 00 02` | Major = 1, Minor = 2 (big-endian) |
| 6 | `C5 00 00 00` | Measured power = -59 dBm (`C5`); reserved bytes must be zero |

Save the dump, select slot 8, and wake the device with a button or USB.
A BLE scanner that recognizes iBeacon should report the lab UUID, Major 1,
Minor 2 and measured power -59 dBm. The complete 30-byte advertisement is:

```text
02 01 06
1A FF 4C 00 02 15
7C 9A 8F 42 3D E6 4A 91 BD 75 2C 61 F4 E8 90 13
00 01 00 02 C5
```

This is **30 bytes of legacy advertising data**, including the 3-byte Flags
field, not 30 bytes of arbitrary user data. iBeacon advertisements are
nonconnectable. With no GUI connection, the single BLE advertising set
alternates between connectable GUI discovery and the nonconnectable beacon
every 500 ms. With the GUI connected, it transmits the beacon about every
100 ms. Selecting another slot, disabling HF, or clearing the `IBC1` marker
stops beacon transmission; normal device sleep still applies.

This is a lab beacon, not a credential or authentication mechanism. The
previous `LAB1` short-label Service Data format is not used by this version.

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
