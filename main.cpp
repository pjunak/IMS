
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
int prodano_menicu = 0;

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

class parts : public Process{
    void Behavior(){
        Wait(DAY);
        soucastky_v_zasobe += max_d_vyroba;
    }
    public: parts(){
        Activate();
    }
};

class gen_parts : public Event{
    void Behavior(){
        new parts;
        Activate(Time+DAY);
    }
    public: gen_parts(){
        Activate();
    }
};

class chips : public Process{
    void Behavior(){
        Wait(DAY);
		if(cas_do_dokonceni_SW <= 0)
		{
			cipy_v_zasobe += nove_cipy;	
		}
    }
    public: chips(int mnozstvi, int cas){
		nove_cipy = mnozstvi;
		cas_do_dokonceni_SW = cas;
		Activate();
    }
	int nove_cipy;
	int cas_do_dokonceni_SW;
};

class gen_chips : public Event{
    void Behavior(){
        new chips(nove_soucastky, cas_tvorby_SW);
		if (cas_tvorby_SW > 0)
		{
			cas_tvorby_SW--;
		}
        Activate(Time+DAY);
    }
    public: gen_chips(int mnozstvi, int cas){
		nove_soucastky = mnozstvi;
		cas_tvorby_SW = cas;
        Activate();
    }
	int nove_soucastky;
	int cas_tvorby_SW;
};

class customer : public Process{
    void Behavior(){
        denni_poptavka+=number_of_daily_customers;
        Wait(DAY);
    }
    public: customer(int number){
		number_of_daily_customers = number;
        Activate();
    }
	int number_of_daily_customers;
};

class gen_customer : public Event{
    void Behavior(){
        new customer(number_of_daily_customers);
        Activate(Time+DAY);
    }
    public: gen_customer(int number){
		number_of_daily_customers = number;
        Activate();
    }
	int number_of_daily_customers;
};

class customer_satisfied : public Process{
    void Behavior(){
        int timer = 0;
        time_t max_time;
        max_time = 90;
        while(timer <= max_time){
            if(vyrobeno_sklad < 50){
                Wait(DAY);
                timer += DAY;
            }
            else{
                satisfied += 1;
                vyrobeno_sklad -=50;
				prodano_menicu += 50;
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
    new gen_customer(18);
    for(int i=0; i< NUBER_of_customers;i++){
        new gen_customer_satisfied;
    }
    new gen_vyrobna;
    new gen_parts;
    new gen_chips(300, 0);
	new gen_chips(600, 60);
    Run();
    Vyroba.Output();
    printf("Satisfied: %d\nUnsatisfied: %d\n",satisfied,unsatisfied);
	printf("Prodáno měničů: %d\n",prodano_menicu);
    return EXIT_SUCCESS;
}
