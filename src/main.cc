#include <gtkmm-4.0/gtkmm.h>

#include "include/mainWindow.h"
#include "dynamicMath/include/dynamicMath.h"
#include "include/globalContext.h"

int main(const int argc, char *argv[]) {
    const auto app = Gtk::Application::create("org.zyazya-pw.graphertoolkit");

    std::vector<std::string> tokens = DynMath::Util::tokenize("x^2");

    DynMath::Expression *expr = DynMath::Util::parse(tokens);

    GlobalContext ctx = {expr};

    return app -> make_window_and_run<MainWindow>(argc, argv, &ctx);
}