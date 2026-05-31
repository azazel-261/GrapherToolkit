#ifndef GRAPHERTOOLKIT_MAINWINDOW_H
#define GRAPHERTOOLKIT_MAINWINDOW_H

#include <gtkmm-4.0/gtkmm.h>

#include "globalContext.h"

class GraphView : public Gtk::DrawingArea
{
public:
    explicit GraphView(GlobalContext *ctx);
private:
    GlobalContext *context;
    /*
     * pixels per unit on the graph
     */
    double scale;

    double cameraX;
    double cameraY;

    double dragBeginX{};
    double dragBeginY{};

    void onDragBegin(double startX, double startY);
    void onDragUpdate(double offsetX, double offsetY);

    bool onScroll(double dx, double dy);

    void onDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) const;
};

class MainWindow : public Gtk::Window
{
public:
    explicit MainWindow(GlobalContext *ctx);
private:
    void onExpressionTextFieldChange();

    GlobalContext *context;
    Gtk::Entry expressionBox;
    GraphView view;
};

#endif
