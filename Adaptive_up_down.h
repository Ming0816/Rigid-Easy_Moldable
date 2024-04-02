#pragma once
#include "Scheduler.h"
using namespace std;

class Adaptive_up_down : public Scheduler<ProcessQueue> {

    public:

        Adaptive_up_down(EventPriorityQueue *eventQueue);
        virtual ~Adaptive_up_down();
        void schedule();
};
