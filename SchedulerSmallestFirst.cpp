#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "SchedulerSmallestFirst.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

SchedulerSmallestFirst::SchedulerSmallestFirst(EventPriorityQueue *eventQueue) : Scheduler<order>(eventQueue) {
}

SchedulerSmallestFirst::~SchedulerSmallestFirst() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.top();
        waitQueue.pop();
        delete topProc;
    }
}

void SchedulerSmallestFirst::schedule() {
       if (!waitQueue.empty()) {
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.top();
                if ( waitProc->request <= totalCPUNum) {
                    waitQueue.pop();
                    Process *find = findProcess(waitProc->procId);
                    find->StartTime = waitProc->StartTime = system_clock;
                    Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                    eventQueue->push(runEvent);
                    totalCPUNum -= find->request;
                }
                else{
                    break;
                }
            }
       }
}
