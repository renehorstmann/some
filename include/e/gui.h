#ifndef E_GUI_H
#define E_GUI_H

//
// debug gui, nuklear based
// module based!
//


struct nk_context;
struct eWindow;

void e_gui_init(const struct eWindow *window);

void e_gui_kill();

void e_gui_render();

struct nk_context *e_gui_get_nk_context();

// creates a nuklear window to set the float attribute with a slider
void e_gui_wnd_float_attribute(const char *title, float *attribute, float min, float max, float step);

// nuklear test window
void e_gui_test();


#endif //E_GUI_H
