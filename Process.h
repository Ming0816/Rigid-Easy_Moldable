#pragma once
using namespace std;

class Process {

	public:
        enum Status {
            READY,
            TERMINATED,
            WAITING,
            ESTIMATE
        };
        Status status;
		int procId;
		int arrivalTime;
        int StartTime;
        int execTime;
		int finishTime;
        int request;
        int remain_node;
        int anchor_start;
        int reduce;
		int getArrivalTime();
        Process(int procId, int arrivalTime, int execTime, int request);
};


class SmallestFirst {
	public:
		bool operator ()(Process *a, Process *b);
};

class LargestFirst {
    public:
        bool operator ()(Process *a, Process *b);
};

class FCFS {
    public:
        bool operator ()(Process *a, Process *b);
};
