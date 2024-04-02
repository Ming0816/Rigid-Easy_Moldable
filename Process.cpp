#include<iostream>
#include "Process.h"
using namespace std;

Process::Process(int procId, int arrivalTime, int execTime, int request) {
	this->procId = procId;
	this->arrivalTime = arrivalTime;
    this->execTime = execTime;
    this->request = request;
}

bool SmallestFirst::operator ()(Process *a, Process *b) {
	return (a->request == b->request) ? (a->arrivalTime > b->arrivalTime) : (a->request > b->request);
}

bool LargestFirst::operator ()(Process *a, Process *b) {
    return (a->request == b->request) ? (a->arrivalTime > b->arrivalTime) : (a->request < b->request);
}

bool FCFS::operator ()(Process *a, Process *b) {
    return (a->arrivalTime == b->arrivalTime) ? (a->procId > b->procId) :
        (a->arrivalTime > b->arrivalTime);
}


int Process::getArrivalTime() {
	return arrivalTime;
}
