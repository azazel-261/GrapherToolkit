#include <gtkmm-4.0/gtkmm.h>
#include <gtkmm/eventcontrollerscroll.h>

#include <map>
#include <cmath>

#include "include/globalContext.h"
#include "include/mainWindow.h"
#include "debugging/include/debugging.h"
#include "dynamicMath/include/dynamicMath.h"
#include "dynamicMath/include/grapher.h"

constexpr double yValueLimit = 1e4;
constexpr double jumpLimit = 500;

/*
 * Not much here, just a lot of signal connections
 */
GraphView::GraphView(GlobalContext *ctx) : scale(30.0), cameraX(0.0), cameraY(0.0)
{
    dragBeginX = 0.0;
    dragBeginY = 0.0;

    windowWidth = 0.0;
    windowHeight = 0.0;

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

/*
 * Called with each queue_draw to update the graph
 */
void GraphView::onDraw(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height)
{
    windowWidth = width;
    windowHeight = height;

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

    const DynMath::Grapher gr(context -> expr);

    cr -> set_line_width(4);
    cr -> set_source_rgb(.5, 0, .7);

    const auto graph = gr.GraphExpression(cameraX, cameraX + width / scale, 1/scale);

    for (const auto &segment : graph)
    {
        for (const auto &point : segment)
        {
            cr -> line_to(point.x * scale - cameraX * scale, point.y * scale - cameraY * scale);
        }
        cr -> stroke();
        cr -> begin_new_path();
    }
}


/*
 * Everything below is self-explanatory
 */
bool GraphView::onScroll(double dx, const double dy)
{
    scale -= dy * 2;
    scale = std::clamp(scale, 4.0, 300.0);
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

/*
 * Not only is responsible for parsing the expression, but also catches all the exceptions from the parser to be displayed nicely
 */
void MainWindow::onExpressionTextFieldChange()
{
    std::string entryExpr = expressionBox.get_text();
    std::erase(entryExpr, ' ');

    DynMath::Expression *expr = nullptr;

    try
    {
        const std::vector<std::string> tokens = DynMath::Util::tokenize(entryExpr);
        expr = DynMath::Util::parse(tokens);
    } catch (std::invalid_argument &e)
    {
        const auto popover = Gtk::make_managed<Gtk::Popover>();
        const auto label = Gtk::make_managed<Gtk::Label>(e.what());
        popover -> set_child(*label);
        popover -> set_parent(expressionBox);
        popover -> set_has_arrow(true);
        popover -> popup();
    }

    delete context -> expr;
    context -> expr = expr;

    if (expr != nullptr) logInfo(std::format("Expression: {}", expr -> toString()));

    view.queue_draw();
}

/*
 * Good ol' window setup
 */
MainWindow::MainWindow(GlobalContext *ctx) : view(ctx)
{
    context = ctx;

    const auto masterLayout = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);

    view.set_expand(true);
    view.set_margin_bottom(10);
    masterLayout -> append(view);

    expressionBox.set_placeholder_text("f(x)=...");
    expressionBox.set_hexpand(true);
    expressionBox.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::onExpressionTextFieldChange));

    masterLayout -> append(expressionBox);

    masterLayout -> set_margin(10);

    set_child(*masterLayout);

    set_title("Grapher Toolkit v0.1");
    set_size_request(600, 800);
    // set_resizable(false);
}
