#include <gtkmm-4.0/gtkmm.h>

#include "include/mainWindow.h"

int main(const int argc, char *argv[]) {
    const auto app = Gtk::Application::create("org.zyazya-pw.graphertoolkit");

    return app -> make_window_and_run<MainWindow>(argc, argv);
}