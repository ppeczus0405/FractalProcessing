#include <algorithm>
#include <functional>
#include "GradientGenerator.hpp"
#include "CompareDoubles.hpp"
#include "spline.hpp"

vector<RGB> GradientGenerator::generateGradientMap(vector <pair <double, RGB> > gradient_points, int map_size)
{
    if((int)gradient_points.size() == 0){
        return vector<RGB>(map_size, {0, 0, 0});
    } // For gradient consisting from 0 points -> MAP[0...(map_size - 1)] = black
    if((int)gradient_points.size() == 1){
        return vector<RGB>(map_size, {gradient_points[0].second});
    } // For gradient consisting from 1 point -> MAP[0...(map_size - 1)] = given_color

    // Gradient consisting from at least 2 points - right part of algorithm
    // At the beginning sort values by position
    sort(gradient_points.begin(), gradient_points.end(), 
         [](const auto &x, const auto &y)
            {return CompareDoubles::isLesser(x.first, y.first);});
    
    // Next step is to find inervals of monotonicity for all colors
    // Support function for separate all colors
    auto get_color_values = [&gradient_points](function <uint8_t(RGB)> f){
        vector <int> v;
        v.reserve((int)gradient_points.size());
        for(const auto &x : gradient_points){
            v.push_back((int)f(x.second));
        }
        return v;
    };
    // Separate all colors(interpolate each color individually)
    auto rvalues = get_color_values(&RGB::getR);
    auto gvalues = get_color_values(&RGB::getG);
    auto bvalues = get_color_values(&RGB::getB);
    
    /*
    auto debug_values = [](const auto &values){
        cout << "[";
        for(int i = 0; i < (int)values.size(); i++){
            cout << values[i];
            if(i != (int)values.size() - 1){
                cout << ", ";
            }
        }
        cout << "]";
    };

    cout << "rvalues = "; debug_values(rvalues); cout << endl;
    cout << "gvalues = "; debug_values(gvalues); cout << endl;
    cout << "bvalues = "; debug_values(bvalues); cout << endl;
    */

    // Support function to get monotonicity intervals and set interpolation object for each interval
    auto get_mono_intervals = [&](const auto & values){
        vector <double> X{gradient_points[0].first, gradient_points[1].first};
        vector <double> Y{(double)values[0], (double)values[1]};
        vector <tuple<tk::spline, int, int> > intervals;

        auto add_interval = [&intervals, &X, &Y](const int a, const int b){
            const double correction = 0.00001f;
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
                
                X = {gradient_points[i - 1].first, gradient_points[i].first};
                Y = {(double)values[i - 1], (double)values[i]};
                start = i - 1;
                seg_sign = p_sign;
                continue;
            }
            X.push_back(gradient_points[i].first);
            Y.push_back((double)values[i]);
        }
        // cout << "X = "; debug_values(X); cout << endl;
        // cout << "Y = "; debug_values(Y); cout << endl;
        add_interval(start, (int)values.size() - 1);
        return intervals;
    };

    auto rintervals = get_mono_intervals(rvalues);
    auto gintervals = get_mono_intervals(gvalues);
    auto bintervals = get_mono_intervals(bvalues);

    /*
    auto debug_intervals = [](const auto &intervals){
        cout << "[";
        for(int i = 0; i < (int)intervals.size(); i++){
            cout << "(" << get<1>(intervals[i]) << ", " << get<2>(intervals[i]) << ")";
            if(i != (int)intervals.size() - 1){
                cout << ", ";
            }
        }
        cout << "]";
    };

    cout << "rintervals = "; debug_intervals(rintervals); cout << endl;
    cout << "gintervals = "; debug_intervals(gintervals); cout << endl;
    cout << "bintervals = "; debug_intervals(bintervals); cout << endl;
    */

    auto get_interpolated = [&gradient_points](const auto &v, double x) -> double
    {
        auto is_inside = [](const double &a, const double &b, const double &val){
            return (CompareDoubles::isGreater(val, a) || CompareDoubles::isEqual(val, a)) &&
                   (CompareDoubles::isLesser(val, b)  || CompareDoubles::isEqual(val, b));
        };

        for(const auto &interval : v){
            double a = gradient_points[get<1>(interval)].first;
            double b = gradient_points[get<2>(interval)].first;
            if(is_inside(a, b, x))
                return get<0>(interval)(x);
        }
    };

    double begin = gradient_points.front().first;
    double length = gradient_points.back().first - begin;
    double step = length / (double)(map_size - 1);

    vector <RGB> result;
    result.reserve(map_size);
    for(int i = 0; i < map_size; i++){
        double s = begin + i * step;
        int r = get_interpolated(rintervals, s);
        int g = get_interpolated(gintervals, s);
        int b = get_interpolated(bintervals, s);
        assert(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);
        result.emplace_back(r, g, b);
    }
    return result;
}