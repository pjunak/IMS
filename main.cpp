
#include <simlib.h>


#define ENDTime = 365;
const int max_d_vyroba = 900;

Store Vyroba("Vyroba",max_d_vyroba);

class vyrobna : public Process{
    double Arrival;
    void Behavior(){
        Enter(Vyroba,900);
        Wait(1);
        Leave(Vyroba,900);
    }
    public: vyrobna(){
        Activate();
    }
};

int main(int argc, char **argv){



    return EXIT_SUCCESS;
}