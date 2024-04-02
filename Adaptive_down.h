#pragma once
#include "Scheduler.h"
using namespace std;

class Adaptive_down : public Scheduler<ProcessQueue> {

    public:

        Adaptive_down(EventPriorityQueue *eventQueue);
        virtual ~Adaptive_down();
        void schedule();
};
