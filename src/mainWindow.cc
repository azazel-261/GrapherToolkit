#include <gtkmm-4.0/gtkmm.h>
#include <gtkmm/eventcontrollerscroll.h>

#include <map>

#include "include/globalContext.h"
#include "include/mainWindow.h"
#include "debugging/include/debugging.h"
#include "dynamicMath/include/dynamicMath.h"


GraphView::GraphView(GlobalContext *ctx) : scale(30.0), cameraX(0.0), cameraY(0.0)
{
    const auto dragController = Gtk::GestureDrag::create();
    const auto scrollController = Gtk::EventControllerScroll::create();

    scrollController -> set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
    scrollController -> signal_scroll().connect(sigc::mem_fun(*this, &GraphView::onScroll), false);

    dragController -> signal_drag_begin().connect(sigc::mem_fun(*this, &GraphView::onDragBegin));
    dragController -> signal_drag_update().connect(sigc::mem_fun(*this, &GraphView::onDragUpdate));

    context = ctx;

    set_draw_func(sigc::mem_fun(*this, &GraphView::onDraw));

    add_controller(dragController);
    add_controller(scrollController);
}

void GraphView::onDraw(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) const
{
    cr -> set_source_rgb(.8, .8, .8);
    cr -> paint();

    if (-cameraY * scale >= 0 && -cameraY * scale <= height)
    {
        cr -> set_line_width(1);
        cr -> set_source_rgb(.2, .2, .2);
        cr -> move_to(0, -cameraY * scale);
        cr -> line_to(width, -cameraY * scale);
        cr -> stroke();
        cr -> begin_new_path();
    }

    if (-cameraX * scale >= 0 && -cameraX * scale <= width)
    {
        cr -> set_line_width(1);
        cr -> set_source_rgb(.2, .2, .2);
        cr -> move_to(-cameraX * scale, 0);
        cr -> line_to(-cameraX * scale, height);
        cr -> stroke();
        cr -> begin_new_path();
    }

    if (context -> expr == nullptr) return;

    if (context -> graph != nullptr) delete context -> graph;

    context -> graph = DynMath::Util::prepareGraph(context -> expr, cameraX, cameraX + height / scale, 1/scale);

    std::map<double, double> graph = *(context -> graph);

    cr -> set_line_width(4);

    cr -> set_source_rgb(.5, 0, .7);

    for (const auto &[x, y] : graph)
    {
        cr -> line_to(x * scale - cameraX * scale, -y * scale - cameraY * scale);
    }

    cr -> stroke();
}

bool GraphView::onScroll(double dx, const double dy)
{
    scale -= dy * 2;
    scale = std::clamp(scale, 4.0, 300.0);
    logInfo(std::format("Scale: {}", scale));
    queue_draw();
    return true;
}

void GraphView::onDragBegin(const double startX, const double startY)
{
    dragBeginX = cameraX;
    dragBeginY = cameraY;
}

void GraphView::onDragUpdate(const double offsetX, const double offsetY)
{
    cameraX = dragBeginX - offsetX / scale;
    cameraY = dragBeginY - offsetY / scale;
    queue_draw();
}

MainWindow::MainWindow(GlobalContext *ctx) : view(ctx)
{
    context = ctx;

    const auto masterLayout = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);

    view.set_expand(true);
    view.set_margin_bottom(10);
    masterLayout->append(view);

    expressionBox.set_hexpand(true);
    masterLayout->append(expressionBox);

    masterLayout->set_margin(10);

    set_child(*masterLayout);

    set_title("Grapher Toolkit v0.1");
    set_size_request(600, 800);
}
