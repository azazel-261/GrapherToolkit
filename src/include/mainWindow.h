#ifndef GRAPHERTOOLKIT_MAINWINDOW_H
#define GRAPHERTOOLKIT_MAINWINDOW_H

#include <gtkmm-4.0/gtkmm.h>

#include "include/globalContext.h"

class GraphView : public Gtk::DrawingArea
{
public:
    explicit GraphView(GlobalContext *ctx);
private:
    GlobalContext *context;
    /*
     * steps per pixel
     */
    double scale;
    double cameraX;
    double cameraY;
    void onDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) const;
};

class MainWindow : public Gtk::Window
{
public:
    explicit MainWindow(GlobalContext *ctx);
private:
    GlobalContext *context;
    Gtk::Entry expressionBox;
    GraphView view;
};

#endif
