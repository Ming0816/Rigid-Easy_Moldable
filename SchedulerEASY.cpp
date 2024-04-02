#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "SchedulerEASY.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

SchedulerEASY::SchedulerEASY(EventPriorityQueue *eventQueue) : Scheduler<order>(eventQueue) {
    this->first_begin = true;
    this->shadow_time = 0;
    this->collect = 0;
    this->extra_node = 0;
}

SchedulerEASY::~SchedulerEASY() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.top();
        waitQueue.pop();
        delete topProc;
    }
}

void SchedulerEASY::schedule() {
    int i, terminal;
    
       if (!waitQueue.empty()) {
            while (!waitQueue.empty()) {
                Process *waitProc = waitQueue.top();
                waitQueue.pop();
                
                if ( system_clock == shadow_time ) {
                     first_begin = true;
                }
                terminal = system_clock + waitProc->execTime;
                if ( waitProc->request <= totalCPUNum && first_begin ) {
                    cout << "Process " << waitProc->procId << " back to process queue" << endl;
                        Process *find = findProcess(waitProc->procId);
                        find->StartTime = waitProc->StartTime = system_clock;
                        Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                        eventQueue->push(runEvent);
                        Process *running = new Process(waitProc->procId, waitProc->StartTime + waitProc->execTime, waitProc->execTime, waitProc->request);
                        runningQueue.push(running);
                        totalCPUNum -= find->request;
                    cout << "Process: " << waitProc->procId <<
                    endl << "Using cpu number: " << find->request <<
                    endl << "Remaining cpu number: " << totalCPUNum <<
                    endl << "Start time: " << waitProc->StartTime << endl << endl;
                }
                else if ( waitProc->request > totalCPUNum && first_begin ) {
                        first_begin = false;
                        collect = totalCPUNum;
                    while (!runningQueue.empty()) {
                        Process *runProc = runningQueue.top();
                        runningQueue.pop();
                        Process *find = findProcess(runProc->procId);
                        if ( find->status != Process::TERMINATED ) {
                        collect += runProc->request;
                        runTable.push_back(runProc);
                        cout << "Running process " << runProc->procId << " Released resources " << runProc->request << " Collected " << collect << " Terminated time " << runProc->getArrivalTime() << endl;
                        if ( collect >= waitProc->request ) {
                            shadow_time = runProc->getArrivalTime();
                            extra_node = collect - waitProc->request;
                            break;
                        }
                    }
                }
                    cout << "Process " << waitProc->procId << ":" << endl <<
                            "Collected node: " << collect << endl <<
                            "Resource need: " << waitProc->request << endl <<
                            "Extra node: " << extra_node << endl <<
                            "Shadow time: " << shadow_time << endl << endl;
                    BacktoQueue.push_back(waitProc);
                    break;
                }
                else if ( (waitProc->request <= totalCPUNum) && ((terminal <= shadow_time) || ((terminal > shadow_time) && (waitProc->request <= extra_node))) ) {
                    cout << "Process " << waitProc->procId <<
                    " was backfilled to process queue" << endl << endl;
                        Process *find = findProcess(waitProc->procId);
                        find->StartTime = waitProc->StartTime = system_clock;
                        
                        Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                        eventQueue->push(runEvent);
                    
                        Process *running = new Process(waitProc->procId, waitProc->StartTime + waitProc->execTime, waitProc->execTime, waitProc->request);
                        runTable.push_back(running);
            
                    if ( terminal > shadow_time ) {
                        cout << "Process " << waitProc->procId <<
                        " Terminated time = " << waitProc->StartTime + waitProc->execTime << " > Shadow time = " << shadow_time << endl <<
                        "Resource need = " << waitProc->request <<
                        " <= Extra node = " << extra_node << endl;
                        extra_node -= find->request;
                        cout << "Remain extra node: " << extra_node << endl << endl;
                    }
                    totalCPUNum -= find->request;
                    cout << "Process: " << waitProc->procId <<
                    endl << "Using cpu number: " << find->request <<
                    endl << "Remaining cpu number: " << totalCPUNum <<
                    endl << "Start time: " << waitProc->StartTime << endl << endl;
                }
                else{
                    BacktoQueue.push_back(waitProc);
                }
            }
            
            for (i = 0; i < BacktoQueue.size(); i++) {
                waitQueue.push(BacktoQueue[i]);
            }
            BacktoQueue.clear();
            
            for ( i = 0; i < runTable.size(); i++) {
                runningQueue.push(runTable[i]);
            }
            runTable.clear();
       }
}
