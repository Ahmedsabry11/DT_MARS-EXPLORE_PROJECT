#include "UI.h"
#include<string>


UI::UI()
	:Mode(Terminal_Mode::Interactive)
{
	for (int i = 0; i < 3; i++)
		std::cout << "\n================================================================================================================\n";
	SelectMode();
}

UI::~UI()
{
	if (Mode == Silent)
	{
		cout << endl<<"Silent Mode" << endl;
		cout << "Simulation Starts..." ;
	}
	std::cout << "\nSimulation ends, Output file created\n";
	for (int i = 0; i < 3; i++)
		std::cout << "\n================================================================================================================\n";

}
Terminal_Mode UI::get_mode()
{
	return Mode;
}
void UI::SelectMode()
{
	string M;
	std::cout << "\n<<<<<<<< Please select simulation mode [default mode : step-by-step] >>>>>>>>\n" <<
		"1-Interactive Mode  (produces output file including simulation statistics, monitors & prints daily statistics\n during simulation on terminal window, you can increase simulation days manually!) \n" <<
		"2-Step-By-Step Mode (same as interactive , but increases days automatically)! \n" <<
		"3-Silent Mode		 (program produces only output files, no simulation steps will be shown on the console)!\n" <<
		"please type number of mode from (1,2 and 3)\n";
	std::cin >> M;

	if (M == "1")
	{
		Mode = Terminal_Mode::Interactive;
	}
	else if (M == "2")
	{
		Mode = Terminal_Mode::Step_By_Step;
	}
	else if (M == "3")
	{
		Mode = Terminal_Mode::Silent;
	}
	else
	{
		Mode = Terminal_Mode::Step_By_Step;
	}
	
}

void UI::Print_In_Execution_Missions_Rovers(int NUM_OF_Missions, PriorityQueue<Mission*> Emergency_EX_Mission, PriorityQueue<Mission*> Mountainous_EX_Mission, PriorityQueue<Mission*> Polar_EX_Mission)
{
	cout << NUM_OF_Missions << " In-Execution Missions/Rovers: ";
	Mission* PTR = nullptr;
	int count = 0;//To avoid the comma
	//printing the emergency
	if (Emergency_EX_Mission.peek(PTR))
	{
		cout << '[';
	}
    while (Emergency_EX_Mission.dequeue(PTR))
    {
			if (count != 0)
			{
				cout << ',';
			}
			cout << PTR->Get_ID() << '/'<<PTR->Get_Rptr()->GetID();
			count++;
    }
	if (count != 0)
	{
		cout << ']'<<" ";
	}
	//printing the polar
	count = 0;
	if (Polar_EX_Mission.peek(PTR))
	{
		cout << '(';
	}
	while (Polar_EX_Mission.dequeue(PTR))
	{
		if (count != 0)
		{
			cout << ',';
		}
		cout << PTR->Get_ID() << '/' << PTR->Get_Rptr()->GetID();
		count++;
	}
	if (count != 0)
	{
		cout << ')' << " ";
	}
	//printing the mountainous
	count = 0;
	
	if (Mountainous_EX_Mission.peek(PTR))
	{
		cout << '{';
	}
	while (Mountainous_EX_Mission.dequeue(PTR))
	{
		if (count != 0)
		{
			cout << ',';
		}
		cout << PTR->Get_ID() << '/' << PTR->Get_Rptr()->GetID();
		count++;
	}
	if (count != 0)
	{
		cout << '}' << " ";
	}
	cout << endl << "---------------------------------------------------------------------------------------" << endl;
}

void UI::Print_Completed(int NUM_OF_Missions, string& M_ID, string& P_ID, string& E_ID)
{
	NUM_OF_Missions = 0;
	int count1 = count(M_ID.begin(), M_ID.end(), ',');
	int count2 = count(P_ID.begin(), P_ID.end(), ',');
	int count3 = count(E_ID.begin(), E_ID.end(), ',');
	if (M_ID != "") { NUM_OF_Missions += count1 ; }
	if (P_ID != "") { NUM_OF_Missions += count2 ; }
	if (E_ID != "") { NUM_OF_Missions += count3 ; }
	cout << NUM_OF_Missions<<" Completed Missions: ";
	if (M_ID != "")
	{
		M_ID = M_ID.substr(0,M_ID.size() - 1);
		cout << "  " << '{' << M_ID << '}';
		//M_ID = "";
	}

	if (P_ID != "")
	{
		P_ID = P_ID.substr(0,P_ID.size() - 1);
		cout <<"  " <<'(' << P_ID << ')';
		//P_ID = "";
	}

	if (E_ID != "")
	{
		E_ID=E_ID.substr(0,E_ID.size()-1);
		cout<<"  "<< '[' << E_ID << ']';
		//E_ID = "";
	}
	
	cout << endl << "---------------------------------------------------------------------------------------" << endl;
}

void UI::Print_In_Checkup_Rovers(int NUM_OF_Rovers, LinkedQueue<Rover*>& Check_up_ER,
	LinkedQueue<Rover*>& Check_up_PR, LinkedQueue<Rover*>& Check_up_MR)
{
	cout << NUM_OF_Rovers << " In-Checkup Rovers: ";
	Rover* PTR = nullptr;
	Rover* TOP = nullptr;
	int count = 0;//To avoid the comma
	//printing the Emergency

	Check_up_ER.peek(PTR);
	if (PTR)
	{
		PTR = nullptr;
		Check_up_ER.dequeue(TOP);
		if (TOP)
		{
			cout << '[';
			cout << TOP->GetID();
			count++;
		}
		Check_up_ER.enqueue(TOP);
		while (TOP != PTR)
		{
			Check_up_ER.peek(PTR);
			if (PTR != TOP)
			{
				Check_up_ER.dequeue(PTR);
			}
			else
			{
				break;
			}
			if (count != 0)
			{
				cout << ',';
			}

			cout << PTR->GetID();
			count++;
			Check_up_ER.enqueue(PTR);
		}
		if (count != 0)
			cout << ']' << ' ';
	}
	// printing the polar 
	count = 0;
	PTR = nullptr;
	Check_up_PR.peek(PTR);
	if (PTR)
	{

		PTR = nullptr;
		Check_up_PR.dequeue(TOP);
		if (TOP)
		{
			cout << "(";
			cout << TOP->GetID();
			count++;
		}
		Check_up_PR.enqueue(TOP);
		while (TOP != PTR)
		{
			Check_up_PR.peek(PTR);
			if (PTR != TOP)
			{
				Check_up_PR.dequeue(PTR);
			}
			else
			{
				break;
			}
			if (count != 0)
			{
				cout << ',';
			}

			cout << PTR->GetID();
			count++;
			Check_up_PR.enqueue(PTR);
		}
		if (count != 0)
			cout << ')' << ' ';
	}
	//printing the Mountainous
	count = 0;
	PTR = nullptr;
	Check_up_MR.peek(PTR);
	if (PTR)
	{
		

		PTR = nullptr;
		Check_up_MR.dequeue(TOP);
		if (TOP)
		{
			cout << '{';
			cout << TOP->GetID();
			count++;
		}
		Check_up_MR.enqueue(TOP);
		while (TOP != PTR)
		{
			Check_up_MR.peek(PTR);
			if (PTR != TOP)
			{
				Check_up_MR.dequeue(PTR);
			}
			else
			{
				break;
			}
			if (count != 0)
			{
				cout << ',';
			}

			cout << PTR->GetID();
			count++;
			Check_up_MR.enqueue(PTR);
		}
		if (count != 0)
			cout << '}' << ' ';
	}
	cout << endl << "---------------------------------------------------------------------------------------" << endl;
}


void UI::print_Availble_missions(int day_count,int waiting_missions,PriorityQueue<Mission*>& E_Mission,
	LinkedQueue<Mission*>& P_Mission, LinkedQueue<Mission*>& M_Mission)
{
	cout << "current day:" << day_count << endl;
	cout << waiting_missions <<" Waiting Missions: ";
	if (!E_Mission.isEmpty())
	{
		cout << "[";
		print_Availble_missions_PriorityQueue(E_Mission);
		cout <<"] ";
	}
	if (!P_Mission.isEmpty())
	{
		cout << "(";
		print_Availble_missions_Queue(P_Mission);
		cout <<  ") ";
	}
	if (!M_Mission.isEmpty())
	{
		
		cout << "{";
		print_Availble_missions_Queue(M_Mission);
		cout <<  "} ";
	}
	cout << endl << "---------------------------------------------------------------------------------------" << endl;
}
void UI::Print_Availble_Rover(int availble_Rover_count,PriorityQueue<Rover*> Available_ER,
	PriorityQueue<Rover*> Available_MR,
	PriorityQueue<Rover*> Available_PR)
{
	Rover* rover;
	Rover* rover_next;
	cout << availble_Rover_count << " Availble Rovers: ";
	if (!Available_ER.isEmpty())
	{
		Available_ER.dequeue(rover);
		cout << "[";
		while (Available_ER.dequeue(rover_next))
		{
			cout << rover->GetID() << ",";
			rover = rover_next;
		}
		cout << rover->GetID() << "] ";
	}
	if (!Available_PR.isEmpty())
	{
		Available_PR.dequeue(rover);
		cout << "(";
		while (Available_PR.dequeue(rover_next))
		{
			cout << rover->GetID() << ",";
			rover = rover_next;
		}
		cout << rover->GetID() << ") ";
	}
	if (!Available_MR.isEmpty())
	{
		Available_MR.dequeue(rover);
		cout << "{";
		while (Available_MR.dequeue(rover_next))
		{
			cout << rover->GetID() << ",";
			rover = rover_next;
		}
		cout << rover->GetID() << "} ";
	}
	cout << endl<<"---------------------------------------------------------------------------------------" << endl;
}
void UI::print_Availble_missions_Queue(LinkedQueue<Mission*>& Queue_Mission_List)
{
	Mission* mission, * mission_next, * TOP;
	Queue_Mission_List.dequeue(TOP);
	Queue_Mission_List.enqueue(TOP);
	cout << TOP->Get_ID();
	if (Queue_Mission_List.peek(mission))
	{
		while (TOP != mission)
		{
			Queue_Mission_List.dequeue(mission);
			cout << "," << mission->Get_ID();
			Queue_Mission_List.enqueue(mission);
			Queue_Mission_List.peek(mission);
		}
	}
}
void UI::print_Availble_missions_PriorityQueue(PriorityQueue<Mission*> PriorityQueue_Mission_List)
{
	Mission* mission,* mission_next;
	PriorityQueue_Mission_List.dequeue(mission);
	cout << mission->Get_ID();
	while (PriorityQueue_Mission_List.dequeue(mission))
	{
		cout << "," << mission->Get_ID();
	}
	
}
string UI::read_input_file_name( int i)
{if(i==1)
{
	string FName;
	cout << "\nPlease::enter the name of the file the you want to load it\n" << endl;
	cin >> FName;
	return FName;
}
if (i == 2)
{
	string FName;
	cout << "the file is not exist" << endl;
	cout << "Please::enter the correct name or if you want to exit enter number 0\n" << endl;
	cin >> FName;
	return FName;
}
	
}
void UI::read_output_file_name()
{
	cout << "Please::enter the name of the output file to load the data in it" << endl<<endl;
	cin >> Output_File_Name;
}

string UI::Get_Output_File_Name()
{
	return Output_File_Name;
}



/*
void UI::Print_Availble_Rover(int availble_Rover_count, PriorityQueue<Rover*> Available_ER,
	PriorityQueue<Rover*> Available_MR,
	PriorityQueue<Rover*> Available_PR)
{
	Rover* rover;
	Rover* rover_next;
	cout << availble_Rover_count << " Availble Rovers: ";
	if (!Available_ER.isEmpty())
	{
		Available_ER.dequeue(rover);
		cout << "[";
		while (Available_ER.dequeue(rover_next))
		{
			cout << rover->GetID() << ",";
			rover = rover_next;
		}
		cout << rover->GetID() << "] ";
	}
	if (!Available_PR.isEmpty())
	{
		Available_PR.dequeue(rover);
		cout << "(";
		while (Available_PR.dequeue(rover_next))
		{
			cout << rover->GetID() << ",";
			rover = rover_next;
		}
		cout << rover->GetID() << ") ";
	}
	if (!Available_MR.isEmpty())
	{
		Available_MR.dequeue(rover);
		cout << "{";
		while (Available_MR.dequeue(rover_next))
		{
			cout << rover->GetID() << ",";
			rover = rover_next;
		}
		cout << rover->GetID() << "} ";
	}
	cout << endl << "---------------------------------------------------------------------------------------" << endl;
}*/



