#pragma once
#include "Scheduler.h"
using namespace std;

class SchedulerFCFS : public Scheduler<ProcessQueue> {

	public:

		SchedulerFCFS(EventPriorityQueue *eventQueue);
		virtual ~SchedulerFCFS();
		void schedule();
};
