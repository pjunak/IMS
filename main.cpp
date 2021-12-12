#include <simlib.h>
#include <iostream>
#include <cmath>

#define ENDTime 364
#define NUBER_of_customers 18
#define DAY 1.0
const int max_d_vyroba = 900;
int cipy_v_zasobe = 6300;
int soucastky_v_zasobe = 6300;
int denni_poptavka = 18;
int vyrobeno_sklad = 900;
int satisfied = 0;
int unsatisfied = 0;
int prodano_menicu = 0;
int impatient = 0;

Store Vyroba("Vyroba",max_d_vyroba);
Queue Fronta("Čekajici zakaznici");

class Timeout : public Event {
    Process *ptr;
    public: Timeout(double t, Process *p): ptr(p){
        Activate(Time+t);
    }
    void Behavior(){
		unsatisfied += 1;
        delete ptr;
        impatient++;
    }
};

class customer : public Process{
    void Behavior(){
        timeout = new Timeout(avg_wait_time,this);
        if(vyrobeno_sklad < 50){
            Into(Fronta);
            Passivate();
        }
        else{
            delete timeout;
            vyrobeno_sklad-=50;
            prodano_menicu+=50;
			satisfied += 1;
        }
        for( Queue::iterator p = Fronta.begin(); p != Fronta.end(); ++p){
            customer *z = (customer*)(*p);
            if(vyrobeno_sklad > 50 && z != nullptr){
                z->Out();
                delete z->timeout;
                vyrobeno_sklad-=50;
                prodano_menicu+=50;
				satisfied += 1;
                z->Activate();
            }
        }
    }
    public: customer(int t){
		avg_wait_time = round(Normal(t,(t/10)));
        Activate();
    }
    Event *timeout;
	int avg_wait_time;
};

class gen_customer : public Event{
    void Behavior(){
		for (int i = 0; i < daily; i++)
		{
			new customer(avg_wait_time);
			Activate(Time+DAY);
		}
    }
    public: gen_customer(int d, int t){
		daily = d;
		avg_wait_time = t;
		Activate();
    }
	int daily;
	int avg_wait_time;
};
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
    public: gen_chips(int mnozstvi, double cas){
		nove_soucastky = mnozstvi;
		if(cas != 0)
		{

			cas_tvorby_SW = round(Normal(cas, (cas/10)));
		}else
		{
			cas_tvorby_SW = 0;
		}
        Activate();
    }
	int nove_soucastky;
	int cas_tvorby_SW;
};

int main(int argc, char **argv){
    Init(0,ENDTime);
	RandomSeed(time(NULL));
	new gen_customer(atoi(argv[1]), atoi(argv[2]));
    new gen_vyrobna;
    new gen_parts;
    new gen_chips(300, 0);
	new gen_chips(600, atoi(argv[3]));
    Run();
    Vyroba.Output();
    Fronta.Output();
    printf("Satisfied: %d\nUnsatisfied: %d\n",satisfied,unsatisfied);
	printf("Prodáno měničů: %d\n",prodano_menicu);
    return EXIT_SUCCESS;
}
