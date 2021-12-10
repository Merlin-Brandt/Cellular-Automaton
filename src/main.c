
//readme a simple cellular automaton 
//readme that extends Conway's Game of Life with floating point values.
//readme 
//readme in order to compile this project you need to have GLFW3 and GLEW installed.
//readme 
//readme To view presets press one of the keys from 1 to 9.
//readme 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "cells.h"
#include "cells_render.h"
#include "sleep.h"


// initial window width and height
const int winW = 800, winH = 600;

// when set to 0, the simulation is paused and in step-mode.
// otherwise the simulation is running. 
static int main_pause = 0;

// when set to 0, a mouse button is not being held down.
// the variable button_hold_button indicates which button is being held down.
static int button_hold = 0;
static int button_hold_button;

// table of all floating point parameters which are saved with cells_logic_save()
#define param_table_size 5
float *param_table[] = {
	&cell_bestAvg, 
	&cell_best_range, 
	&cellDieSpeed, 
	&cell_minLife, 
	&cellLifeBoost,
};

// currently user-selected parameter
static int selected_param = 0;
// currently user-defined parameter modification amount for each parameter in the parameter table
float param_weights[param_table_size] = {
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

// seed for random number generator
unsigned masterSeed;

// handler function for GLFW error event
static void handleGlfwErr(int code, const char *msg)
{
    fprintf(stderr, "GLFW error: %s\n", msg);
    exit(EXIT_FAILURE);
}

// handler function for GLFW mouse move event
// also called from handleGlfwMouse()
static void handleGlfwMove(GLFWwindow *window, double mx, double my)
{
	if (button_hold)
	{
		int mouse_move_x = (1 - (my / winH)) * cellGridH;
		int mouse_move_y = mx / winW * cellGridW;
		cellLife[cell_index(mouse_move_x, mouse_move_y)] =
		    button_hold_button == GLFW_MOUSE_BUTTON_LEFT;
	}
}

// handler function for GLFW mouse button event
static void handleGlfwMouse(GLFWwindow *window, int button, int action, int mods)
{	
	double mx, my;
	button_hold = action == GLFW_PRESS;
	button_hold_button = button;
	glfwGetCursorPos(window, &mx, &my);
	handleGlfwMove(window, mx, my);
}

// handler function for GLFW key event
static void handleGlfwKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	//readme controls:
	//readme ---------
	//readme 
	switch (key)
	{

	//readme 1                        load file save/1.txt
	//readme 2                        load file save/2.txt
	//readme ...                      ...
	//readme 
	case GLFW_KEY_0:
		if (action == GLFW_PRESS)
			cells_logic_load("save/0.txt");
		break;
	case GLFW_KEY_1:
		if (action == GLFW_PRESS)
			cells_logic_load("save/1.txt");
		break;
	case GLFW_KEY_2:
		if (action == GLFW_PRESS)
			cells_logic_load("save/2.txt");
		break;
	case GLFW_KEY_3:
		if (action == GLFW_PRESS)
			cells_logic_load("save/3.txt");
		break;
	case GLFW_KEY_4:
		if (action == GLFW_PRESS)
			cells_logic_load("save/4.txt");
		break;
	case GLFW_KEY_5:
		if (action == GLFW_PRESS)
			cells_logic_load("save/5.txt");
		break;
	case GLFW_KEY_6:
		if (action == GLFW_PRESS)
			cells_logic_load("save/6.txt");
		break;
	case GLFW_KEY_7:
		if (action == GLFW_PRESS)
			cells_logic_load("save/7.txt");
		break;
	case GLFW_KEY_8:
		if (action == GLFW_PRESS)
			cells_logic_load("save/8.txt");
		break;
	case GLFW_KEY_9:
		if (action == GLFW_PRESS)
			cells_logic_load("save/9.txt");
		break;

	//readme enter                    generate random pixels everywhere
	case GLFW_KEY_ENTER:
		if (action == GLFW_PRESS)
		{
			cells_clean();
			cells_gen(rand(), !(mods & GLFW_MOD_CONTROL));
		}
		break;

	//readme p                        pause/unpause simulation
	//readme 	
	case GLFW_KEY_P:
		if (action == GLFW_PRESS)
			main_pause = !main_pause;
		break;
		
	//readme q,w,e,r,t,y,...,o,p      select parameter "q", "w", ...
	//readme                          these parameters influence the pattern of the simulation.
	//readme                          it is not important to know their actual meaning to explore the simulation
	//readme 	
	case GLFW_KEY_Q:
		selected_param = 0;
		break;
	case GLFW_KEY_W:
		selected_param = 1;
		break;
	case GLFW_KEY_E:
		selected_param = 2;
		break;
	case GLFW_KEY_R:
		selected_param = 3;
		break;
	case GLFW_KEY_T:
		selected_param = 4;
		break;
	case GLFW_KEY_Y:
		cell_diagNeighbors = !cell_diagNeighbors;
		break;

	//readme left-arrow               decrease selected parameter by current "weight" amount
	//readme right-arrow              increase selected parameter by current "weight" amount (notice the changing pattern)
	//readme 
	case GLFW_KEY_LEFT:
		*param_table[selected_param] -= param_weights[selected_param];
		break;
	case GLFW_KEY_RIGHT:
		*param_table[selected_param] += param_weights[selected_param];
		break;

	//readme up-arrow                 increase the "weight" of left-arrow and right-arrow
	//readme down-arrow               decrease the "weight" of left-arrow and right-arrow
	//readme                          the larger the "weight", the larger the modification of the selected parameter
	//readme 		
	//readme 
	case GLFW_KEY_DOWN:
		param_weights[selected_param] *= 0.5f;
		break;
	case GLFW_KEY_UP: 
		param_weights[selected_param] *= 2.0f;
		break;

	//readme space                    when paused: simulate single step
	//readme                          else:        randomize parameters
	//readme 			
	case GLFW_KEY_SPACE:
		if (main_pause)
		{
			if (action != GLFW_RELEASE)
				cells_logic();
		}
		else if (action == GLFW_PRESS)
		{
			cells_clean();
			cells_logic_rand(rand());
			cells_gen(rand(), 0);
		}
		break;
			
	//readme escape                   exit simulation
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);
		break;

	//readme s                        save current parameters to save/current.txt
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			cells_logic_save("save/current.txt");
		break;
	}
}

// handler function for GLFW framebuffer event (called when window size changes)
void handleGlfwFramebuffer(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
    GLFWwindow *window;
	
    masterSeed = time(0);
    srand(masterSeed);

    glfwSetErrorCallback(handleGlfwErr);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    atexit(glfwTerminate);

    window = glfwCreateWindow(winW, winH, "Cells", 0, 0);
    if (!window)
        exit(EXIT_FAILURE);
	glfwSetKeyCallback(window, handleGlfwKey);
	glfwSetMouseButtonCallback(window, handleGlfwMouse);
	glfwSetCursorPosCallback(window, handleGlfwMove);
	glfwSetFramebufferSizeCallback(window, handleGlfwFramebuffer);

    glfwMakeContextCurrent(window);

    cells_alloc(128, 128);
    cells_clean();
    cells_gen(rand(), 0);
    cells_logic_rand(rand());
    cells_render_init(0, 0, winW, winH);

    while(!glfwWindowShouldClose(window))
    {
		if (!main_pause)
			cells_logic();
		cells_render_update();
		cells_render();
		glfwSwapBuffers(window);
        glfwPollEvents();
		sleep(1000./30.);
    }

    cells_render_deinit();
    cells_dealloc();

    glfwDestroyWindow(window);
    exit(EXIT_SUCCESS);
}

