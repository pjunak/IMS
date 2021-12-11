
#include <simlib.h>


#define ENDTime 364
#define NUBER_of_customers 18
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
        cipy_v_zasobe-=max_d_vyroba;
        soucastky_v_zasobe-=max_d_vyroba;            
        Enter(Vyroba,900);
        Wait(DAY);
        Leave(Vyroba,900);
        vyrobeno_sklad+=900;
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
        cipy_v_zasobe += 300;
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

class customer : public Process{
    void Behavior(){
        denni_poptavka+=NUBER_of_customers;
        Wait(DAY);
    }
    public: customer(){
        Activate();
    }
};

class gen_customer : public Event{
    void Behavior(){
        new customer;
        Activate(Time+DAY);
    }
    public: gen_customer(){
        Activate();
    }
};

class customer_satisfied : public Process{
    void Behavior(){
        int timer = 0;
        time_t max_time;
        max_time = Exponential(90);
        while(timer < max_time){
            if(vyrobeno_sklad < 50){
                Wait(DAY);
            }
            else{
                satisfied += 1;
                vyrobeno_sklad -=50;
                break;
            }
            timer += DAY;
        }
        if(timer > max_time){
            unsatisfied += 1;
        }
    }
    public: customer_satisfied(){
        Activate();
    }
};

class gen_customer_satisfied : public Event{
    void Behavior(){
        new customer_satisfied;
        Activate(Time+DAY);
    }
    public: gen_customer_satisfied(){
        Activate();
    }
};

int main(int argc, char **argv){
    Init(0,ENDTime);
    new gen_customer;
    for(int i=0; i< NUBER_of_customers;i++){
        new gen_customer_satisfied;
    }
    new gen_vyrobna;
    new gen_parts;
    Run();
    Vyroba.Output();
    printf("Satisfied: %d\nUnsatisfied: %d\n",satisfied,unsatisfied);
    return EXIT_SUCCESS;
}
