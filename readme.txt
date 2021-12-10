a simple cellular automaton 
that extends Conway's Game of Life with floating point values.

To view presets press one of the keys from 1 to 9.

controls:
---------

1                        load file save/1.txt
2                        load file save/2.txt
...                      ...

enter                    generate random pixels everywhere
p                        pause/unpause simulation
	
q,w,e,r,t,y,...,o,p      select parameter "q", "w", ...
                         these parameters influence the pattern of the simulation.
                         it is not important to know their actual meaning to explore the simulation
	
left-arrow               decrease selected parameter by current "weight" amount
right-arrow              increase selected parameter by current "weight" amount (notice the changing pattern)

up-arrow                 increase the "weight" of left-arrow and right-arrow
down-arrow               decrease the "weight" of left-arrow and right-arrow
                         the larger the "weight", the larger the modification of the selected parameter
		

space                    when paused: simulate single step
                         else:        randomize parameters
			
escape                   exit simulation
s                        save current parameters to save/current.txt
