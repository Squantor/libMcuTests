# lib Mcu LowLevel examples and development
Examples and experiments live here
# test projects
Here is a list of the various projects.
* [libMcuLL generic CortexM0+ tests for C++ programming](projects/tests_CPP_CortexM0plus//README.md)
* [libMcuLL LPC812M101DH20 first tests for C++ programming](projects/tests_CPP_LPC812M101DH20_first/README.md) 
* [libMcuLL LPC812M101DH20 second tests for C++ programming](projects/tests_CPP_LPC812M101DH20_second/README.md) 
* [libMcuLL LPC812M101DH20 third tests for C++ programming](projects/tests_CPP_LPC812M101DH20_third/README.md) 
* [libMcuLL LPC844M201BD64 tests for C++ programming](projects/tests_CPP_LPC844M201BD64/README.md) 
# Building
To get a list of available projects run ```make projects``` to get a list of available projects. Building a project is done with the ```PROJECT``` variable that needs to be passed on, for example ```make PROJECT=example```.
There are various build configurations available that can configured with the ```CONFIG``` variable, available options are debug, debug_size, release_size and release_speed. The default build configuration is debug.
For cleaning run ```make clean```
# Hardware dependencies
Per test project you need hardware dependencies, see per project what is needed. Most of the hardware is based on the [nuclone series of development boards](https://github.com/Squantor/squantorDevelBoards/tree/master) and a [libMcuLL test board](https://github.com/Squantor/squantorDevelBoards/tree/master/electronics/nuclone_small_libmcull_tester)
# Debugging embedded projects
For debugging on embedded targets, the [Black Magic Probe](https://github.com/blacksphere/blackmagic/wiki) is used, scripts to use this debugger are present in the ```gdb_scripts``` directory. Make can be used to run debugging:
```
make PROJECT=project CONFIG=config gdbusbdebug
make PROJECT=project CONFIG=config gdbusbrelease
```
This will build, if needed, the debug or release build respectivly and load it into the target.
```
make tpwrdisable
make tpwrenable
```
The black magic probe is capable of powering the target, these commands enable/disable this function.
# Depedencies
The templates have a few dependencies that are automatically cloned when you do a clone with ```--recurses-submodules
* [squantorLibC](https://github.com/Squantor/squantorLibC) for minimal LibC functionality
* [squantorLibEmbedded](https://github.com/Squantor/squantorLibEmbedded) for various embedded helper functions
* [squantorMinUnit](https://github.com/Squantor/squantorMinUnit) for minimal unittesting functionality on target
* [libMcu](https://github.com/Squantor/libMcu) for testing this codebase
# Usage
To use the software you need the following hardware:
* [Black magic probe](https://github.com/blacksphere/blackmagic)
* [Various nuclone development boards](https://github.com/Squantor/squantorDevelBoards/tree/master)
