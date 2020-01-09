# OpenWatch
Open source watch based on [https://github.com/S-March/smarchWatch_PUBLIC](smarchWatch), da14683 ic. It uses some files Samson wrote, but most are heavily changed or created by me. I did not want to fork it, since it's a different watch (on the same pcb but with touch connector). Also started this repo just for myself but decided to share it.

![Picture of the watch and os](https://www.digitalfactor.nl/watch/watch.jpg)

It uses [this touchscreen](https://www.topfoison.com/Products-TF13016A-C.html) (because other screens can't fold the connector around the display. 

It does work with [this one](https://www.dhgate.com/product/1-3-inch-240-240-round-ips-tft-lcd-module/411021741.html#seo=WAP) but that won't fit the housing and can't fold nicely flat

No tutorial yet but as a start follow this

Download the Dialog Semi DA14683 SDK
Install SmartSnippetsStudio [found here](https://www.dialog-semiconductor.com/products/connectivity/bluetooth-low-energy/smartbond-da14682-and-da14683), scroll down for your platform.
Follow the [getting started guide found here](https://www.dialog-semiconductor.com/sites/default/files/um-b-047-da1468x_getting_started_7v0.pdf)

Connect a J-Link mini or similar to the pads, and don't forget to connect VREF to VBAT somewhere on the board.
For debugging connect the RX TX too. Maybe best to start with the [Development Kit USB](https://www.dialog-semiconductor.com/products/da14683-development-kit-usb)

You can use Smart Snipets Toolbox (after running the code once, it creates the partitions on flash) to load the images (picturefiles.txt) to the flash partition (NVMS_FLASH_STORAGE). These where created with Samson's biptmap loader tool found in the original git.