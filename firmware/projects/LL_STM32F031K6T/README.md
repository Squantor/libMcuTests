# $(MICROCONTROLLER) tests project
$(MICROCONTROLLER) test project that tests the following peripherals: 
# Hardware required
To run this project you need the following hardware
* $(MICROCONTROLLER) nuclone board
* libMcuLL test board nuclone expansion
* Black magic probe
# compiling and debugging
In the root makefile run the following command:
```
make PROJECT=project
```
To debug with the black magic probe on the commandline via GDB run:
```
make PROJECT=project gdbbmp
```