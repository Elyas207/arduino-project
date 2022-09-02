# A FM radio that also functions as a Bluetooth speaker. [School Project]
Parts List:
> [2x 5W Speakers](https://www.amazon.com.au/CQRobot-Speaker-Interface-Electronic-Projects/dp/B0822YL2L2?th=1)

> [1x TEA5767 FM Radio Module (I2C)](https://www.amazon.com.au/Ximimark-TEA5767-Arduino-76-108MHZ-Frequency/dp/B07L74WYRX)

> [2x LM386 Audio Amplifier (1x for each of the two speakers)](https://www.amazon.com.au/SOOHAB-Amplifier-Module-Adjustable-Resistance/dp/B0B86HT6SB/ref=sr_1_10?crid=2G9Q7YA84A612&keywords=lm386&qid=1662102124&s=electronics&sprefix=lm38%2Celectronics%2C228&sr=1-10)

> [1x BT401 V3 Audio & BLE/SPP Pass-through Module - Bluetooth 5.0](https://www.digikey.com.au/en/products/detail/dfrobot/DFR0782/13688341)

> [1x 16x2 LCD Character Display](https://www.amazon.com/SunFounder-Serial-Module-Display-Arduino/dp/B019K5X53O/ref=sr_1_7?keywords=i2c+1602+lcd&qid=1662102357&sr=8-7)

> [1x Male to Male TRS Audio Cable](https://www.amazon.com.au/Astrotek-Stereo-3-5mm-Flat-Cable/dp/B07X6RTK3R/ref=sr_1_4?c=ts&keywords=Stereo+Jack+Cables&qid=1662102546&refinements=p_36%3A-400&rnid=5355409051&s=electronics&sr=1-4&ts_id=4885453051)

> [10K Ohm Potentiometer (For controlling selected radio frequency)](https://www.amazon.com.au/Potentiometer-Panel-Mount-Breadboard-Friendly/dp/B07XXWWXMC/ref=sr_1_5?crid=15CC35EPVZNPS&keywords=10k+potentiometer&qid=1662102607&s=electronics&sprefix=10k+petentiom%2Celectronics%2C215&sr=1-5)

> [Ardunio UNO (R3)](https://www.amazon.com.au/Gikfun-Enclosure-Transparent-Computer-Compatible/dp/B00UBT87XM/ref=sr_1_21?crid=21VJV8FZAYMHD&keywords=Arduino+uno&qid=1662103392&s=electronics&sprefix=arduino+un%2Celectronics%2C242&sr=1-21)

> [General Arduino Jumper Wires](https://www.amazon.com.au/120pcs-Multicoloured-Dupont-Breadboard-arduino/dp/B01EV70C78/ref=sr_1_4?crid=KCC4FPIN3SEB&keywords=arduino+jumper+cables&qid=1662103471&s=electronics&sprefix=arduno+jumper+cable%2Celectronics%2C197&sr=1-4)

##

## Wiring Diagram
![Wiring Diagram Image](https://github.com/Elyas207/arduino-project/blob/master/Wiring_Layout_bb.png?raw=true)

> Note: The wiring diagram currently only includes connections for the TEA5767 Radio Module and no connections for the BT401 module.



## Documentation for the TEA5767 and BT401 modules (for your own code exploration).
>[BT401 by DFROBOT](https://wiki.dfrobot.com/Audio_BLE_SPP_Pass_Through_Module_Bluetooth_5.0_SKU_DFR0781)

>[TEA5767 Arduino Library](https://mathertel.github.io/Radio/html/class_t_e_a5767.html)

## Possible things to change/add
- [ ] Be able to use both speakers using an Audio Mixer
- [ ] Use available TF Card and U-Disk port provided with the BT401 Evaluation Board





# Code
https://github.com/Elyas207/arduino-project/blob/master/MainCode.ino
