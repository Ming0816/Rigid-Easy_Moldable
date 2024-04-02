#pragma once
#include "Scheduler.h"
using namespace std;

class UpperBound_up_down : public Scheduler<ProcessQueue> {

	public:
        int parall_limit;
		UpperBound_up_down(EventPriorityQueue *eventQueue);
		virtual ~UpperBound_up_down();
		void schedule();
};
