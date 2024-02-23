# build all the templates and clean afterwards
#!/bin/bash
make -j24 PROJECT=PC CONFIG=debug
make -j24 PROJECT=LPC844M201BD64_C_tests CONFIG=debug
make -j24 PROJECT=LPC844M201BD64_CPP_tests CONFIG=debug
make -j24 PROJECT=tests_template CONFIG=debug
make -j24 PROJECT=LPC812M101DH20_CPP_tests_first CONFIG=debug
make -j24 PROJECT=LPC812M101DH20_CPP_tests_second CONFIG=debug
make -j24 PROJECT=LPC812M101DH20_CPP_tests_third CONFIG=debug
make -j24 PROJECT=CortexM0plus_tests CONFIG=debug
make -j24 clean