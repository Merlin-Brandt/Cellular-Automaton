#ifndef CELL_RENDER_H_INCLUDED
#define CELL_RENDER_H_INCLUDED

// initialize renderer
void cells_render_init(int renderX, int renderY, int renderW, int renderH);

// free resources
void cells_render_deinit();

// updates the main texture which visualizes the cell grid
void cells_render_update();

// render the main texture to the window
void cells_render();

#endif // CELL_RENDER_H_INCLUDED
