
//
// from "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
// modified
//

#ifndef WINDOW_GUARD
#define WINDOW_GUARD

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "Point.h"

#include<string>
#include<vector>


namespace Graph_lib
{
    class Shape;   // "forward declare" Shape
    class Widget;

//------------------------------------------------------------------------------

    class Window : public Fl_Window { 
    public:
        // let the system pick the location:
        Window(int w, int h, const std::string& title);
        // top left corner in xy
        Window(Point xy, int w, int h, const std::string& title);    
        // let the system pick the location with icon:
        Window(Point xy, int ww, int hh, const std::string& title, char* iconSrc);

        virtual ~Window() { }

        int x_max() const { return w; }
        int y_max() const { return h; }

        void resize(int ww, int hh) { w=ww, h=hh; size(ww,hh); }

        void set_label(const std::string& s) { copy_label(s.c_str()); }
        void set_icon(char* iconSource, int source){ icon(iconSource); }

        void attach(Shape& s) { shapes.push_back(&s); }
        void attach(Widget&);

        void detach(Shape& s);     // remove s from shapes 
        void detach(Widget& w);    // remove w from window (deactivates callbacks)

        void put_on_top(Shape& p); // put p on top of other shapes

    protected:
        void draw();

    private:
        std::vector<Shape*> shapes;     // shapes attached to window
        int w,h;                   // window size

        void init();
    };

//------------------------------------------------------------------------------

           int gui_main(); // invoke GUI library's main event loop
    inline int x_max() { return Fl::w(); } // width of screen in pixels
    inline int y_max() { return Fl::h(); } // height of screen in pixels

} // of namespace Graph_lib

#endif // WINDOW_GUARD
