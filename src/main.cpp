#include <gtkmm.h>
#include <iostream>
#include "BitmapFileHeader.hpp"
#include "BitmapInfoHeader.hpp"
#include "Bitmap.hpp"
#include "Complex.hpp"
#include "Scale.hpp"
#include "Mandelbrot.hpp"

Gtk::Window *pWindow = nullptr;
Glib::RefPtr<Gtk::Application> app;

void on_button_clicked()
{
    if (pWindow != nullptr)
        pWindow->hide(); //hide() will cause Gtk::Application::run() to end.
}

void on_app_activate()
{
    // Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = nullptr;
    try
    {
        refBuilder = Gtk::Builder::create_from_file("../resources/gtk4interface.glade");
    }
    catch(const Glib::FileError& ex)
    {
        std::cout << "FileError: " << ex.what() << std::endl;
        return;
    }
    catch(const Glib::MarkupError& ex)
    {
        std::cout << "MarkupError: " << ex.what() << std::endl;
        return;
    }
    catch(const Gtk::BuilderError& ex)
    {
        std::cout << "BuilderError: " << ex.what() << std::endl;
        return;
    }

    // Get the GtkBuilder-instantiated window:
    pWindow = refBuilder->get_widget<Gtk::Window>("mainWindow");
    auto img = refBuilder->get_widget<Gtk::Image>("lol_img");
    img->set("../resources/fractal.jpg");
    if (pWindow == nullptr)
    {
        std::cerr << "Could not get the window" << std::endl;
        return;
    }

    // Get the GtkBuilder-instantiated button, and connect a signal handler:
    auto pButton = refBuilder->get_widget<Gtk::Button>("mainButton");
    if (pButton){
        pButton->signal_clicked().connect([] () { on_button_clicked(); });
    }
    else{
        std::cout << "Cannot get the button" << std::endl;
    }

    // It's not possible to delete widgets after app->run() has returned.
    // Delete the dialog with its child widgets before app->run() returns.
    pWindow->signal_hide().connect([] () { delete pWindow; });

    app->add_window(*pWindow);
    pWindow->show();
}

int main(int argc, char** argv)
{
    const int d = 10000;
    Bitmap b(d, d);
    Scale s(d, d, -0.957, -0.893, 0.234, 0.298);
    Mandelbrot::setMaxIterations(250);
    for(int i = 1; i <= d; i++){
        for(int j = 1; j <= d; j++){
            auto result = s.to_scale(i, j);
            //cout << "(" << i << ", " << j << ") ---> (" << result.first << ", " << result.second << ")" << endl;
            Complex c(result.first, result.second);
            int iter = Mandelbrot::getIterations(c);
            int8_t color = iter * 255 / Mandelbrot::getMaxIterations();
            b.setPixel(i, j, color, color, color);
        }
    }
    b.write("test.bmp");
    /*
    app = Gtk::Application::create("org.gtkmm.example");
    app->signal_activate().connect([] () { on_app_activate(); });
    return app->run(argc, argv);
    */
   return 0;
}