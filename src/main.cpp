#include <gtkmm.h>
#include <iostream>
#include "Fractal.hpp"
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
    // FractalAlgorithm *fract = new Newton();
    // cout << fract->getIterations({2L, 3L}).second << endl;
    // cout << fract->getIterations({7L, -2L}).second << endl;
    // cout << fract->getIterations(Complex::ONE).second << endl;
    // cout << fract->getIterations({4L, 0L}).second << endl;

    /*
    const int MAP_SIZE = 2048;
    vector <pair <double, RGB> > gradient;
    gradient.push_back({0.0, {0, 7, 100}});
    gradient.push_back({0.16, {32, 107, 203}});
    gradient.push_back({0.42, {237, 255, 255}});
    gradient.push_back({0.6425, {255, 170, 0}});
    gradient.push_back({0.8575, {0, 2, 0}});
    gradient.push_back({1.0, {0, 5, 97}});
    auto colors = GradientGenerator::generateGradientMap(gradient, MAP_SIZE);
    */

    const int MAP_SIZE = 1024;
    PekiProcessing::Gradient gradient;
    gradient.insertPoint(0.0, {0, 7, 100});
    gradient.insertPoint(0.16, {32, 107, 203});
    gradient.insertPoint(0.42, {237, 255, 255});
    gradient.insertPoint(0.6425, {255, 170, 0});
    gradient.insertPoint(0.8575, {0, 2, 0});
    auto points = gradient.getPoints();
    for(const auto &p : points){
        cout << "(" << p.first << ", " << p.second << ")" << endl;
    }
    auto colors = gradient.generateGradientMap(MAP_SIZE);
    gradient.write("testujemy");

    /*
    Bitmap b(MAP_SIZE, MAP_SIZE);
    for(int i = 1; i <= MAP_SIZE; i++){
        uint8_t rr = colors[i - 1].getR();
        uint8_t gg = colors[i - 1].getG();
        uint8_t bb = colors[i - 1].getB();
        cout << i << " -> (" << (int)rr << ", " << (int)gg << ", " << (int)bb << ")" << endl;
        for(int j = 1; j <= MAP_SIZE; j++){
            b.setPixel(i, j, rr, gg, bb);
        }
    }
    b.write("test_gradient.bmp");
    */

    /*
    const int d = 512;
    PekiProcessing::Image b(d, d);
    //Scale s(d, d, -1.7L, 0.5L, -1.0L, 1.0L);
    //Scale s(d, d, -2.8L, 2.8L, -2.1L, 2.1L);
    Scale s(d, d, -2.2L, 2.2L, -1.65L, 1.65L);
    //vector <Complex> poly = {Complex::ONE, Complex::ZERO, Complex::ZERO, -Complex::ONE};
    //vector <Complex> poly = {Complex::ONE, Complex::ZERO, Complex(-2.0L, 0.0L), Complex(2.0L, 0.0L)};
    //vector <Complex> poly = {Complex::ONE, Complex::ZERO, Complex::ZERO, Complex::ZERO, 
    //                         Complex(3.0L, 0.0L), Complex::ZERO, Complex::ZERO, Complex::ZERO, Complex(-4.0L, 0.0L)};
    //vector <Complex> poly = {Complex::ONE, Complex(2.0L, 0.0L), Complex(3.0L, 1.0L), Complex::ZERO, Complex::ZERO, -Complex::ONE};
    //vector <Complex> poly = {Complex::ONE, Complex::ZERO, Complex(-2.0L, 0.2L)};
    unique_ptr<FractalAlgorithm> fractal = FractalAlgorithmCreator::createMultibrot(3);
    fractal->setMaxIterationsNumber(45);
    unique_ptr<FractalColoring> fcolor = 
        FractalColoringCreator::createSmoothConvergence(fractal->getMaxIterationsNumber(), 6, gradient);
    for(int i = 1; i <= d; i++){
        for(int j = 1; j <= d; j++){
            auto result = s.to_scale(i, j);
            Complex c(result.first, result.second);
            b.setPixel(i, j, fcolor->getPixel(fractal->getIterationsAndOrbit(c)));
        }
    }
    b.write("test1");
    */
    
    /*
    unique_ptr<Fractal> frac = FractalBuilder(512, 512).setAlgorithm(FractalAlgorithmCreator::createMandelbrot()).
                                                        setGradient(gradient).setMaxIterations(4000).build();
    frac->write("bigtest");
    
    int i = 0;
    while(true){
        char c; cin >> c;
        if(c == 'R'){
            long double a, b, c, d;
            cin >> a >> b >> c >> d;
            if(frac->setScale(a, b, c, d))
                frac->write(to_string(++i));
        }
        else{
            break;
        }
    }
    */
    /*
    app = Gtk::Application::create("org.gtkmm.example");
    app->signal_activate().connect([] () { on_app_activate(); });
    return app->run(argc, argv);
    */
    return 0;
}