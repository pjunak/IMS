
#include <simlib.h>


#define ENDTime 364
#define DAY_demand 900
#define DAY 1.0
const int max_d_vyroba = 900;
int cipy_v_zasobe = 6300;
int soucastky_v_zasobe = 6300;
int denni_poptavka = 0;
int vyrobeno_sklad = 900;
int satisfied = 0;
int unsatisfied = 0;

Store Vyroba("Vyroba",max_d_vyroba);

class vyrobna : public Process{
    void Behavior(){            
        Enter(Vyroba,900);
        Wait(DAY);
        Leave(Vyroba,900);
    }
    public: vyrobna(){
        Activate();
    }
};

class gen_vyrobna : public Event{
    void Behavior(){
        new vyrobna();
        Activate(Time+DAY);
    }
    public: gen_vyrobna(){
        Activate();
    }
};

class new_parts : public Process{
    void Behavior(){
        Wait(DAY);
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
        Activate(Time+DAY);
    }
    public: gen_parts(){
        Activate();
    }
};

class demand : public Process{
    void Behavior(){
        denni_poptavka+=DAY_demand;
        Wait(DAY);
    }
    public: demand(){
        Activate();
    }
};

class gen_demand : public Event{
    void Behavior(){
        new demand;
        Activate(Time+DAY);
    }
    public: gen_demand(){
        Activate();
    }
};


class demand_un_satisfied : public Process{
    void Behavior(){
        int timer = 0;
        time_t max_time;
        max_time = Exponential(30);
        while(timer < max_time){
            if(vyrobeno_sklad == 0){
                Wait(DAY);
            }
            else{
                satisfied += 1;
                break;
            }
            timer += DAY;
        }
        if(timer > max_time){
            unsatisfied += 1;
        }
    }
    public: demand_un_satisfied(){
        Activate();
    }
};

class gen_demand_un_satisfied : public Event{
    void Behavior(){
        new demand_un_satisfied;
        Activate(Time+DAY);
    }
};

int main(int argc, char **argv){
    Init(0,ENDTime);
    new gen_demand;
    for(int i=0; i< DAY_demand;i++){
        new gen_demand_un_satisfied;
    }
    new gen_vyrobna;
    new gen_parts;
    Run();
    Vyroba.Output();

    return EXIT_SUCCESS;
}
