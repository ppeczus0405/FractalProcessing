#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "BitmapFileHeader.hpp"
#include "BitmapInfoHeader.hpp"
#include "Bitmap.hpp"
#include "Complex.hpp"
#include "Scale.hpp"
#include "Mandelbrot.hpp"
#include "GradientGenerator.hpp"
#include "spline.hpp"
using namespace std;

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
    const int MAP_SIZE = 2048;
    vector <pair <double, RGB> > gradient;
    gradient.push_back({0.0, {0, 7, 100}});
    gradient.push_back({0.16, {32, 107, 203}});
    gradient.push_back({0.42, {237, 255, 255}});
    gradient.push_back({0.6425, {255, 170, 0}});
    gradient.push_back({0.8575, {0, 2, 0}});
    auto colors = GradientGenerator::generateGradientMap(gradient, MAP_SIZE);
    
    /*
    Bitmap b(MAP_SIZE, MAP_SIZE);
    for(int i = 1; i <= MAP_SIZE; i++){
        uint8_t rr = colors[i - 1].getR();
        uint8_t gg = colors[i - 1].getG();
        uint8_t bb = colors[i - 1].getB();
        cout << i << " -> (" << (int)rr << ", " << (int)gg << ", " << (int)bb << ")" << endl;
        for(int j = 1; j <= MAP_SIZ);E; j++){
            b.setPixel(i, j, rr, gg, bb);
        }
    }
    b.write("test_gradient.bmp");
    */
    
    const int d = 10024;
    Bitmap b(d, d);
    Scale s(d, d, -0.957f, -0.893f, 0.234f, 0.298f);
    Mandelbrot::setMaxIterations(250);
    for(int i = 1; i <= d; i++){
        for(int j = 1; j <= d; j++){
            auto result = s.to_scale(i, j);
            //cout << "(" << i << ", " << j << ") ---> (" << result.first << ", " << result.second << ")" << endl;
            Complex c(result.first, result.second);
            auto end = Mandelbrot::getIterations(c);
            int iter = end.second;
            uint8_t rr = 0, gg = 0, bb = 0;
            if(iter < Mandelbrot::getMaxIterations()){
                double smoothed = log2(log2(Complex::absolute_square(end.first)) / 2);
                int colorI = (int)(sqrt(iter + 10 - smoothed) * 256) % MAP_SIZE;
                rr = colors[colorI].getR();
                gg = colors[colorI].getG();
                bb = colors[colorI].getB();
            }
            b.setPixel(i, j, rr, gg, bb);
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