#include <iostream>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"
#include "SchedulerConservative.h"
using namespace std;

extern int totalCPUNum;
extern int system_clock;
extern vector<Process*> procTable;

SchedulerConservative::SchedulerConservative(EventPriorityQueue *eventQueue) : Scheduler<order>(eventQueue) {
    this->isCPUIdle = true;
    this->sucess = true;
    this->first_begin = true;
    this->back_sucess = true;
    this->usage = 0;
    this->anchor_point = 0;
}

SchedulerConservative::~SchedulerConservative() {
    while (!waitQueue.empty()) {
        Process *topProc = waitQueue.top();
        waitQueue.pop();
        delete topProc;
    }
}

void SchedulerConservative::back() {
    if (!estimateQueue.empty()) {
        while (!estimateQueue.empty()) {
              Process *Proc = estimateQueue.top();
              if ( system_clock == Proc->getArrivalTime() ) {
              first_begin = true;
              }
              else{
              first_begin = false;
              }
               if ( first_begin ) {
               estimateQueue.pop();
               cout << "Estimate process " << Proc->procId <<
               " back to process queue" << endl;
               Process *find = findProcess(Proc->procId);
               find->StartTime = Proc->StartTime = system_clock;
               Event *runEvent = new Event(Event::CPU_COMPLETION, Proc->procId, Proc->StartTime + Proc->execTime);
               eventQueue->push(runEvent);
               totalCPUNum -= find->request;
               cout << "Process: " << Proc->procId <<
               endl << "Using cpu number: " << find->request <<
               endl << "Remaining cpu number: " << totalCPUNum <<
               endl << "Start time: " << Proc->StartTime << endl << endl;
               }
               else{
                   break;
               }
        }
    }
}

void SchedulerConservative::schedule() {
           back();
           if (!waitQueue.empty()) {
                   while (!waitQueue.empty()) {
                   Process *waitProc = waitQueue.top();
                   waitQueue.pop();
                   
                if ( waitProc->request <= totalCPUNum && estimateQueue.empty() ) {
                    cout << "Process " << waitProc->procId << " back to process queue" << endl;
                    Process *find = findProcess(waitProc->procId);
                    find->StartTime = waitProc->StartTime = system_clock;
                    
                    Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                    eventQueue->push(runEvent);
                    
                    Process *running = new Process(waitProc->procId, waitProc->StartTime + waitProc->execTime, waitProc->execTime, waitProc->request);
                    running->status = Process::READY;
                    runTable.push_back(running);
                    totalCPUNum -= find->request;
                    cout << "Process: " << waitProc->procId <<
                    endl << "Using cpu number: " << find->request <<
                    endl << "Remaining cpu number: " << totalCPUNum <<
                    endl << "Start time: " << waitProc->StartTime << endl << endl;
                }
                else {
                   usage = totalCPUNum;
                   anchor_point = system_clock;
                   back_sucess = true;
                   while (1) {
                       sucess = true;
                       if ( totalCPUNum < waitProc->request ) {
                       isCPUIdle = true;
                       }else if ( totalCPUNum >= waitProc->request && back_sucess ) {
                       isCPUIdle = false;
                       }
                       else{
                       isCPUIdle = true;
                       }
                       while (!runningQueue.empty()) {
                           Process *runProc = runningQueue.top();
                           runningQueue.pop();
                           Process *find = findProcess(runProc->procId);
                           if ( (usage >= waitProc->request) && (runProc->getArrivalTime() > anchor_point) ) {
                               isCPUIdle = false;
                           }
 
                           if ( find->status != Process::TERMINATED ) {
                               if ( runProc->status == Process::READY ) {
                               runTable.push_back(runProc);
                                   if (isCPUIdle) {
                               cout << "Running process " << runProc->procId << " Released resources " << runProc->request << " Terminated time " << runProc->getArrivalTime() << endl;
                               anchor_point = runProc->getArrivalTime();
                               usage += find->request;
                                   cout << "Usage: " << usage << endl;
                                   }
                               }
                               else if ( runProc->status == Process::ESTIMATE && runProc->anchor_start >= system_clock ) {
                                   if ( (anchor_point + waitProc->execTime > runProc->anchor_start) && (runProc->remain_node < waitProc->request) ) {
                                       cout << "Process " << waitProc->procId <<
                                       " Terminated time = " << anchor_point + waitProc->execTime << " > Estimated process " << runProc->procId << " start time = " << runProc->anchor_start << endl <<
                                       "Resource need = " << waitProc->request <<
                                       " > Remain node = " << runProc->remain_node << endl << endl;
                                       usage = runProc->remain_node;
                                       sucess = false;
                                       runTable.push_back(runProc);
                                       break;
                                   }
                                   else if ( anchor_point + waitProc->execTime <= runProc->anchor_start ) {
                                       runTable.push_back(runProc);
                                       break;
                                   }
                                  else if ( (anchor_point + waitProc->execTime > runProc->anchor_start) && (runProc->remain_node >= waitProc->request) ) {
                                      cout << "Process " << waitProc->procId <<
                                      " Terminated time = " << anchor_point + waitProc->execTime << " > Estimated process " << runProc->procId <<" start time = " << runProc->anchor_start << endl <<
                                      "Resource need = " << waitProc->request <<
                                      " < Remain node = " << runProc->remain_node << endl;
                                      if ( anchor_point == runProc->anchor_start ) {
                                      usage = runProc->remain_node;
                                      }
                                      runProc->remain_node -= waitProc->request;
                                      cout << "Remain node: " << runProc->remain_node << endl << endl;
                                      runProc->reduce = -1;
                                      runTable.push_back(runProc);
                                  }
                               }
                           }
                       }
                       if ( !sucess && totalCPUNum >= waitProc->request ) {
                           back_sucess = false;
                       }
                       else if ( sucess && totalCPUNum >= waitProc->request && back_sucess ) {
                       cout << "Process " << waitProc->procId <<
                       " was backfilled to process queue" << endl << endl;
                       Process *find = findProcess(waitProc->procId);
                       find->StartTime = waitProc->StartTime = system_clock;
                       
                       Event *runEvent = new Event(Event::CPU_COMPLETION, waitProc->procId, waitProc->StartTime + waitProc->execTime);
                       eventQueue->push(runEvent);
                       
                       Process *running = new Process(waitProc->procId, waitProc->StartTime + waitProc->execTime, waitProc->execTime, waitProc->request);
                       running->status = Process::READY;
                       runTable.push_back(running);
                       totalCPUNum -= find->request;
                       cout << "Process: " << waitProc->procId <<
                       endl << "Using cpu number: " << find->request <<
                       endl << "Remaining cpu number: " << totalCPUNum <<
                       endl << "Start time: " << waitProc->StartTime << endl << endl;
                           for (int i = 0; i < runTable.size(); i++) {
                               Process *runProc = runTable[i];
                               if ( runProc->status == Process::ESTIMATE && runProc->reduce == -1 ) {
                                   runProc->reduce = 0;
                                   if ( waitProc->StartTime > runProc->anchor_start )
                                   {
                                   runProc->remain_node += waitProc->request;
                                   }
                               }
                           }
                           break;
                       }
                       else if ( sucess && ( waitProc->request > totalCPUNum || ( waitProc->request <= totalCPUNum && !back_sucess )) ) {
                            Process *find = findProcess(waitProc->procId);
                            Process *Proc = new Process(waitProc->procId, anchor_point, waitProc->execTime, waitProc->request);
                            Process *run = new Process(waitProc->procId, anchor_point + waitProc->execTime, waitProc->execTime, waitProc->request);
                            run->status = find->status = Process::READY;
                            runTable.push_back(run);
                           
                           Proc->status = Process::ESTIMATE;
                           Proc->anchor_start = anchor_point;
                           Proc->remain_node = usage - waitProc->request;
                           runTable.push_back(Proc);
                           estimateQueue.push(Proc);
                           cout << "Process " << waitProc->procId << ":" << endl <<
                           "Resource need: " << waitProc->request << endl <<
                           "Remain node: " << usage - waitProc->request << endl <<
                           "Submit time: " << waitProc->getArrivalTime() << endl <<
                           "Anchor point: " << anchor_point << endl << endl;
                           for (int i = 0; i < runTable.size(); i++) {
                               Process *runProc = runTable[i];
                               if ( runProc->status == Process::ESTIMATE && runProc->reduce == -1 ) {
                                   runProc->reduce = 0;
                                   if ( anchor_point > runProc->anchor_start )
                                   {
                                   runProc->remain_node += waitProc->request;
                                   }
                               }
                           }
                           break;
                       }
                }
        }
                       for (int i = 0; i < runTable.size(); i++) {
                           runningQueue.push(runTable[i]);
                       }
                       runTable.clear();
        }
    }
}
