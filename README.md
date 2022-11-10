# Interface library for comma.ai's USB panda.

A library that handles communication with a comma.ai Panda interface device.  Simple demos collect data from the panda (grey version) to the screen.

A tutorial for hardware setup is located here:
[https://jmscslgroup.github.io/libpanda/](https://jmscslgroup.github.io/libpanda/)

Full Raspbian-based images with ROS/libpanda preinstalled can be found here:
[https://jmscslgroup.github.io/libpanda/doc/image.md](doc/image.md)

## Dependencies:
1. Required - libusb-1.0-0-dev
2. Optional - libcurses5-dev
3. Included - NMEAParser from https://github.com/visualgps/NMEAParser/

### libusb-1.0-0-dev
The core of this library is based on libusb, the default version installable through Ubuntu's apt-get.

### libcurses5-dev
An optional example for data visualization is based on ncurses.  If desired to run this example, libcurses needs to be installed but is not required

### NMEAParser
The NMEAParser is already included as a modified version with some bug fixes and added features.
1. Bugs fixed:
* Incomplete time precision for seconds in RMC strings
* Year calculation incorrect byte offset resulting in incorrect year parsings
2. Additional features:
* ZDA NMEA message parsing

## Setup:
These instructions were successful on a Raspberry pi 4 running Ubuntu 18.04.
### Install libusb-1.0-0-dev
`$ sudo apt-get install libusb-1.0-0-dev`

### Optional: install libsurses5-dev
`$ sudo apt-get install libncurses5-dev`

### Compile
1. `$ mkdir build`
2. `$ cd build`
3. `$ cmake ..`
4. `$ make`

## Examples
### simplecan
Minimal example showing a minimal implementation of classes Panda::Usb and Panda::Can (no GPS).

`$ sudo ./simplecan`

### simplegps
Minimal example showing a minimal implementation of classes Panda::Usb and Panda::Gps (no CAN).

`$ sudo ./simplegps`

### panda
Barebones example showing a minimal implementation of class Panda::Handler, utilizing Panda::Usb, Panda::Can, and Panda::Gps.  Command line arguments can be specified to record log files.

Simple Execution:

`$ sudo ./panda`

Saving CSV CAN data to file:

`$ sudo ./panda -c csvData.txt`

Saving GPS NMEA data to file, and also run USB in synchronous mode:

`$ sudo ./panda -u s -g nmeaStrings.txt`

Saving GPS NMEA, CSV CAN, and raw CAN data simultaneously:

`$ sudo ./panda -c csvData.txt -g nmeaStrings.txt -r rawCANdata.txt`

### pandaSetTime
This example listens to the GPS and when data is valid, synchronizes the system clock to the GPS's UTC if the difference is larger than a defined delta.

`$ sudo ./pandaSetTime`

### pandaCurses
An example that plots GPS and CAN data to the console in a pretty fashion.  This also takes arguments for various data collection and hardware modes.

`$ sudo ./pandaCurses`

Save NMEA strings to a file named nmeaFile.txt:

`$ sudo ./pandaCurses -g nmeaFile.txt`

Run with USB in synchronous mode (default is asynchronous):

`$ sudo ./pandaCurses -u s`

Run with faked CAN data (i.e. when not connected to a car...):

`$ sudo ./pandaCurses -f`

## Documentation:
The library is self-documenting using Doxygen, producing both HTML and LaTex.  Generate the documentation as follows:

`$ doxygen doxygen.conf`

If you do not have doxyen, you may install using, on Ubuntu:

`$ sudo apt-get install doxygen`

Or on macOS with macports:

`$ sudo port install doxygen`

## Todo
- [x] Panda GPS interface
- [ ] Isochronous USB transfer
- [ ] Timestamped libusb error logging
- [x] CAN frame parsing
- [x] CAN frame handling
- [ ] ROS based publishing
- [ ] Testing

# LICENSE

> Author: Matt Bunting
> Copyright (c) 2021 Arizona Board of Regents
> All rights reserved.
> Permission is hereby granted, without written agreement and without
> license or royalty fees, to use, copy, modify, and distribute this
> software and its documentation for any purpose, provided that the
> above copyright notice and the following two paragraphs appear in
> all copies of this software.

> IN NO EVENT SHALL THE ARIZONA BOARD OF REGENTS BE LIABLE TO ANY PARTY
> FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
> ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
> IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
> SUCH DAMAGE.
> THE ARIZONA BOARD OF REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES,
> INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
> AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER
> IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION
> TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
