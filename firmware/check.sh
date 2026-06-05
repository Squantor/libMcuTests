# build all the templates and clean afterwards
#!/bin/bash
make -j24 PROJECT=PC check
make -j24 PROJECT=unittests check
make -j24 PROJECT=tests_template check
make -j24 PROJECT=test_LL_CortexM0plus check
make -j24 PROJECT=test_LL_LPC812DH20_1 check
make -j24 PROJECT=test_LL_LPC812DH20_2 check
make -j24 PROJECT=test_LL_LPC812DH20_3 check
make -j24 PROJECT=test_LL_LPC845BD48 check
make -j24 PROJECT=test_LL_RP2040 check
make -j24 PROJECT=test_HAL_RP2040 check
make -j24 PROJECT=test_HAL_LPC845BD48 check
make -j24 PROJECT=driver_mux3to8_74hc595 check
make -j24 PROJECT=test_LL_STM32F031K6T check
