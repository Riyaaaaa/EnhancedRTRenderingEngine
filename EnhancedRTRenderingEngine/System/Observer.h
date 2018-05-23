#pragma once

#include <vector>

namespace ERTRESystem {
    template<class Event>
    class Observer {
    public:
        virtual void Notify(Event e) = 0;
    };

    template<class Event>
    class Subject {
    public:
        void AddObserver(Observer<Event>* observer) {
            observers.push_back(observer);
        }

        void RemoveObserver(Observer<Event>* observer) {
            auto it = std::find(observers.begin(), observers.end(), observer);
            if (it != observers.end()) {
                observers.erase(it);
            }
        }

    protected:
        void Notify(Event e) {
            for (auto &&  observer: observers) {
                observer->Notify(e);
            }
        }

        std::vector<Observer<Event>*> observers;
    };
}