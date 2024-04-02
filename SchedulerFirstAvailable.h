#pragma once
#include "Scheduler.h"
using namespace std;

class SchedulerFirstAvailable : public Scheduler<order> {

	public:
        order LargeQueue;
        vector<Process*> BacktoQueue;
		SchedulerFirstAvailable(EventPriorityQueue *eventQueue);
		virtual ~SchedulerFirstAvailable();
		void schedule();
};
