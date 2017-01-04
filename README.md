# Pedestrian Traffic Simulation

This was my thesis for my engineering degree. It's a simulation of crowds as they move through Town Hall station in Sydney. 
The purpose of this simulation was to see if people would change their behaviour if they could see the density of crowds through a 
mobile phone application, or visible indicators on platforms.

The program runs a simulation and records the number of passengers at each node each second, and outputs to a CSV file for data analysis later. 
The main experiment spawns 5 random people outside the train station per second until 400 passengers have been spawned. 
Each passenger has the goal to reach a position on the platform and changes their speed and direction based on the density of 
people around them. The crowds spread out depending on the settings of the experiment which determine how averse each person is to crowds, 
and to the distance to the next uncrowded node. There are other experiments to run as well, which model trains arriving and departing.

# Install
Download the Qt IDE from [here](https://www.qt.io/download/). You will need a C++ compiler too. I checked this works with msvc2015 in December 2016.
you might like to install a [C++ debugger](http://doc.qt.io/qtcreator/creator-debugger-engines.html) too. Attempt to open one of the example projects in Qt to check that your compiler and kits are working.

When Qt is working, download this repo and open TrafficSimulation.pro

Edit the globals.h file to set options for the simulation, run the project and observe the [results](http://i.imgur.com/44nJpja.gifv).

# Built With
- [Qt](https://www.qt.io/download/) IDE (pronounced "cute")

# Author
James Kirk 

[GitHub](https://github.com/alias8)

# License
Distributed under the MIT license.
