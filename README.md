# OpenWatch
Open source watch based on [https://github.com/S-March/smarchWatch_PUBLIC](smarchWatch), da14683 ic. It uses some files Samson wrote, but most are heavily changed or created by me. I did not want to fork it, since it's a different watch (on the same pcb but with touch connector). Also started this repo just for myself but decided to share it.

![Picture of the watch and os](https://www.digitalfactor.nl/watch/watch2.jpg)
Small video of the watch in action [found here](https://streamable.com/83sec)

It uses [this touchscreen](https://www.topfoison.com/Products-TF13016A-C.html) (because other screens can't fold the connector around the display. 

It does work with [this one](https://www.dhgate.com/product/1-3-inch-240-240-round-ips-tft-lcd-module/411021741.html#seo=WAP) but that won't fit the housing and can't fold nicely flat

No finished tutorial yet but you can find a guide here:
https://github.com/dannygrob/OpenWatch/blob/master/OpenWatch%20getting%20started%20guide%5Bwip%5D.pdf

It could be best to start with the [Development Kit USB](https://www.dialog-semiconductor.com/products/da14683-development-kit-usb) if you are totally new and don't want to start with a custom pcb. You do need a breakout for the LCD and connect it to the dev kit.

You can use Smart Snipets Toolbox (after running the code once, it creates the partitions on flash) to load the images (picturefiles.txt) to the flash partition (NVMS_FLASH_STORAGE). These where created with Samson's biptmap loader tool found in the original git.
