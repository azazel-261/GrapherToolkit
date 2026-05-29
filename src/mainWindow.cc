#include <gtkmm-4.0/gtkmm.h>

#include <map>

#include "include/globalContext.h"
#include "include/mainWindow.h"

#include "dynamicMath/include/dynamicMath.h"

GraphView::GraphView(GlobalContext *ctx) : scale(10.0), cameraX(0.0), cameraY(0.0)
{
    auto dragController = Gtk::GestureDrag::create();

    dragController -> signal_drag_begin().connect(sigc::mem_fun(*this, &GraphView::onDragBegin));
    dragController -> signal_drag_update().connect(sigc::mem_fun(*this, &GraphView::onDragUpdate));

    context = ctx;

    set_draw_func(sigc::mem_fun(*this, &GraphView::onDraw));

    add_controller(dragController);
}

void GraphView::onDraw(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) const
{
    cr -> set_source_rgb(.8, .8, .8);
    cr -> paint();

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
