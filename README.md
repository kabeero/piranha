# piranha_gui

## PIRANHA (Parallel Imager Array for Real-Time Analysis in a NeighborHood Array)

This project contains the C++ source code for the Qt4 GUI used for the parallel-processing camera used in my research.

The dependencies needed (off the top of my head):
- g++
- qt4 (core, gui)
- boost
- ftdi
- opencv

## Running the GUI

You may need to run cmake beforehand, if the Makefile included widely varies from your Qt install.

`make && ./piranha'

## Using the GUI

Of course, you'll need the camera to view and record video.

But there is a sample noise generator option to see if your GUI is working and how many FPS you get.

Press ctrl+M to access the programming utility, to flash new FPGA code and select which camera you want based on serial number.
