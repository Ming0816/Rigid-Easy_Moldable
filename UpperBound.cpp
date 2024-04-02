#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "UpperBound.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

UpperBound::UpperBound(EventPriorityQueue *eventQueue) : Scheduler<ProcessQueue>(eventQueue) {
    parall_limit = 16;
}

UpperBound::~UpperBound() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.front();
        waitQueue.pop();
        delete topProc;
    }
}

void UpperBound::schedule() {
       if (!waitQueue.empty()) {
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.front();
                Process *find = findProcess(waitProc->procId);
                if ( waitProc->request > parall_limit ) {
                    waitProc->execTime = waitProc->execTime * waitProc->request / parall_limit;
                    find->request = waitProc->request = parall_limit;
                }
              
                if ( totalCPUNum >= find->request ) {
                    waitQueue.pop();
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
