# libMcu tests repository
This repository contains examples and tests for the [libMcu](https://github.com/Squantor/libMcu.git) library. 
This is code meant for usage in a CI/CD pipeline with a HIL (Hardware in the loop) setup I am working on. The tests are subdivided into three main parts:
* libMcuLL low level library tests with HIL setup
* libMcuHal tests, usually can run on PC
* libMcuDrivers tests, these are usually done in a HIL setup with a few boards that get rotated every while.
# Cloning
use ```git clone --recurse-submodules https://github.com/Squantor/$(PROJECT).git``` to clone the repository and its submodules. Be aware I use ssh style clones, anonymous clones might not work.
# Usage
See [firmware](firmware/README.md)
