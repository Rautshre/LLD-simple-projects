#include<bits/stdc++.h>
using namespace std;

struct Location{
    double x, y;
    Location(double _x=0, double _y=0):x(_x), y(_y){}
};

double distance(const Location& a, const Location& b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

class IPayment {
    public:
        virtual void pay(double amount) = 0;
        virtual ~IPayment(){}
};

class CashPayment: public IPayment{
    public:
        void pay(double amount) override{
            cout<<"pay of Rs "<<amount<<" by Cash"<<endl; 
        }
};

class UpiPayment: public IPayment{
    public:
        void pay(double amount)override{
            cout<<"payement via UPI of "<<amount<<" Rs"<<endl;
        }
};

class IObserver {
    public:
        virtual void update(const string& msg)=0;
        virtual ~IObserver(){}
};

class CustomerNotifier: public IObserver{
    string name;
    public:
        CustomerNotifier(string n): name(n){}
            void update(const string& msg) override{
                cout<<"Notification to customer "<<name<<": "<<msg<<endl;
            }
};

class CaptainNotifier: public IObserver{
    string name;
    public:
        CaptainNotifier(string n): name(n){}
        void update(const string& msg) override{
            cout<<" Notifier to caption "<<name<<": "<<msg<<endl;
        }
};

class NotificationService {
    vector<IObserver*>observers;
    public:
        void addObserver(IObserver* obs){
            observers.push_back(obs);
        }
        void notifyAll(const string& msg){
            for(auto obs: observers) obs->update(msg);
        }
};

class User{
    protected:
        string name;
        Location loc;
    public:
        User(string n, Location l): name(n), loc(l) {}
        virtual ~User(){}
        string getName(){
            return name;
        }
        Location getLocation(){
            return loc;
        }
};

class Customer: public User{
    public:
        Customer(string n, Location l): User(n, l){}

};

class Captain : public User{
    public:
        Captain(string n, Location l): User(n, l){}
};

class Ride{
    Customer* customer;
    Captain* captain;
    double fare;
    IPayment* paymentMethod;
    NotificationService notifier;
    public:
        Ride(Customer* c, Captain* cap, double f): customer(c), captain(cap), fare(f), paymentMethod(nullptr){
            notifier.addObserver(new CustomerNotifier(c->getName()));
            notifier.addObserver(new CaptainNotifier(cap->getName()));
        }
        void choosePayment(IPayment* p){
            paymentMethod = p;
        }
        void startRide(){
            notifier.notifyAll("Ride Started!");
        }
        void notifyBooking() {
        notifier.notifyAll("Ride has been booked! Captain will arrive soon.");
        }
        void endRide(){
            notifier.notifyAll("Ride Ended ! Please Make payment.");
            if(paymentMethod)paymentMethod->pay(fare);
            else cout<<"No payment method selected! "<<endl;
        }
};

class RideMatchingService {
    public:
    Captain* findNearestCaptain(Customer* customer, vector<Captain*>& captains){
        Captain* nearest = nullptr;
        double minDist = DBL_MAX;
        for(auto cap: captains){
            double d = distance(customer->getLocation(), cap->getLocation());
            if(d<minDist){
                minDist =d;
                nearest = cap;
            }
        }
        return nearest;
    }
};


int main(){
    Customer* c = new Customer("shreyas", Location(0, 0));
    vector<Captain*>captains = {
        new Captain("ravi", Location(2, 3)),
        new Captain("amit", Location(0, 1)),
        new Captain("Karan", Location(1, 1))
    };
    RideMatchingService rms;
    Captain* nearest = rms.findNearestCaptain(c, captains);

    if(nearest){
        cout<<"Nearest Captian found: "<<nearest->getName()<<endl;
        double fare = distance(c->getLocation(), nearest->getLocation())*20;
        Ride* ride = new Ride(c, nearest, fare);

        ride->choosePayment(new UpiPayment());
        ride->startRide();
        ride->endRide();
        delete ride;
    }
    else{
        cout<<"No captains avalible "<<endl;
    }
    delete c;
    for(auto cap: captains) delete cap;
    return 0;
}