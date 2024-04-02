#pragma once
#include "Scheduler.h"
using namespace std;

class SchedulerEASY : public Scheduler<order> {

	public:
        int shadow_time;
        int collect;
        int extra_node;
        bool first_begin;
        vector<Process*> BacktoQueue;
        vector<Process*> runTable;
        order runningQueue;
		SchedulerEASY(EventPriorityQueue *eventQueue);
		virtual ~SchedulerEASY();
        void schedule();
};
