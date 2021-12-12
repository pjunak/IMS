#include <simlib.h>
#include <cmath>

#define ENDTime 364
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
        for( Queue::iterator p = Fronta.begin(); p != Fronta.end(); ++p)
		{
            customer *z = (customer*)(*p);
            if(vyrobeno_sklad >= 50){
                z->Out();
                delete z->timeout;
                vyrobeno_sklad-=50;
                prodano_menicu+=50;
				satisfied += 1;
                z->Activate();
            } else
			{
				break;
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
    Init(0, (argc > 1) ? atoi(argv[1]) : 365);
	RandomSeed(time(NULL));
    new gen_vyrobna;
    new gen_parts;
    new gen_chips((argc > 3) ? 300 : 900, 0);
	new gen_customer((argc > 2) ? atoi(argv[2]) : 18, (argc > 3) ? atoi(argv[3]) : 90);
	if(argc > 3)
	{
		new gen_chips(600, (argc > 4) ? atoi(argv[4]) : 60);
	}
    Run();
    Vyroba.Output();
    Fronta.Output();
	Print("Na konci obdobé zbylo na interních skladech:\n%d čipů\n%d ostatních součástek\n%d neprodaných měničů\n", cipy_v_zasobe, soucastky_v_zasobe, vyrobeno_sklad);
    Print("Satisfied: %d\nUnsatisfied: %d\n",satisfied,unsatisfied);
	Print("Prodáno měničů: %d\n",prodano_menicu);
	Print("Odhadovaný přibližný obrat z prodeje: %d Euro\n",prodano_menicu*8000/25);
	Print("Odhadovaná přibližná výše hrubého zisku podle marže z prodeje: %d\n",(prodano_menicu*8000/25)/100*23);
	Print("Odhadované přibližné ztráty hrubého zisku způsobené nedodáním zboží: %d Euro\n",(unsatisfied*((argc > 2) ? atoi(argv[2]) : 18)*8000/25)/100*23);
    return EXIT_SUCCESS;
}
