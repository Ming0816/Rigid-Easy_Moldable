#pragma once
#include "Scheduler.h"
using namespace std;

class UpperBound : public Scheduler<ProcessQueue> {

	public:
        int parall_limit;
		UpperBound(EventPriorityQueue *eventQueue);
		virtual ~UpperBound();
		void schedule();
};
