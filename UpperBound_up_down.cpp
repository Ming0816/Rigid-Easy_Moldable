#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "UpperBound_up_down.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

UpperBound_up_down::UpperBound_up_down(EventPriorityQueue *eventQueue) : Scheduler<ProcessQueue>(eventQueue) {
    parall_limit = 16;
}

UpperBound_up_down::~UpperBound_up_down() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.front();
        waitQueue.pop();
        delete topProc;
    }
}

void UpperBound_up_down::schedule() {
       if (!waitQueue.empty()) {
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.front();
                Process *find = findProcess(waitProc->procId);
                if ( waitProc->request > parall_limit ) {
                    waitProc->execTime = waitProc->execTime * waitProc->request / parall_limit;
                    waitProc->request = parall_limit;
                }
              
                if ( totalCPUNum > 0 ) {
                    waitQueue.pop();
                    if ( totalCPUNum > parall_limit ) {
                        waitProc->execTime = (waitProc->execTime * waitProc->request) / parall_limit;
                        find->request = parall_limit;
                    }else{
                        waitProc->execTime = (waitProc->execTime * waitProc->request) / totalCPUNum;
                        find->request = totalCPUNum;
                    }
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
