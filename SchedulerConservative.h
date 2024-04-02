#pragma once
#include "Scheduler.h"
using namespace std;

class SchedulerConservative : public Scheduler<order> {

	public:
        bool isCPUIdle;
        bool first_begin;
        bool sucess;
        bool back_sucess;
        int usage;
        int anchor_point;
        vector<Process*> runTable;
        order runningQueue;
        order estimateQueue;
		SchedulerConservative(EventPriorityQueue *eventQueue);
		virtual ~SchedulerConservative();
        void schedule();
        void back();
};
