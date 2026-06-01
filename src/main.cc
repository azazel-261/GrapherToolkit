#include <gtkmm-4.0/gtkmm.h>
#include <locale>
#include <iostream>

#include "include/mainWindow.h"
#include "dynamicMath/include/dynamicMath.h"
#include "include/globalContext.h"

int main(const int argc, char *argv[]) {
    std::locale::global(std::locale::classic());

    const auto app = Gtk::Application::create("org.zyazya-pw.graphertoolkit");

    GlobalContext ctx = {nullptr};

    return app -> make_window_and_run<MainWindow>(argc, argv, &ctx);
}