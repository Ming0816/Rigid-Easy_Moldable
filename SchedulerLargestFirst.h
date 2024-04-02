#pragma once
#include "Scheduler.h"
using namespace std;

class SchedulerLargestFirst : public Scheduler<order> {

	public:

        int largest;
        bool isCPUIdle;
        vector<Process*> BacktoQueue;
		SchedulerLargestFirst(EventPriorityQueue *eventQueue);
		virtual ~SchedulerLargestFirst();
		void schedule();
};
