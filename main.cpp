
#include <simlib.h>
#include <iostream>


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
		int denni_vyroba = 0;
		while(cipy_v_zasobe > 0 && soucastky_v_zasobe > 0 && denni_vyroba < 900)
		{   
            if(cipy_v_zasobe < max_d_vyroba){
                denni_vyroba = cipy_v_zasobe;
            }
            else{
                denni_vyroba = max_d_vyroba;
            }
            Enter(Vyroba,denni_vyroba);
            Wait(DAY);
            Leave(Vyroba,denni_vyroba);
			cipy_v_zasobe-=denni_vyroba;
			soucastky_v_zasobe-=denni_vyroba;
			vyrobeno_sklad+=denni_vyroba;
		}
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

class new_chips : public Process{
    int dt;
    void Behavior(){
        Wait(DAY);
        soucastky_v_zasobe += dt;
        
    }
    public: new_chips(int d){
        dt = d;
        Activate();
    }
};

class gen_chips : public Event{
    int dt;
    void Behavior(){
        new new_chips(dt);
        //std::cout << dt << std::endl;
        Activate(Time+DAY);
    }
    public: gen_chips(int d): dt(d){
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
        max_time = Exponential(30);
        while(timer <= max_time){
            if(vyrobeno_sklad < 50){
                Wait(DAY);
                timer += DAY;
            }
            else{
                satisfied += 1;
                vyrobeno_sklad -=50;
                break;
            }
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
    new gen_chips(900);
    Run();
    Vyroba.Output();
    printf("Satisfied: %d\nUnsatisfied: %d\n",satisfied,unsatisfied);
    return EXIT_SUCCESS;
}
