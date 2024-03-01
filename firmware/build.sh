# build all the templates and clean afterwards
#!/bin/bash
make -j24 PROJECT=PC CONFIG=debug
make -j24 PROJECT=tests_template CONFIG=debug
make -j24 PROJECT=test_LL_LPC812DH20_1 CONFIG=debug
make -j24 PROJECT=test_LL_LPC812DH20_2 CONFIG=debug
make -j24 PROJECT=test_LL_LPC812DH20_3 CONFIG=debug
make -j24 PROJECT=test_LL_LPC844BD64 CONFIG=debug
make -j24 clean