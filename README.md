# Driver for the RGB LED

Driver lib for RGB LED written in C programming language for ESP-IDF projects. This repository contains the driver files (.h and .c) to be included, or linked directly as a git submodule, in the sensor's projects.


The driver was designed to facilitate calls for a single rgbled using a led_strip lib from offical espressif for led status diagnostic propouses embebed on the sensor hardware.


It implements functions to simple call changes to the rgbled instances in order to change: color and mode[static,blink,pulse]


In order to `clone` the complete content of the repository folder, use the command:

```
git clone https://github.com/iasxtecnologia/rgbled-diag/
```

OR add as git submodule in the sensor project:

```
git submodule add https://github.com/iasxtecnologia/rgbled-diag/ .

```










**Copyright (C) 2023 IASXTecnologia**






