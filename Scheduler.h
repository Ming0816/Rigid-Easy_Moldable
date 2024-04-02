#pragma once
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <queue>
#include <string>
#include <iomanip>
#include "Process.h"
#include "Event.h"
#include "SchedulerPointer.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

typedef queue<Process*> ProcessQueue;
typedef priority_queue<Process*, deque<Process*>, FCFS> order;
typedef priority_queue<Event*, deque<Event*>, EventComparator> EventPriorityQueue;

template <typename WaitingQueue>
class Scheduler : public SchedulerPointer {
    protected:
		EventPriorityQueue *eventQueue;
        WaitingQueue waitQueue;
    
	public:
		Scheduler(EventPriorityQueue *eventQueue){
            this->eventQueue = eventQueue;
        };
    
		virtual ~Scheduler(){
            for (int i = 0; i < procTable.size(); i++) {
                delete procTable[i];
            }
            procTable.clear();
            eventQueue = NULL;
        };


		void handleProcArrival(Event *arrivalEvent){
            Process *newProc = findProcess(arrivalEvent->procId);
            system_clock = arrivalEvent->time;
            newProc->status = Process::READY;
            waitQueue.push(newProc);
            schedule();
        };
    
        void handleCPUComplete(Event *completeEvent) {
            Process *find = findProcess(completeEvent->procId);
            system_clock = find->finishTime = completeEvent->time;
            find->status = Process::TERMINATED;
            totalCPUNum += find->request;
            
            cout << "Process " << find->procId << " completed..." <<
            "Terminated time: " << find->finishTime << endl <<
            "Released cpu number: " << find->request << endl <<
            "Available cpu number: " << totalCPUNum << endl << endl;

            schedule();
        };

     virtual void handleEvent(Event *event){
        switch (event->eventType) {
            case Event::PROCESS_ARRIVAL:
                handleProcArrival(event);
                break;
            case Event::CPU_COMPLETION:
                handleCPUComplete(event);
                break;
        }
    };

    virtual void schedule() = 0;

    Process* findProcess(int procID){
        for (int i = 0; i < procTable.size(); i++) {
            if (procTable[i]->procId == procID) {
                return procTable[i];
            }
        }
        return NULL;
    };
    

    void printStats(){

        double waitingTimeSum = 0;
        double serviceTimeSum = 0;
        double first_start = 0;
        double last_end = 0;
        
        cout << endl << "Information: " << endl << endl;
        cout << "Event:" << setw(18) << "Submit time:" << setw(17) << "Start time:" << setw(18) << "Finish time:" << setw(18) << "Waiting time:" << endl << endl;
        
        for (int i = 0; i < procTable.size(); i++) {
    
            if (procTable[i]->StartTime - procTable[i]->getArrivalTime() != 0) {
            cout << setw(6) << procTable[i]->procId << setw(17) << procTable[i]->getArrivalTime() << "s" << setw(16) << procTable[i]->StartTime << "s" << setw(17) << procTable[i]->finishTime << "s" << setw(17) << (procTable[i]->StartTime - procTable[i]->getArrivalTime()) << "s" << endl;
            }
            if ( i == 0 ) {
                first_start = procTable[i]->StartTime;
            }
            if ( i == procTable.size()-1 ) {
                last_end = procTable[i]->finishTime;
            }
            
            serviceTimeSum += procTable[i]->execTime;
            waitingTimeSum += (procTable[i]->StartTime - procTable[i]->getArrivalTime());
        }
        
        cout << "Turnaround time: " << last_end - first_start << endl <<
                "CPU Utilization: " << fixed << setprecision(3) <<
        (double)(100 * serviceTimeSum / ((last_end-first_start)*totalCPUNum)) << '%' << endl <<
        "Average waiting time: " << waitingTimeSum / procTable.size() << "s" << endl << endl;
    };
};
