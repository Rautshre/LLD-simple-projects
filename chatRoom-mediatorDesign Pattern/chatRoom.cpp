#include<bits/stdc++.h>

using namespace std;

class Colleague;
class Imediator{
public: 
    virtual ~Imediator(){}
    virtual void registerColleague(Colleague* c) = 0;
    virtual void send(const string& from, const string& msg)=0;
    virtual void sendPrivate(const string& from, const string& to, const string& msg)=0;
};
class Colleague{
    protected:
            Imediator* mediator;
    public:
       Colleague(Imediator* m){
             mediator = m;
             mediator->registerColleague(this);
       }        
       virtual ~Colleague(){}
       virtual string getName() = 0;
       virtual void send(const string& msg)=0;
       virtual void sendPrivate(const string& to, const string& msg)=0;
       virtual void receive(const string& from, const string& msg) = 0;
};

class ChatMediator: public Imediator {
    private:
    vector<Colleague*>colleagues;
    vector<pair<string, string>>mutes;

    public:
    void registerColleague(Colleague* c)override{
        colleagues.push_back(c);
    }
    void mute(const string& who, const string& whom){
        mutes.push_back(make_pair(who, whom));
    }
    void send(const string& from, const string& msg)override{
        cout<<"[ "<<from<<" broadcasts]: "<<msg<<" \n";
        for(Colleague* c: colleagues){
            if(c->getName()==from) continue;
            bool ismuted = false;

            for(auto &p: mutes){
                if(from==p.second && c->getName()==p.first){
                    ismuted = true;
                    break;
                }
            }
            if(!ismuted){
                c->receive(from, msg);
            }
        }
    }
    void sendPrivate(const string& from, const string& to, const string& msg)override{
        cout<<"[ "<<from<<"-> "<<to<<" ]: "<<msg<<"\n";
        for(Colleague* c: colleagues){
            if(c->getName()==to){
                for(auto &p: mutes){
                    if(from==p.second && to==p.first){
                        cout<<"\n[Message is muted]\n";
                        return;
                    }
                }
                c->receive(from, msg);
                return;
            }
        }
        cout<<" [Mediator]User \""<<to<<"\"not found]\n";
    }
};

class User: public Colleague{
    private:
        string name;
    public:
        User(const string& n, Imediator* m): Colleague(m){
            name = n;
        }    
        string getName() override {
            return name;
        }
        void send(const string& msg)override{
            mediator->send(name, msg);
        }
        void sendPrivate(const string& to, const string& msg) override {
            mediator->sendPrivate(name, to, msg);
        }
        void receive(const string& from, const string &msg) override{
            cout<<"      "<<name<<" got from "<<from<<": "<<msg<<" \n";
        }
};

int main(){
    ChatMediator* chatRoom = new ChatMediator();

    User* user1 = new User("shreyas", chatRoom);
    User* user2 = new User("Neha", chatRoom);
    User* user3 = new User("rajeev", chatRoom);

    chatRoom->mute("shreyas", "rajeev");
    user1->send("Hello Everyone !!");
    user2->sendPrivate("shreyas", "Hey Shreyas!");
    user3->sendPrivate("shreyas", "buddy unblock me plz!");

    delete user1;
    delete user2;
    delete user3;
    delete chatRoom;
    return 0;
}