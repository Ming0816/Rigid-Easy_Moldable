#pragma once
using namespace std;

class Event {

	public:
		enum EventType { PROCESS_ARRIVAL, CPU_COMPLETION
        };
		EventType eventType;
        int procId;
        int time;
        Event(EventType eventType, int procId, int time);
};

class EventComparator {
	public:
		bool operator ()(Event *a, Event *b);
};

