#include <gtkmm-4.0/gtkmm.h>
#include <iostream>

#include "debugging/include/debugging.h"
#include "include/mainWindow.h"
#include "dynamicMath/include/dynamicMath.h"
#include "include/globalContext.h"

int main(const int argc, char *argv[]) {
    const auto app = Gtk::Application::create("org.zyazya-pw.graphertoolkit");

    GlobalContext ctx = {nullptr, nullptr};

    return app -> make_window_and_run<MainWindow>(argc, argv, &ctx);
}