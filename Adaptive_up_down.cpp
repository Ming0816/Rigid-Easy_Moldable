#include<iostream>
#include<iomanip>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "Adaptive_up_down.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

Adaptive_up_down::Adaptive_up_down(EventPriorityQueue *eventQueue) : Scheduler<ProcessQueue>(eventQueue) {
}

Adaptive_up_down::~Adaptive_up_down() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.front();
        waitQueue.pop();
        delete topProc;
    }
}

void Adaptive_up_down::schedule() {
       if (!waitQueue.empty()) {
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.front();
                if ( totalCPUNum > 0 ) {
                    waitQueue.pop();
                    Process *find = findProcess(waitProc->procId);
                    waitProc->execTime = (waitProc->execTime * find->request) / totalCPUNum;
                    find->request = totalCPUNum;
                    totalCPUNum -= find->request;
                    find->StartTime = waitProc->StartTime = system_clock;
                    Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                    eventQueue->push(runEvent);
                }
                else{
                    break;
                }
            }
       }
}
