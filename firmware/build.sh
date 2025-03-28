# build all the templates and clean afterwards
#!/bin/bash
make -j24 PROJECT=PC CONFIG=debug
make -j24 PROJECT=unittests CONFIG=debug
make -j24 PROJECT=test_LL_CortexM0plus CONFIG=debug
make -j24 PROJECT=test_LL_LPC812DH20_1 CONFIG=debug
make -j24 PROJECT=test_LL_LPC812DH20_2 CONFIG=debug
make -j24 PROJECT=test_LL_LPC812DH20_3 CONFIG=debug
make -j24 PROJECT=test_LL_LPC845BD48 CONFIG=debug
make -j24 PROJECT=test_LL_RP2040 CONFIG=debug
make -j24 PROJECT=test_LL_STM32F031K6T CONFIG=debug
make -j24 PROJECT=test_HAL_RP2040 CONFIG=debug
make -j24 PROJECT=test_HAL_LPC845BD48 CONFIG=debug
make -j24 PROJECT=driver_mux3to8_74hc595 CONFIG=debug

