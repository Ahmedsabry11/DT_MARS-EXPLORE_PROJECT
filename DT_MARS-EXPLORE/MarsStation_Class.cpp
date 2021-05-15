#include "MarsStation_Class.h"
#include"Event.h"
#include"Formulaion_Event.h"
#include"Cancellation_Event.h"
#include"Promotion_Event.h"
#include"Mountainous_Mission.h"
#include"Rover.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int  MarsStation_Class::files_Count = 1;//#files to be created for output(#user operations), needs discussion with team

MarsStation_Class::MarsStation_Class()
	:WD_SUM(0)
{
	files_Count++;
}
//////////////// ASsign Emergency Missions using Priority Queue (Linked List) //////////////////////////
void MarsStation_Class::Execute()
{
	Event* event;
	while (Events_List.peek(event))
	{
		if (event->Get_Event_Day() == Day_count)
		{
			Events_List.dequeue(event);
			event->Execute(E_Mission, P_Mission, M_Mission);
		}
	}
}

void MarsStation_Class::Assign_E_M()
{
	if (Available_ER.isEmpty() && Available_PR.isEmpty() && Available_MR.isEmpty())
	{
		return;
	}
	else
	{
		Emergency_Mission* Emergence_mission;
		Rover* E_Rover;
		Rover* M_Rover;                //// pointer to rovers
		Rover* P_Rover;
		while (E_Mission.dequeue(Emergence_mission))
		{
			if (Available_ER.dequeue(E_Rover))      ///// Check Emergency Rover list first
			{
				Emergence_mission->Set_Rptr(E_Rover);
			}
			else if (Available_MR.dequeue(M_Rover)) ///// Check Mountainous Rover list second
			{
				Emergence_mission->Set_Rptr(M_Rover);
			}
			else                                     ///// Check Polar Rover list Last
			{
				Available_PR.dequeue(P_Rover);
				Emergence_mission->Set_Rptr(P_Rover);
			}
			Emergence_mission->Calculate_WD(Day_count); ///// Add Mission from available to Excution Mission list 
			Emergency_EX_Mission.enqueue(Emergence_mission, Emergence_mission->Calculate_CD_Priority());
		}
	}
}
void MarsStation_Class::Assign_P_M()
{
	if (Available_PR.isEmpty())
		return;
	else
	{
		Polar_Mission* Polar_mission;
		Rover* P_Rover;
		while (P_Mission.dequeue(Polar_mission))
		{
			if (Available_PR.dequeue(P_Rover))      ///// Check Emergency Rover list first
			{
				Polar_mission->Set_Rptr(P_Rover);
			}
			Polar_mission->Calculate_WD(Day_count); ///// Add Mission from available to Excution Mission list 
			Polar_EX_Mission.enqueue(Polar_mission, Polar_mission->Calculate_CD_Priority()); //// note: sorted ascending 
		}
	}
}

void MarsStation_Class::Assign_M_M()
{
	//23ml auto promoted b3d el assign el 3ady?
	Mountainous_Mission* MMptr;
	Rover* ARptr;
	//M_Mission.peek(MMptr)
	while (!M_Mission.isEmpty() && !Available_MR.isEmpty())
	{
		//if()
		M_Mission.dequeue(MMptr);
		Available_MR.dequeue(ARptr);//??
		ARptr->Increment_Mission_Count();
		MMptr->Calculate_WD(Day_count);
		MMptr->Set_Rptr(ARptr);
		Mountainous_EX_Mission.enqueue(MMptr, MMptr->Calculate_CD_Priority());

	}
	while (!M_Mission.isEmpty() && !Available_ER.isEmpty())
	{
		M_Mission.dequeue(MMptr);
		Available_ER.dequeue(ARptr);//??
		ARptr->Increment_Mission_Count();
		MMptr->Calculate_WD(Day_count);
		MMptr->Set_Rptr(ARptr);
		Mountainous_EX_Mission.enqueue(MMptr, MMptr->Calculate_CD_Priority());

	}



}

void MarsStation_Class::Assign_All_Mission()
{
	Assign_E_M();
	Assign_M_M();
	Assign_P_M();
	Auto_Promoting();/////?is it will be like that at the first before any assign???
}

void MarsStation_Class::Emergency_EX_Mission_to_completed()
{
	Emergency_Mission* Emergence_mission;
	while (Emergency_EX_Mission.peek(Emergence_mission))
	{
		if (Emergence_mission->Get_CD() == Day_count)
		{
			Emergency_EX_Mission.dequeue(Emergence_mission);
			Temp_CD_Mission.enqueue(Emergence_mission, Emergence_mission->Calculate_ED_Priority());
			Completed_E_Mission_ID.enqueue(Emergence_mission->Get_ID());
			Rover* rover = Emergence_mission->Get_Rptr();
			if (rover->GetType() == Emergency)
			{
				Check_ER_State(rover);
			}
			else if (rover->GetType() == Mountainous)
			{
				Check_MR_State(rover);
			}
			else
			{
				Check_PR_State(rover);
			}
		}
		else
		{
			break;
		}
	}
}
void MarsStation_Class::Mountainous_EX_Mission_to_completed()
{
	Mountainous_Mission* Mountainous_mission;
	while (Mountainous_EX_Mission.peek(Mountainous_mission))
	{
		if (Mountainous_mission->Get_CD() == Day_count)
		{
			Mountainous_EX_Mission.dequeue(Mountainous_mission);
			Temp_CD_Mission.enqueue(Mountainous_mission, Mountainous_mission->Calculate_ED_Priority());
			Completed_M_Mission_ID.enqueue(Mountainous_mission->Get_ID());
			Rover* rover = Mountainous_mission->Get_Rptr();
			if (rover->GetType() == Mountainous)
			{
				Check_MR_State(rover);
			}
			else
			{
				Check_ER_State(rover);
			}
		}
		else
		{
			break;
		}
	}
}
void MarsStation_Class::Polar_EX_Mission_to_completed()
{
	Polar_Mission* Polar_mission;
	while (Polar_EX_Mission.peek(Polar_mission))
	{
		if (Polar_mission->Get_CD() == Day_count)
		{
			Polar_EX_Mission.dequeue(Polar_mission);
			Temp_CD_Mission.enqueue(Polar_mission, Polar_mission->Calculate_ED_Priority());
			Completed_E_Mission_ID.enqueue(Polar_mission->Get_ID());
			Check_PR_State(Polar_mission->Get_Rptr());
		}
		else
		{
			break;
		}
	}
}
///////////////////////////////////////////////////////////////
void MarsStation_Class::InExecution_to_Completed()
{
	Emergency_EX_Mission_to_completed();
	Mountainous_EX_Mission_to_completed();
	Polar_EX_Mission_to_completed();
	//Mamdouh : maybe here you can call out2? remember you shall send a queue of sorted completed missions ;) 
}

void MarsStation_Class::Auto_Promoting()
{
	Mountainous_Mission* MMptr;
	Emergency_Mission* EMptr;

	while (!M_Mission.isEmpty())
	{
		M_Mission.peek(MMptr);
		if (MMptr->Calculate_WD(Day_count) == Mountainous_Mission::AutoP)
		{
			M_Mission.dequeue(MMptr);
			EMptr = new Emergency_Mission(MMptr->Get_TLOC(), MMptr->Get_MDUR(), MMptr->Get_SIG(), MMptr->Get_FD(), MMptr->Get_ID());
			E_Mission.enqueue(EMptr, EMptr->Get_Priority());
			Mountainous_Mission::NumOfAutoPMissions++;
			Mountainous_Mission::NumOfMMissions--;
			//D.F to add NumOfAutoPMissions with NumOfMMissions to get num of totall m mission to calculat el nsba el m2wya for ayto promoting in output file
			delete MMptr;//?delete in general and null errors......
		}
		else
			break;//or return
	}


}


void MarsStation_Class::Check_MR_State(Rover* CMRptr)
{
	//dynamicM_Rover_Count
	if (CMRptr->GetMission_Count() == Rover::M_Rover_Count)
	{
		Check_up_MR.enqueue(CMRptr);
		//Rover::GetCheck_MR();
		CMRptr->Set_Day_out(Rover::Check_MR + Day_count);
	}
	else
	{
		Available_MR.enqueue(CMRptr, CMRptr->GetSpeed());
	}

}
//check rover satutes

void MarsStation_Class::Check_ER_State(Rover* CERptr)
{
	//dynamicM_Rover_Count
	if (CERptr->GetMission_Count() == Rover::E_Rover_Count)
	{
		Check_up_ER.enqueue(CERptr);
		CERptr->Set_Day_out(Rover::Check_ER + Day_count);
	}
	else
	{
		Available_ER.enqueue(CERptr, CERptr->GetSpeed());
	}

}

void MarsStation_Class::Check_PR_State(Rover* CPRptr)
{
	//dynamicM_Rover_Count
	if (CPRptr->GetMission_Count() == Rover::P_Rover_Count)
	{
		Check_up_PR.enqueue(CPRptr);
		CPRptr->Set_Day_out(Rover::Check_PR + Day_count);
	}
	else
	{
		Available_PR.enqueue(CPRptr, CPRptr->GetSpeed());
	}

}

void MarsStation_Class::Program_Startup()
{
	string File_Name, Line, ed = " ", id = " ", tolc = " ", mdur = " ", sig = " ";
	string* Speeds_Str;
	char Type_Event, Type_Mission;
	int Num_E_Rovers, Num_P_Rovers, Num_M_Rovers;
	int Num_Rovers;
	int Check_UP_E_Rover;
	int Check_UP_M_Rover;
	int Check_UP_P_Rover;
	int Missions_Before_Check_up;
	int Num_Event;
	int Auto_Promotion_in;
	int j, ED, ID, TOLC, MDUR, SIG;
	Rover** Array_OF_Rovers;
	int* Speeds;
	cout << "Please::enter the name of the file the you want to load it" << endl;
	cin >> File_Name;
	ifstream My_File;
	My_File.open("\Files\\" + File_Name + ".txt");
	if (My_File.is_open())                       //Check if the file is found or not
	{
		My_File >> Num_M_Rovers >> Num_P_Rovers >> Num_E_Rovers;
		Num_Rovers = Num_E_Rovers + Num_P_Rovers + Num_M_Rovers;
		Array_OF_Rovers = new Rover * [Num_Rovers];
		Speeds = new int[Num_Rovers];
		Speeds_Str = new string[Num_Rovers];
		//////////////////////Read the speed of the rover///////////////
		getline(My_File, Line);                             //To avoid the reminder of the line
		getline(My_File, Line);
		int i = 0;
		int Spaces = 0;
		for (int k = 0; k < Line.size(); k++)
		{
			if (Line[k] == ' ')
				Spaces++;
		}
		//////////////////// Bounas case///////////////////
	 ///////////////////reading the speeds///////////////////// 
		if (Spaces == Num_Rovers)        //The case of the different speeds
		{
			for (int j = 0; j < Num_Rovers; j++)
			{
				while (Line[i] != ' ')
				{
					int k = 0;
					Speeds_Str[j][k] = Line[i];
					i++;
					k++;
				}
				Speeds[j] = stoi(Speeds_Str[j]);
				while (Line[i] == ' ')
				{
					i++;
				}
			}
			///////////////////////////Creating the rovers////////////////////////////
			int counter = 0;
			int j = 0;
			for (int i = 0; i < Num_M_Rovers; i++)
			{
				Array_OF_Rovers[counter] = new Rover(Speeds[counter], Mountainous);
				Available_MR.enqueue(Array_OF_Rovers[counter], Array_OF_Rovers[counter]->GetSpeed());
				counter++;
			}
			for (int i = 0; i < Num_P_Rovers; i++)
			{
				Array_OF_Rovers[counter] = new Rover(Speeds[counter], Polar);
				Available_PR.enqueue(Array_OF_Rovers[counter], Array_OF_Rovers[counter]->GetSpeed());
				counter++;
			}
			for (int i = 0; i < Num_E_Rovers; i++)
			{
				Array_OF_Rovers[counter] = new Rover(Speeds[counter], Emergency);
				Available_ER.enqueue(Array_OF_Rovers[counter], Array_OF_Rovers[counter]->GetSpeed());
				counter++;
			}

		}
		else
		{                          ////////////////the speed is const/////////////   
			for (int j = 0; j < 3; j++)
			{
				while (Line[i] != ' ')
				{
					int k = 0;
					Speeds_Str[j][k] = Line[i];
					i++;
					k++;
				}
				Speeds[j] = stoi(Speeds_Str[j]);
				while (Line[i] == ' ')
				{
					i++;
				}
			}
			int counter = 0;          //to the Array of rovers 
			int counter1 = 0;          //to the Array of speeds
			for (int i = 0; i < Num_M_Rovers; i++)
			{
				Array_OF_Rovers[counter] = new Rover(Speeds[counter1], Mountainous);
				Available_MR.enqueue(Array_OF_Rovers[counter], Array_OF_Rovers[counter]->GetSpeed());
				counter++;
			}
			counter1++;
			for (int i = 0; i < Num_P_Rovers; i++)
			{
				Array_OF_Rovers[counter] = new Rover(Speeds[counter1], Polar);
				Available_PR.enqueue(Array_OF_Rovers[counter], Array_OF_Rovers[counter]->GetSpeed());
				counter++;
			}
			counter1++;
			for (int i = 0; i < Num_E_Rovers; i++)
			{
				Array_OF_Rovers[counter] = new Rover(Speeds[counter1], Emergency);
				Available_ER.enqueue(Array_OF_Rovers[counter], Array_OF_Rovers[counter]->GetSpeed());
				counter++;
			}
		}
		My_File >> Missions_Before_Check_up >> Check_UP_M_Rover >> Check_UP_P_Rover >> Check_UP_E_Rover;
		///////////////////////select the static data to the rovers ///////////////////////
		Rover::P_Rover_Count = Num_P_Rovers;
		Rover::Check_PR = Check_UP_P_Rover;
		Rover::E_Rover_Count = Num_E_Rovers;
		Rover::Check_ER = Check_UP_E_Rover;
		Rover::M_Rover_Count = Num_M_Rovers;
		Rover::Check_MR = Check_UP_M_Rover;
		Rover::Missions_Before_Check_Up = Missions_Before_Check_up;

		My_File >> Auto_Promotion_in;                //The value of the auto promotion limit
		Mountainous_Mission::AutoP = Auto_Promotion_in;
		My_File >> Num_Event;                       //The number of events
		Event** PTR_Event = new Event * [Num_Event];
		getline(My_File, Line);                    //To avoid the reminder of the line
		for (int i = 0; i < Num_Event; i++)         //Loop to fill the list of the events
		{
			j = 0;
			getline(My_File, Line);                   //The line of the event data
			Type_Event = Line[j];                        //type of the event formulation or promotion or cancellation  
			if (Type_Event == 'F')
			{
				j++;
				while (Line[j] == ' ')
				{
					j++;
				}
				Type_Mission = Line[j];                       //type of the mission Em or moun or polar
				j++;
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						ed += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				ED = stoi(ed);
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						id += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				ID = stoi(id);
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						tolc += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				TOLC = stoi(tolc);
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						mdur += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				MDUR = stoi(mdur);
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size() && j < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						sig += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				SIG = stoi(sig);
			}
			else if (Type_Event == 'P' || Type_Event == 'X')
			{
				j++;
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						ed += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				ED = stoi(ed);
				while (Line[j] == ' ')
				{
					j++;
				}
				for (int i = 0; i < Line.size(); i++)
				{
					if (Line[j] != ' ')
					{
						id += Line[j];
						j++;
					}
					else
					{
						break;
					}
				}
				ID = stoi(id);
			}
			//////////////Creating the events/////////////
			if (Type_Event == 'P')
			{
				PTR_Event[i] = new Promotion_Event(ID, ED);
				Events_List.enqueue(PTR_Event[i]);
			}
			if (Type_Event == 'F')
			{
				PTR_Event[i] = new Formulaion_Event(Type_Mission, TOLC, MDUR, SIG, ID, ED);
				Events_List.enqueue(PTR_Event[i]);
			}
			if (Type_Event == 'X')
			{
				PTR_Event[i] = new Cancellation_Event(ID, ED);
				Events_List.enqueue(PTR_Event[i]);
			}
			ed = " ", id = " ", tolc = " ", mdur = " ", sig = " ";

		}
	}
}

void MarsStation_Class::Check_Up_to_Available_M()
{
	Rover* MMptr;
	Check_up_MR.peek(MMptr);
	while (MMptr->Get_Day_out() == Day_count)
	{
		Check_up_MR.dequeue(MMptr);
		Available_MR.enqueue(MMptr, MMptr->GetSpeed());
		Check_up_MR.peek(MMptr);
	}
}
void MarsStation_Class::Check_Up_to_Available_E()
{
	Rover* EMptr;
	Check_up_ER.peek(EMptr);
	while (EMptr->Get_Day_out() == Day_count)
	{
		Check_up_ER.dequeue(EMptr);
		Available_ER.enqueue(EMptr, EMptr->GetSpeed());
		Check_up_ER.peek(EMptr);
	}

}
void MarsStation_Class::Check_Up_to_Available_P()
{
	Rover* PMptr;
	Check_up_PR.peek(PMptr);
	while (PMptr->Get_Day_out() == Day_count)
	{
		Check_up_PR.dequeue(PMptr);
		Available_PR.enqueue(PMptr, PMptr->GetSpeed());
		Check_up_PR.peek(PMptr);
	}

}
void MarsStation_Class::Check_Up_to_Available_All()
{
	 Check_Up_to_Available_M();
	 Check_Up_to_Available_E();
	 Check_Up_to_Available_P();
}
bool MarsStation_Class::isFinished()
{
	return (Events_List.isEmpty() && P_Mission.isEmpty() && M_Mission.isEmpty() &&
		E_Mission.isEmpty() && Emergency_EX_Mission.isEmpty() && Mountainous_EX_Mission.isEmpty() &&
		Polar_EX_Mission.isEmpty() && Temp_CD_Mission.isEmpty());
}

void MarsStation_Class::Out1()
{
	ofstream outF;//variable to deal with output file , declared here for multiple functions
	if (files_Count == 1)
		outF.open("Station Statistics"+to_string(files_Count)+".txt", ios::out);

	outF << "CD\t ID\t FD\t WD\t ED\n";
	outF.close();
}

void MarsStation_Class::Out2(PriorityQueue<Mission*>& CM)
{
	ofstream outF;//variable to deal with output file , declared here for multiple functions

	outF.open("Station Statistics" + to_string(files_Count) + ".txt", ios::app);
	
	Mission* dummy_mission;
	while (CM.dequeue(dummy_mission))
	{
		outF << dummy_mission->Get_CD() << "\t" << dummy_mission->Get_ID() << "\t"
			<< dummy_mission->Get_FD() << "\t" << dummy_mission->Get_WD() << "\t"
			<< dummy_mission->Calculate_ED() << "\n";

		delete dummy_mission;
	}
}

void MarsStation_Class::Out3()
{
	ofstream outF;//variable to deal with output file , declared here for multiple functions

	outF.open("Station Statistics" + to_string(files_Count) + ".txt", ios::app);

	outF << "������������������\n������������������\n"
		<< "Missions:" << Mountainous_Mission::NumOfMMissions + Polar_Mission::NumOfPMissions + Emergency_Mission::NumOfEMissions;
	outF << " [M: " << Mountainous_Mission::NumOfMMissions << ", P: " << Polar_Mission::NumOfPMissions
		<< ", E: " << Emergency_Mission::NumOfEMissions << "]\n";
	outF << "Rovers:" << Rover::E_Rover_Count + Rover::P_Rover_Count + Rover::M_Rover_Count
		<< " \t[M: " << Rover::M_Rover_Count << ", P: " << Rover::P_Rover_Count << ", E: "
		<< Rover::E_Rover_Count << "]\n";
	outF << "Avg Wait = "/*<< */ << ", Avg Exec"/*<<*/ << endl;
	outF << "Auto-promoted: " <</* <<*/"%\n";

	outF.close();

}


int MarsStation_Class::Day_count = 0;
