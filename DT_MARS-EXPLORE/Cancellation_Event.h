#pragma once
#include "Event.h"
#include"LinkedQueue.h"
#include"Mountainous_Mission.h"
class Cancellation_Event :
    public Event
{

public:
    Cancellation_Event(int id,int ED);
    void Execute(PriorityQueue<Mission*>& Eme_Missions, LinkedQueue<Mission*>& Pol_Missions, LinkedQueue<Mission*>& Mou_Missions);
    ~Cancellation_Event();
};

