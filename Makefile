COMPILER = g++
FLAGS = -Wall -O3 -DNDEBUG -std=c++14



data/results.dat: main
	./$< >> $@

data/times_results.dat : main_timer
	./$< >> $@
	
main: main.cpp helper.hpp strategy.hpp mystrategy.hpp world.hpp
	$(COMPILER) $(FLAGS) ./$< -o $@
	
main_timer: my_main.cpp helper.hpp strategy.hpp mystrategy.hpp world.hpp
	$(COMPILER) $(FLAGS) ./$< -o $@
	
test: test.cpp helper.hpp strategy.hpp mystrategy.hpp world.hpp
	$(COMPILER) $(FLAGS) test.cpp -o $@
	./$@
	

.PHONY: clean
clean:
	rm -f data/results.dat data/times_results.dat main test main_timer
