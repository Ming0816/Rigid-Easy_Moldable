#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Event.h"
#include "Rigid_job.h"
#include "Adaptive_down.h"
#include "Adaptive_up_down.h"
#include "Restrict_up_down.h"
#include "UpperBound.h"
#include "UpperBound_up_down.h"
#include "SchedulerPointer.h"
using namespace std;

int totalCPUNum;
int system_clock;
vector<Process*> procTable;

SchedulerPointer* Rigid(EventPriorityQueue *eventQueue, int schedulerChoice) {
    switch (schedulerChoice) {
        case 1:
            cout << "You chose FCFS." << endl;
            return new SchedulerFCFS(eventQueue);
        case 2:
            cout << "You chose First-available." << endl;
            return new SchedulerFirstAvailable(eventQueue);
        case 3:
            cout << "You chose Smallest-request-first." << endl;
            return new SchedulerSmallestFirst(eventQueue);
        case 4:
            cout << "You chose Largest-request-first." << endl;
            return new SchedulerLargestFirst(eventQueue);
        case 5:
            cout << "You chose Easy-backfilling." << endl;
            return new SchedulerEASY(eventQueue);
        case 6:
            cout << "You chose Conservative-backfilling." << endl;
            return new SchedulerConservative(eventQueue);
        default:
            cout << "Invalid scheduler. Aborting." << endl;
            exit(1);
    }
}

SchedulerPointer* Moldable(EventPriorityQueue *eventQueue, int schedulerChoice) {
    switch(schedulerChoice) {
        case 1:
        cout << "You chose Adaptive-scale-down." << endl;
        return new Adaptive_down(eventQueue);
        case 2:
        cout << "You chose Adaptive-scale-up-down." << endl;
        return new Adaptive_up_down(eventQueue);
        case 3:
        cout << "You chose Restrict-up-down." << endl;
        return new Restrict_up_down(eventQueue);
        case 4:
        cout << "You chose UpperBound." << endl;
        return new UpperBound(eventQueue);
        case 5:
        cout << "You chose UpperBound-up-down." << endl;
        return new UpperBound_up_down(eventQueue);
        default:
        cout << "Invalid scheduler. Aborting." << endl;
        exit(1);
    }
}

void deallocateEventQueue(EventPriorityQueue *eventQueue) {
	while (!eventQueue->empty()) {
		Event *topEvent = eventQueue->top();
		eventQueue->pop();
		delete topEvent;
	}
	delete eventQueue;
}

int main() {
    ifstream read("l_sdsc_sp2.swf.extracted", ios::in);
    string ch;
    int scheduleChoice;
    int load_factor = 1;
    totalCPUNum = 384;
    int count;
    int jobCount = 0;
    int mode;
    int n[17];
    EventPriorityQueue *eventQueue = new EventPriorityQueue();
    SchedulerPointer *scheduler;
    
    while ( getline(read,ch) ) {
        string a = "";
        count = 0;
        
            for (int i = 0; i < 17; i++) {
        while ( ch[count] == ' ' || ch[count] == '\t' || ch[count] == '\n' )
        count++;
        while ( ch[count] != ' ' && ch[count] != '\t' && ch[count] != '\n') {
                a += ch[count++];
            }
            n[i] = stoi(a);
            a = "";
            }
            if ( n[10] == 1 ) {
            Event *arrivalEvent = new Event(Event::PROCESS_ARRIVAL, jobCount, n[1]);
            eventQueue->push(arrivalEvent);
            Process *newProc = new Process(jobCount, n[1], n[3]*load_factor, n[7]);
            procTable.push_back(newProc);
            jobCount++;
            }
    }
    cout << endl <<
    "Choose the scheduler mode: " << endl <<
    "1. Rigid" << endl <<
    "2. Moldable" << endl;
    cin >> mode;
    
    if ( mode == 1 ) {
        cout << endl << "Total cpu number: " << totalCPUNum
             << endl << "Load factor: " << load_factor << endl << endl
             << "Available schedulers: " << endl
             << "1. First-Come-First-Serve (FCFS)" << endl
             << "2. First-available" << endl
             << "3. Smallest-request-first" << endl
             << "4. Largest-request-first" << endl
             << "5. Easy-backfilling" << endl
             << "6. Conservative-backfilling" << endl
             << "Choose a scheduler's number: " << endl;
        cin >> scheduleChoice;
        scheduler = Rigid(eventQueue, scheduleChoice);
    }
    else if ( mode == 2 ) {
        cout << endl << "Total cpu number: " << totalCPUNum
             << endl << "Load factor: " << load_factor << endl << endl
             << "Available schedulers: " << endl
             << "1. Adaptive-scale-down" << endl
             << "2. Adaptive-scale-up-down" << endl
             << "3. Restrict-up-down" << endl
             << "4. UpperBound" << endl
             << "5. UpperBound-up-down" << endl
             << "Choose a scheduler's number: " << endl;
        cin >> scheduleChoice;
        scheduler = Moldable(eventQueue, scheduleChoice);
    }
    else{
        cout << "Invalid input." << endl;
        exit(1);
    }
    cout << "Scheduling: " << endl << endl;
    
    while (!eventQueue->empty()) {
        Event *nextEvent = eventQueue->top();
        eventQueue->pop();
        scheduler->handleEvent(nextEvent);
        delete nextEvent;
    }
    cout << "Scheduler finished..." << endl;
    scheduler->printStats();
	deallocateEventQueue(eventQueue);
	return 0;
}
