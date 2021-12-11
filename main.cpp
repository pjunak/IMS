
#include <simlib.h>


#define ENDTime = 365
#define year_demand = 328500
#define day = 1
const int max_d_vyroba = 900;
int cipy_v_zasobe = 6300;
int soucastky_v_zasobe = 6300;
int denni_poptavka = 0;
int vyrobeno_sklad = 900;

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

class new_parts : public Process{
    void Behavior(){
        Wait(day);
        cipy_v_zasobe += max_d_vyroba;
        soucastky_v_zasobe += max_d_vyroba;
    }
    public: new_parts(){
        Activate();
    }
};

class gen_parts : public Event{
    void Behavior(){
        new new_parts;
        Activate(Time+day);
    }
    public: gen_parts(){
        Activate();
    }
};

class demand : public Process{
    void Behavioral(){
        denni_poptavka+=max_d_vyroba;
        Wait(day);
    }
    public: demand(){
        Activate();
    }
};

class gen_demand : public Event{
    void Behavior(){
        new demand;
        Activate(Time+day);
    }
    public: gen_demand(){
        Activate();
    }
};


//not done
class demand_un_satisfied : public Process{
    void Behavior(){
        if(vyrobeno_sklad > 0){
            vyrobeno_sklad--;
        }
        else{
            Wait()
        }
    }
};

int main(int argc, char **argv){



    return EXIT_SUCCESS;
}

