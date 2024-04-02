#include<iostream>
#include<iomanip>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "Adaptive_down.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

Adaptive_down::Adaptive_down(EventPriorityQueue *eventQueue) : Scheduler<ProcessQueue>(eventQueue) {
}

Adaptive_down::~Adaptive_down() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.front();
        waitQueue.pop();
        delete topProc;
    }
}

void Adaptive_down::schedule() {
    if (!waitQueue.empty()) {
            while ( !waitQueue.empty() && totalCPUNum > 0 ) {
                    Process *waitProc = waitQueue.front();
                    waitQueue.pop();
              //  cout << "Process " << waitProc->procId << " back to process queue" << endl;
                    Process *find = findProcess(waitProc->procId);
                    if ( totalCPUNum < find->request ) {
                      //  cout << "Process " << waitProc->procId << endl << "Request = " << find->request << " > available = " << totalCPUNum << endl << endl;
                        waitProc->execTime = waitProc->execTime * find->request / totalCPUNum;
                        find->request = totalCPUNum;
                    }
                    
                    find->StartTime = waitProc->StartTime = system_clock;
                    Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                    eventQueue->push(runEvent);
                    totalCPUNum -= find->request;
                /*
                cout << "Process: " << waitProc->procId <<
                endl << "Using cpu number: " << find->request <<
                endl << "Remaining cpu number: " << totalCPUNum <<
                endl << "Start time: " << waitProc->StartTime << endl << endl;
                */
                }
    }
}
