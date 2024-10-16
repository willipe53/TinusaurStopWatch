# TinusaurStopWatch
This is an SPI stopwatch using ATTiny85, MAX7219, and 7-segment LEDs that I created to get some familiarity with the [Tinusaur](https://tinusaur.org/) platform.  Code is straightforward:

* [LedControl](https://wayoda.github.io/LedControl/) by Eberhard Fahle to manage the MAX7219
* [debounce](https://github.com/kimballa/button-debounce) by Aaron Kimball to handle button presses
* A `runState` variable to store whether the stopwatch is currently `CLEARED`, `STOPPED`, or `RUNNING`
* Use of Tinusaur SPI on GPIO 0, 1, and 2, and a button on GPIO 4
* Use of `millis()` as a simple timer to update the display every 100 ms.

Most of my challenges getting this to work had to do with getting Arduino IDE on my Apple Silicon Mac to interface with the ATTiny85.  Things I did to make this sketch work on the hardware were, in no particular order:

1. Added a 0.1 uF ceramic capacitor across MAX7219 `V+` and `GND`
2. Added a 470 ohm resistor across MAX7219 `V+` and `ISET` to dim the LEDs a bit and save power
3. Added reference to David Mellis ATTiny libary to the additional board manager URLs in Arduino IDE settings: `https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json`
4. Did `brew install avrdude` so I had a programmer that could talk to the USB interface of the Tinusaur
5. Even though I should be able to use `Sketch->Upload Using Programmer` in the Arduino IDE, I was only able to successfully upload the sketch to the Tinusaur by using the `Sketch->Export Compiled Binary` command and then running avrdude from the command line:

```zsh
avrdude -v -c usbasp -p attiny85 -B 125kHz -U flash:w:/Users/willipe/github/TinusaurStopWatch/build/attiny.avr.ATtinyX5/TinusaurStopWatch.ino.hex:i
```

