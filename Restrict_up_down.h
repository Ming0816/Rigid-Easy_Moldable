#pragma once
#include "Scheduler.h"
using namespace std;

class Restrict_up_down : public Scheduler<ProcessQueue> {

    public:

        Restrict_up_down(EventPriorityQueue *eventQueue);
        virtual ~Restrict_up_down();
        void schedule();
};
