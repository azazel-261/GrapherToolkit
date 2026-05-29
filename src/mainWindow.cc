#include <gtkmm-4.0/gtkmm.h>

#include <map>

#include "include/mainWindow.h"
#include "dynamicMath/include/dynamicMath.h"

GraphView::GraphView(GlobalContext *ctx) : scale(0.1), cameraX(0.0), cameraY(0.0)
{
    context = ctx;
    set_draw_func(sigc::mem_fun(*this, &GraphView::onDraw));
}

void GraphView::onDraw(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) const
{
    std::map<double, double> graph = DynMath::Util::prepareGraph(context -> expr, cameraX, cameraX + width / scale, 1/scale);
}

MainWindow::MainWindow(GlobalContext *ctx) : view(ctx)
{
    context = ctx;

    const auto masterLayout = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);

    view.set_expand(true);
    masterLayout->append(view);

    expressionBox.set_hexpand(true);
    masterLayout->append(expressionBox);

    masterLayout->set_margin(10);

    set_child(*masterLayout);

    set_title("Grapher Toolkit v0.1");
    set_size_request(600, 800);
}
