#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "SchedulerLargestFirst.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;
int id = 0;

SchedulerLargestFirst::SchedulerLargestFirst(EventPriorityQueue *eventQueue) : Scheduler<order>(eventQueue) {
    largest = 0;
    isCPUIdle = true;
}

SchedulerLargestFirst::~SchedulerLargestFirst() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.top();
        waitQueue.pop();
        delete topProc;
    }
}

void SchedulerLargestFirst::schedule() {
    if (!waitQueue.empty()) {
        for (int i = 0; i < BacktoQueue.size(); i++) {
            if ( id == BacktoQueue[i]->procId ) {
                cout << "Process " << BacktoQueue[i]->procId << " back to process queue" << endl;
                Process *find = findProcess(BacktoQueue[i]->procId);
                find->StartTime = BacktoQueue[i]->StartTime = system_clock;
                Event *runEvent = new Event(Event::CPU_COMPLETION, BacktoQueue[i]->procId, BacktoQueue[i]->StartTime + BacktoQueue[i]->execTime);
                eventQueue->push(runEvent);
                totalCPUNum -= find->request;
                cout << "Process: " << BacktoQueue[i]->procId <<
                endl << "Using cpu number: " << find->request <<
                endl << "Remaining cpu number: " << totalCPUNum <<
                endl << "Start time: " << BacktoQueue[i]->StartTime << endl << endl;
                isCPUIdle = true;
            }
            else{
                waitQueue.push(BacktoQueue[i]);
            }
        }
        BacktoQueue.clear();
        largest = 0;
        
            while (!waitQueue.empty() ) {
                Process *waitProc = waitQueue.top();
                cout << "First: " << waitProc->procId << endl << endl;
                if ( isCPUIdle && (waitProc->getArrivalTime() >= system_clock) && (totalCPUNum >= waitProc->request) ) {
                    cout << "Process " << waitProc->procId << " back to process queue" << endl;
                    waitQueue.pop();
                    Process *find = findProcess(waitProc->procId);
                    find->StartTime = waitProc->StartTime = system_clock;
                    Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                    eventQueue->push(runEvent);
                    totalCPUNum -= find->request;
                    cout << "Process: " << waitProc->procId <<
                    endl << "Using cpu number: " << find->request <<
                    endl << "Remaining cpu number: " << totalCPUNum <<
                    endl << "Submit time: " << waitProc->getArrivalTime() <<
                    endl << "Start time: " << waitProc->StartTime << endl << endl;
                }
                else if ( (waitProc->getArrivalTime() < system_clock) && (totalCPUNum >= waitProc->request) ) {
                    waitQueue.pop();
                    isCPUIdle = false;
                    if ( waitProc->request > largest ) {
                        largest = waitProc->request;
                        id = waitProc->procId;
                    }
                    BacktoQueue.push_back(waitProc);
            }
            else{
                break;
            }
        }
    }
}
