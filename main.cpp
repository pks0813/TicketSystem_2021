#include "include.hpp"
int main(){
    hc(sizeof(Corn::User));
    hc(sizeof(Corn::TrainInfo));
    hc(sizeof(Corn::Order));
    hc(sizeof(Corn::Seat));
    Corn pks;
    pks.game();
}
