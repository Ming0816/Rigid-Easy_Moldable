#pragma once
#include "Scheduler.h"
using namespace std;

class SchedulerSmallestFirst : public Scheduler<order> {

	public:

        int smallest;
		SchedulerSmallestFirst(EventPriorityQueue *eventQueue);
		virtual ~SchedulerSmallestFirst();
		void schedule();

};
