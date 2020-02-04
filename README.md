# OpenWatch
Open source watch based on [https://github.com/S-March/smarchWatch_PUBLIC](smarchWatch), da14683 ic. It uses some files Samson wrote, but most are heavily changed or created by me. I did not want to fork it, since it's a different watch (on the same pcb but with touch connector). Also started this repo just for myself but decided to share it.

It can connect to iOS devices and receive notifications. At the moment the project is still WIP.

![Picture of the watch and os](https://www.digitalfactor.nl/watch/watch2.jpg)
Small video of the watch in action [found here](https://streamable.com/83sec)

It uses [this touchscreen](https://www.topfoison.com/Products-TF13016A-C.html). I ordered 10 samples from Topfoison on alibaba, but it's hard and expensive to get. Also the backlight is wired in series, but the pcb can only supply 3.2v parallel). I chose this display because it has a better layout for the connectors.
It does work with [this one](https://www.dhgate.com/product/1-3-inch-240-240-round-ips-tft-lcd-module/411021741.html#seo=WAP) but that won't fit the housing and can't fold nicely flat (in my batch of lcd's). If a better version will be found I'll update the pcb and housing.

There is no finished tutorial yet but you can find a getting started guide here:
[Getting started guide](https://github.com/dannygrob/OpenWatch/blob/master/OpenWatch%20getting%20started%20guide%5Bwip%5D.pdf)

It could be best to start with the [Development Kit USB](https://www.dialog-semiconductor.com/products/da14683-development-kit-usb) if you are totally new and don't want to start with a custom pcb. You do need a breakout for the LCD and connect it to the dev kit.