#include "Newton.hpp"

const Complex Newton::NEUTRAL_INCREMENT(0.0L, 0.0L);
const Complex Newton::NEUTRAL_RELAXATION(1.0L, 0.0L);

Newton::Newton(){

}

Newton::Newton(const vector <Complex> &polynomial){

}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relax){

}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relax, const Complex &incrementation){

}

pair <int, Complex> getIterations(const Complex &c){
    return {0, Newton::NEUTRAL_INCREMENT}; // TO DO
}