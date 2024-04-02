#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "SchedulerFCFS.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

SchedulerFCFS::SchedulerFCFS(EventPriorityQueue *eventQueue) : Scheduler<ProcessQueue>(eventQueue) {
}

SchedulerFCFS::~SchedulerFCFS() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.front();
        waitQueue.pop();
        delete topProc;
    }
}

void SchedulerFCFS::schedule() {
       if (!waitQueue.empty()) {
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.front();
                if ( waitProc->request <= totalCPUNum ) {
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
