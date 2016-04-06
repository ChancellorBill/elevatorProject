#include "Header.h"
#include "User.h"
#include "elevator.h"
#include "PrototypeDefinitions.h"
#include "PriorityQueue.h"
#include <iostream>

using namespace std;

void Elevator::progression(vector<User> &peopleWait, vector<User> &peopleAccepted, vector<User> &peopleProgress, vector<User> &peopleDone)
{
	//Declarations:
	bool addAccepted = false;
	bool eraseOccured = false;
	bool atStop;
	vector<int> deletion;
	
	cout << "Elevator current floor: " << currentFloor << endl;
	cout << peopleWait.size() << endl;
	if (!peopleWait.empty())
	{
		if (currentPriorities.size() == 0)
		{
			currentDirection = peopleWait.front().getIntendedDirection();
			currentPriorities.changeDirection(currentDirection);
			currentPriorities.add(peopleWait.front().getCurrentFloor());
			peopleAccepted.push_back(peopleWait.front());
			peopleWait.erase(peopleWait.begin());
			if (currentDirection == UP)
				cout << "UP established!" << endl;
			else
				cout << "Down established!" << endl;
		}
	}	
	cout << "Elevators current destination: " << currentPriorities.get() << endl;
	if (!peopleWait.empty())
	{
		for (int i = 0; i < peopleWait.size(); i++)
		{
			if (peopleWait[i].getIntendedDirection() == currentDirection)
			{
				if (currentDirection == UP)
				{
					if (peopleWait[i].getCurrentFloor() < currentPriorities.get()) // Make sure their current floor is below the elevator's current destination floor
					{
						if (peopleWait[i].getCurrentFloor() > currentFloor) // Make sure their current floor is above the elevator's current floor
						{
							addAccepted = true;
						}
					}
				}
				else
				{
					if (peopleWait[i].getCurrentFloor() > currentPriorities.get()) // Make sure their current floor is above the elevator's current destination floor
					{
						if (peopleWait[i].getCurrentFloor() < currentFloor) // Make sure their current floor is below the elevator's current floor
						{
							addAccepted = true;
						}
					}
				}
			}
			if (addAccepted)
			{
				currentPriorities.add(peopleWait[i].getCurrentFloor()); // Add the floor to the top of the elevator's desintation queue
				peopleAccepted.push_back(peopleWait[i]); // Add to accepted list
				deletion.push_back(i); // Add to deletion list, to be removed from wait list
				eraseOccured = true;
			}
		}
	}
	deleteEntries(peopleWait, deletion);
	deletion.clear();
	
	atStop = (currentPriorities.get() == currentFloor); // Check if the currentFloor is a stop point
	if (atStop) //If the floor the elevator is at is one of its destinations (atStop == true) then this statment will run
	{
		cout << "Elevator arrived at destination" << endl;
		incrementTimeAllUsers(peopleWait, true);
		incrementTimeAllUsers(peopleAccepted, true);
		incrementTimeAllUsers(peopleProgress, true);
		for (int i = 0; i < peopleAccepted.size(); i++) // Check to see who gets on the elevator
		{
			if (peopleAccepted[i].getCurrentFloor() == currentFloor) // They must have already been accepted to get on, meaning they have the same direction and are on the way to the elevator's destination
			{
				cout << "Boarding" << endl;
				peopleAccepted[i].board(); // Board the elevator
				currentPriorities.add(peopleAccepted[i].getDesiredFloor()); // Add the new user's desired floor to the priority queue
				peopleProgress.push_back(peopleAccepted[i]); // Add to progress list
				deletion.push_back(i); // Remove from accepted list
			}
		}
		deleteEntries(peopleAccepted, deletion);
		deletion.clear();
		
		cout << peopleProgress.size() << endl;
		for (int i = 0; i < peopleProgress.size(); i++)
		{
			cout << peopleProgress[i].getDesiredFloor() << " " << currentFloor << endl;
			if (peopleProgress[i].getDesiredFloor() == currentFloor)
			{
				cout << "Arriving" << endl;
				peopleProgress[i].arrive(currentFloor); // Get off the elevator
				peopleDone.push_back(peopleProgress[i]); // Add to done list
				deletion.push_back(i); // Remove from progress list
			}
			
		}
		deleteEntries(peopleProgress, deletion);
		deletion.clear();
		
		currentPriorities.complete();
		cout << "current priority complete" << endl;
	}
	else
	{
		incrementTimeAllUsers(peopleWait, false);
		incrementTimeAllUsers(peopleAccepted, false);
		incrementTimeAllUsers(peopleProgress, false);
	}
	if (!currentPriorities.empty())
	{
		if (currentPriorities.get() > currentFloor)
		{
			cout << "Previous floor: " << currentFloor << endl;
			cout << "Go up" << endl;
			goUp();
			cout << "New floor: " << currentFloor << endl;
		}
		else
		{
			cout << "Previous floor: " << currentFloor << endl;
			cout << "Go down" << endl;
			goDown();
			cout << "New floor: " << currentFloor << endl;
		}
	}
	cout << "Progress complete" << endl;
}