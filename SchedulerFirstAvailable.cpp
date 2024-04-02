#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "SchedulerFirstAvailable.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

SchedulerFirstAvailable::SchedulerFirstAvailable(EventPriorityQueue *eventQueue) : Scheduler<order>(eventQueue) {
}

SchedulerFirstAvailable::~SchedulerFirstAvailable() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.top();
        waitQueue.pop();
        delete topProc;
    }
}

void SchedulerFirstAvailable::schedule() {
       if (!waitQueue.empty()) {
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.top();
                waitQueue.pop();
                
                if ( waitProc->request <= totalCPUNum) {
                    Process *find = findProcess(waitProc->procId);
                    find->StartTime = waitProc->StartTime = system_clock;
                    Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                    eventQueue->push(runEvent);
                    totalCPUNum -= find->request;
                }
                else{
                    BacktoQueue.push_back(waitProc);
                }
            }
            for (int i = 0; i < BacktoQueue.size(); i++) {
                waitQueue.push(BacktoQueue[i]);
            }
            BacktoQueue.clear();
       }
}
