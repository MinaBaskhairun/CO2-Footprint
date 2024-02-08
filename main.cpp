/***********************************************************************
Matr.-Nr:                     3115219
Nachname/Surname:             Baskhairun
Vorname/Given name:           Mina
Uni-Email:                    mina.baskhairun@stud.uni-due.de
Studiengang/Course of studies: Software Engineering ISE
***********************************************************************/


#include <iostream>
#include <iomanip>

using namespace std;

enum struct Frequency
{
    once, daily, weekly, monthly
};

ostream& operator<<(ostream& out, Frequency& fre)
{
    switch(fre){
    case Frequency::once:
        return out << "once";
    case Frequency::daily:
        return out << "daily";
    case Frequency::weekly:
        return out << "weekly";
    case Frequency::monthly:
        return out << "monthly";
    default:
        return out << "unknown frequency";
    }
}

istream& operator>>(istream& in, Frequency& fre)
{
    char choice;
    cout << "frequency input: " << endl;
    cout << "(o) once" << endl << "(d) daily" << endl << "(m) monthly" << endl << "(w) weekly" << endl << "your choice: ";
    in >> choice;
    if(choice == 'o')
        fre = Frequency::once;
    else if (choice == 'd')
        fre = Frequency::daily;
    else if (choice == 'w')
        fre = Frequency::weekly;
    else if (choice == 'm')
        fre = Frequency::monthly;
    else{
        cerr << "Frequency unknown!";
        exit(-1);
    }
    return in;
}

class CO2Emission{
private:
    CO2Emission* next;
public:
    CO2Emission();
    virtual ~CO2Emission();
    CO2Emission* get_next();
    void set_next(CO2Emission* nextValue);
    virtual float get_co2() = 0;
    virtual void print() = 0;
};

CO2Emission::CO2Emission(): next(nullptr){}

CO2Emission::~CO2Emission() {cout << "destructor CO2Emission: done"<<endl<<endl;}

CO2Emission* CO2Emission::get_next(){return next;}

void CO2Emission::set_next(CO2Emission* nextValue){next=nextValue;}

class Transport: public CO2Emission{
private:
    float km;
    Frequency frequency;
    float virtual get_co2_pkm() = 0;
public:
    Transport();
    Transport(float initKm, Frequency initFrequency);
    virtual ~Transport();
    float get_co2();
    void print();
};

Transport::Transport()
{
    cout << "transport distance kilometers: ";
    cin >> km;
    cin >> frequency;
}

Transport::Transport(float initKm, Frequency initFrequency=Frequency::once):
    km(initKm),
    frequency(initFrequency)
    {}

Transport::~Transport()
{
    cout << "destructor Transport: " << frequency << " " << km << " km done" <<endl;
}

float Transport::get_co2()
{
    if(frequency==Frequency::once)
        return get_co2_pkm()*km;
    else if(frequency==Frequency::daily)
        return get_co2_pkm()*km*365;
    else if(frequency==Frequency::weekly)
        return get_co2_pkm()*km*52;
    else if(frequency==Frequency::monthly)
        return get_co2_pkm()*km*12;
    else{
        exit(-1);
    }
}

void Transport::print()
{
    cout << " transport " << frequency << " " << fixed << setprecision(1) << km << " km (" << fixed << setprecision(3) << get_co2_pkm() << " kg CO2/pkm) " << fixed << setprecision(1) << get_co2() << " kg CO2"<<endl;
}

class Power: public CO2Emission{
private:
    float kWh;
public:
    Power();
    Power(float initKWh);
    virtual ~Power();
    virtual float get_co2_kWh()= 0;
    float get_co2();
    void print();
};

Power::Power()
{
    cout << "power data input kilo Watt hours: ";
    cin >> kWh;
}

Power::Power(float initKWh):
    kWh(initKWh)
    {}

Power::~Power()
{
    cout << "destructor Power: " << kWh << " kWh done" <<endl;
}

float Power::get_co2()
{
    return get_co2_kWh()*kWh;
}

void Power::print()
{
    cout << " power " << fixed << setprecision(1) << kWh << " kWh (" << fixed << setprecision(3) << get_co2_kWh() << " kg CO2/kWh) " << fixed << setprecision(1) << get_co2() << " kg CO2"<<endl;
}

class Bus: public Transport{
private:
    bool local_transport;
    float get_co2_pkm();
public:
    Bus();
    Bus(bool initLongdist, float initKm);
    virtual ~Bus();
    void print();
};

Bus::Bus()
{
    int temp;
    cout << "(1) local transport" <<endl<< "(2) long distance"<<endl;
    cout << "your choice: ";
    cin >> temp;
    local_transport=temp-1;
}

Bus::Bus(bool initLongdist, float initKm):
    local_transport(initLongdist),
    Transport(initKm)
{}

Bus::~Bus()
{
    cout << "destructor Bus: ";
    print();
}

float Bus::get_co2_pkm()
{
    if(local_transport==true){
        return 0.036;
    }
    else{
        return 0.111;
    }
}

void Bus::print()
{
    if(local_transport==true){
        cout << "long distance bus ";
    }
    else{
        cout << "local bus ";
    }
    Transport::print();
}

class Car: public Transport{
private:
    float l_fuel_km;
    float get_co2_pkm();
public:
    Car();
    Car(float initFuel, float initKm);
    virtual ~Car();
    void print();
};

Car::Car()
{
    cout << "passenger car consumption liter petrol per 100 kilometers: ";
    cin >> l_fuel_km;
}

Car::Car(float initFuel, float initKm):
    l_fuel_km(initFuel),
    Transport(initKm)
{}

Car::~Car()
{
    cout << "destructor Car: ";
    print();
}

float Car::get_co2_pkm()
{
    return 0.023*l_fuel_km;
}

void Car::print()
{
    cout<< "passenger car ";
    Transport::print();
}

class Cruise: public CO2Emission{
private:
    float days;
    float sea_days;
    float get_co2_day(bool seaDay);
public:
    Cruise();
    Cruise(float initDays,float initSeadays);
    virtual ~Cruise();
    float get_co2();
    void print();
};

Cruise::Cruise(): CO2Emission()
{
    cout <<"\ncruise holiday data input: "<<endl;
    cout <<"number of days of the whole cruise: ";
    cin >> days;
    cout <<"number of sea days: ";
    cin >> sea_days;
}

Cruise::Cruise(float initDays, float initSeadays)
{
    days=initDays;
    sea_days=initSeadays;
}

Cruise::~Cruise()
{
     cout << "destructor Cruise: ";
     print();
}

float Cruise::get_co2_day(bool seaDay)
{
    if(seaDay==true)
        return 280;
    else
        return 190;
}

float Cruise::get_co2()
{
    return get_co2_day(true)*sea_days+get_co2_day(false)*(days-sea_days);
}

void Cruise::print()
{
    cout << "cruise holiday (without flights): " << fixed << setprecision(0) << days << " days duration with " << fixed << setprecision(0) << sea_days << " sea days (sea day "<< fixed << setprecision(3) << get_co2_day(true) << " kg CO2/pkm, harbour day " << get_co2_day(false) << " kg CO2/pkm) " << fixed << setprecision(1) << get_co2() << " kg CO2" <<endl;
}

class Flight: public Transport{
private:
    float get_co2_pkm();
public:
    Flight();
    Flight(float initKm);
    virtual ~Flight();
    void print();
};

Flight::Flight(){}

Flight::Flight(float initKm):
    Transport(initKm)
    {}


Flight::~Flight()
{
    cout << "destructor Flight: done";
    print();
}

float Flight::get_co2_pkm()
{
    return 0.27;
}

void Flight::print()
{
    cout << "flight ";
    Transport::print();
}

class Train: public Transport{
private:
    bool local_transport;
    bool one_way;
    float get_co2_pkm();
public:
    Train();
    Train(bool initLocaltrans, bool initWay, float initKm);
    virtual ~Train();
    float get_co2();
    void print();
};

Train::Train()
{
    int temp1,temp2;
    cout << "(1) local transport" << endl << "(2) long distance" << endl;
    cout << "your choice: ";
    cin >> temp1;
    local_transport=temp1-1;
    cout << "(1) one-way" << endl << "(2) return" << endl;
    cout << "your choice: ";
    cin >> temp2;
    one_way=temp2-1;
}

Train::~Train()
{
    cout << "destructor Train: ";
    print();
}

Train::Train(bool initLocaltrans, bool initWay, float initKm):
    local_transport(initLocaltrans),
    one_way(initWay),
    Transport(initKm)
    {}

float Train::get_co2_pkm()
{
    if(local_transport==true)
        return 0.035;
    else
        return 0.085;
}

float Train::get_co2()
{
    if(one_way==true)
        return Transport::get_co2()*2;
    else
        return Transport::get_co2();
}

void Train::print()
{
    if(local_transport)
        cout << "long distance";
    else
        cout << "local transport";
    if(one_way)
        cout << " return";
    else
        cout << " one way";
    Transport::print();
}

class Wind: public Power{
private:
    float get_co2_kWh();
public:
    Wind();
    virtual ~Wind();
    Wind(float initKWh);
    void print();
};

Wind::Wind(){}

Wind::Wind(float initKWh):
    Power(initKWh)
    {}

Wind::~Wind()
{
    cout << "destructor Wind: ";
    print();
}

float Wind::get_co2_kWh()
{
    return 0.086;
}

void Wind::print()
{
    cout << "wind power ";
    Power::print();
}

class Coal: public Power{
private:
    bool anthracite;
    float get_co2_kWh();
public:
    Coal();
    virtual ~Coal();
    Coal(bool initAnthra, float initKWh);
    void print();
};

Coal::Coal()
{
    char temp;
    cout << "\ncoal power data input:" << endl << "type of coal:" << endl;
    cout << "(a) anthracite coal" << endl << "(l) lignite" << endl;
    cout << "your choice: ";
    cin >> temp;
    if(temp=='a')
        anthracite=true;
    else if(temp=='l')
        anthracite=false;
    else{
        cerr << "Wrong choice!";
        exit(-1);}
}

Coal::Coal(bool initAnthra, float initKWh):
    anthracite(initAnthra),
    Power(initKWh)
    {}

Coal::~Coal()
{
    cout << "destructor Coal: ";
    print();
}

float Coal::get_co2_kWh()
{
    if(anthracite==true)
        return 0.798;
    else
        return 1.150;
}

void Coal::print()
{
    if(anthracite==true){
        cout << "anthracite coal ";
        Power::print();}
    else{
        cout << "lignite ";
        Power::print();}
}

class CO2Footprint{
private:
    string name;
    CO2Emission* emissions;
public:
    CO2Footprint(string initName);
    ~CO2Footprint();
    string get_name();
    void add(CO2Emission* ptr);
    float get_sum_co2();
    void print();
};

CO2Footprint::CO2Footprint(string initName):
    name(initName),emissions(nullptr){}

CO2Footprint::~CO2Footprint()
{
    cout << "\ndestructor for CO2Footprint: for " << name << " started..." <<endl<<endl;
    CO2Emission* temp=nullptr;
    while(emissions!=nullptr){
        temp=emissions->get_next();
        delete emissions;
        emissions=temp;}
    emissions=nullptr;
    cout << "destructor CO2Footprint done";
}

string CO2Footprint::get_name()
{
    return name;
}

void CO2Footprint::add(CO2Emission* ptr)
{
    CO2Emission* newco2= emissions;
    emissions=ptr;
    ptr->set_next(newco2);
}

float CO2Footprint::get_sum_co2()
{
    float sum=0.0;
    CO2Emission* temp=emissions;
    while(temp!=nullptr){
        sum+=temp->get_co2();
        temp=temp->get_next();}
    return sum;
}

void CO2Footprint::print()
{
    cout<< "CO2 FOOTPRINT FOR: " << name <<endl;
    CO2Emission* temp=emissions;
    while(temp!=nullptr){
        temp->print();
        temp=temp->get_next();
    }
       cout << "TOTAL CO2 EMISSIONS: " << fixed << setprecision(1) << get_sum_co2() << " kg = " << fixed << setprecision(1) << get_sum_co2()/1000 << " tons" <<endl;
}

int main()
{
    string name;
    char select;
    cout << "CO2 Footprint for? ";
    cin >> name;
    CO2Footprint f(name);
    Bus* b2= new Bus(0,2200);
    Car* c2= new Car(7.5,5500);
    Cruise* h2= new Cruise(14,3);
    Flight* f2= new Flight(2669.9);
    Train* t2= new Train(1,1,628.8);
    Wind* w2 = new Wind(2000);
    Coal* k2 = new Coal(1,2000);
    f.add(b2);
    f.add(c2);
    f.add(h2);
    f.add(f2);
    f.add(t2);
    f.add(w2);
    f.add(k2);
    do{
        cout << "\nCO2 footprint input for " << f.get_name() << endl;
        cout << "(e) end" <<endl;
        cout << "(b) bus transport" <<endl;
        cout << "(c) car transport" <<endl;
        cout << "(f) flight tranport" <<endl;
        cout << "(h) holiday cruise" <<endl;
        cout << "(t) train transport" <<endl;
        cout << "(w) power from wind" <<endl;
        cout << "(k) power from coal" <<endl;
        cout << "(p) print all" <<endl;
        cout << ">>";
        cin >> select;
        switch(select){
        case 'b':
        {
            Bus* b1 = new Bus();
            f.add(b1);
            break;
        }
        case 'c':
        {
            Car* c1 = new Car();
            f.add(c1);
            break;
        }
        case 'f':
        {
            Flight* f1 = new Flight();
            f.add(f1);
            break;
        }
        case 'h':
        {
            Cruise* h1 = new Cruise();
            f.add(h1);
            break;
        }
        case 't':
        {
            Train* t1 = new Train();
            f.add(t1);
            break;
        }
        case 'w':
        {
            Wind* w1 = new Wind();
           f.add(w1);
           break;
        }
        case 'k':
        {
            Coal* k1 = new Coal();
            f.add(k1);
            break;
        }
        case 'p':
        {
            f.print();
            f.get_sum_co2();
            break;
        }
        case 'e':
            return 0;
            break;
        default:
            cerr << "Incorrect choice!";
            exit(-1);}
    }
    while(select!='e');
    return 0;
}
