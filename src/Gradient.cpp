#include <algorithm>
#include <functional>
#include "Gradient.hpp"
#include "CompareDoubles.hpp"
using namespace PekiProcessing;

// Gradient and RGB static members initialization
const RGB RGB::WHITE(255, 255, 255);
const RGB RGB::GRAY(128, 128, 128);
const RGB RGB::BLACK(0, 0, 0);
const RGB RGB::RED(255, 0, 0);
const RGB RGB::GREEN(0, 255, 0);
const RGB RGB::BLUE(0, 0, 255);
const Gradient Gradient::WHITE_BLACK({{0.0, RGB::WHITE}, {0.5, RGB::GRAY}, {1.0, RGB::BLACK}});

Gradient::Gradient(const double x, const RGB &color) : 
    grad_points(1, {x, color}) 
{}

Gradient::Gradient(const vector < pair <double, RGB> > &gradient_points) :
    grad_points(gradient_points)
{
    // Gradient points have to be sorted for proper interpolation
    sort(grad_points.begin(), grad_points.end(), 
        [](const auto &x, const auto &y)
          {return CompareDoubles::isLesser(x.first, y.first);});
}

bool Gradient::insertPoint(const double x, const RGB &color)
{
    // Point already inside. Can't add point in the same position.
    if(isPointInside(x))
        return false;
    
    // Insertion sort to keep gradient points sorted
    grad_points.emplace_back(x, color);
    int ind = (int)grad_points.size() - 2;
    while(ind >= 0 && CompareDoubles::isGreater(grad_points[ind].first, grad_points[ind + 1].first)){
        swap(grad_points[ind], grad_points[ind + 1]);
        ind--;
    }
    interpolated = false; // Successfully added point. Need to interpolate
    return true;
}

bool Gradient::changePoint(const double x, const RGB &color)
{
    // Remove given point and insert again with changed value
    
    bool removed = removePoint(x);
    if(!removed) return false; // Point wasn't inside
    
    // Point inside
    insertPoint(x, color);
    return true;
}

bool Gradient::isPointInside(const double x) noexcept
{
    return getPointIter(x) != grad_points.end();
}

bool Gradient::removePoint(const double x)
{
    // Given point isn't inside.
    auto wanted = getPointIter(x);
    if(wanted == grad_points.end())
        return false;
    
    // Point inside. Just remove and set interpolation flag
    grad_points.erase(wanted);
    interpolated = false;
    return true;
}

RGB Gradient::getPointColor(const double x)
{
    // If point is inside then color will be returned else invalid argument exception is thrown
    auto wanted = getPointIter(x);
    if(wanted == grad_points.end()){
        throw invalid_argument("The point at the position does not belong to the gradient set of points");
    }
    return wanted->second;
}

vector <pair <double, RGB> > Gradient::getPoints() noexcept
{
    return grad_points;
}

vector<RGB> Gradient::generateGradientMap(int map_size)
{
    // Can't generate map with negative size
    assert(map_size >= 0);
    // Base case if user want just empty gradient map
    if(map_size == 0){
        return vector<RGB>();
    }
    // No points inside. Gradient map is just BLACK
    if((int)grad_points.size() == 0){
        return vector<RGB>(map_size, RGB(0, 0, 0));
    }
    // One points inside. Gradient map is just color of the point
    if((int)grad_points.size() == 1){
        return vector<RGB>(map_size, grad_points.front().second);
    }
    
    // At least 2 points inside we can interpolate
    double begin = grad_points.front().first;
    double length = grad_points.back().first - begin;
    // Solve map_size = 1 case, because we want to divide by (map_size - 1)
    if(map_size == 1){
        return vector<RGB>(map_size, interpolate(begin + length / 2.0));
    }
    double step = length / (double)(map_size - 1);
    vector <RGB> result;
    result.reserve(map_size);
    for(int i = 0; i < map_size; i++){
        double s = begin + i * step;
        result.emplace_back(interpolate(s));
    }
    return result;
}

bool Gradient::write(string filename, int w, int h){
    if(w <= 0 || h <= 0)
        return false;
    auto colors = generateGradientMap(w); 
    Image gradientImage(w, h);
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            gradientImage.setPixel(i + 1, j + 1, colors[i]);
        }
    }
    return gradientImage.write(filename);
}


vector <pair <double, RGB> >::iterator Gradient::getPointIter(const double x) noexcept
{
    for(auto it = grad_points.begin(); it != grad_points.end(); it++){
        if(CompareDoubles::isEqual(it->first, x))
            return it;
    }
    return grad_points.end();
}


void Gradient::generateMonotoneIntervals(){
    // Nothing to do
    if(interpolated)
        return;

    // Assumption: we have at least 2 points inside
    assert((int)grad_points.size() != 0);
    assert((int)grad_points.size() != 1);

    // Gradient consisting from at least 2 points - right part of algorithm
    // Support function for separate all colors
    auto get_color_values = [this](function <uint8_t(RGB)> f){
        vector <int> v;
        v.reserve((int)grad_points.size());
        for(const auto &x : grad_points){
            v.emplace_back((int)f(x.second));
        }
        return v;
    };

    // Separate all colors(interpolate each color individually)
    auto rvalues = get_color_values(&RGB::getR);
    auto gvalues = get_color_values(&RGB::getG);
    auto bvalues = get_color_values(&RGB::getB);

    // Next step is to find inervals of monotonicity for all colors
    // Support function for this purpose
    auto get_mono_intervals = [&](const auto & values){
        vector <double> X{grad_points[0].first, grad_points[1].first};
        vector <double> Y{(double)values[0], (double)values[1]};
        vector <tuple<tk::spline, int, int> > intervals;

        auto add_interval = [&intervals, &X, &Y](const int a, const int b){
            const double correction = CompareDoubles::EPSILON;
            if((int)X.size() == 2){
                X.emplace_back(X.back() + correction);
                Y.emplace_back(Y.back());
            }
            tk::spline s;
            s.set_points(X, Y, tk::spline::cspline);
            s.make_monotonic();
            intervals.emplace_back(s, a, b);
        };

        auto sign = [](const int &x){
            if(x < 0) return -1;
            else if(x == 0) return 0;
            return 1;
        };

        int start = 0;
        int seg_sign = sign(values[1] - values[0]);

        for(int i = 2; i < (int)values.size(); i++){
            int p_sign = sign(values[i] - values[i - 1]);
            // Interval found
            if(p_sign != 0 && p_sign != seg_sign){  
                // Setting interval and spline
                add_interval(start, i - 1);
                
                X = {grad_points[i - 1].first, grad_points[i].first};
                Y = {(double)values[i - 1], (double)values[i]};
                start = i - 1;
                seg_sign = p_sign;
                continue;
            }
            X.push_back(grad_points[i].first);
            Y.push_back((double)values[i]);
        }
        add_interval(start, (int)values.size() - 1);
        return intervals;
    };

    // Getting monotone intervals using support function
    mono_intervals[static_cast<uint8_t>(InterpolationTarget::RED)]   = get_mono_intervals(rvalues);
    mono_intervals[static_cast<uint8_t>(InterpolationTarget::GREEN)] = get_mono_intervals(gvalues);
    mono_intervals[static_cast<uint8_t>(InterpolationTarget::BLUE)]  = get_mono_intervals(bvalues);
    interpolated = true;
}

RGB Gradient::interpolate(const double x)
{
    // If we have no points inside then interpolation value is BLACK for any x value
    if((int)grad_points.size() == 0){
        return RGB(0, 0, 0);
    }
    // If we have one point inside then interpolation is RGB value of this point
    if((int)grad_points.size() == 1){
        return grad_points.front().second;
    }
    
    // Given point has to be inside of interval [begin, end] where
    // begin, end are respectively first.x, last.x in grad_points vector
    int is_outside = CompareDoubles::isLesser(x, grad_points.front().first) ||
                     CompareDoubles::isGreater(x, grad_points.back().first);
    assert(!is_outside);
    
    // Check if we have interpolation functions and intervals generated
    if(!interpolated){
        generateMonotoneIntervals();
    }

    // Support function for get interpolated value in specific color
    auto get_interpolated = [this](const auto &v, double xx) -> double
    {
        auto is_inside = [](const double &a, const double &b, const double &val){
            return (CompareDoubles::isGreater(val, a) || CompareDoubles::isEqual(val, a)) &&
                   (CompareDoubles::isLesser(val, b)  || CompareDoubles::isEqual(val, b));
        };

        for(const auto &interval : v){
            double a = grad_points[get<1>(interval)].first;
            double b = grad_points[get<2>(interval)].first;
            if(is_inside(a, b, xx))
                return get<0>(interval)(xx);
        }
        return -1.0; // Result is inside of for, just to silent warnings
    };

    // Anwser build
    int r = get_interpolated(mono_intervals[static_cast<uint8_t>(InterpolationTarget::RED)], x);
    int g = get_interpolated(mono_intervals[static_cast<uint8_t>(InterpolationTarget::GREEN)], x);
    int b = get_interpolated(mono_intervals[static_cast<uint8_t>(InterpolationTarget::BLUE)], x);
    assert(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);
    return RGB(r, g, b);
}
