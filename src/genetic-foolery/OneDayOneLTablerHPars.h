#pragma once

#include <util/Macros.h>

typedef int Professor;
typedef int Subject;
typedef int Group;
typedef int Room;
typedef int HourOfDay;

#define NUM_Professors 6
#define NUM_Subjects 4
#define NUM_Groups 5
#define NUM_Rooms 9
#define NUM_Hours 10

#define Professor_None -1
#define Subject_None -1
#define Group_None -1
#define Room_None -1

#define FOR_T(T, var, MAX) for(T var = 0; var < MAX; var++)
#define FOR_Professor(var) FOR_T(Professor, var, NUM_Professors)
#define FOR_Subject(var) FOR_T(Subject, var, NUM_Subjects)
#define FOR_Group(var) FOR_T(Group, var, NUM_Groups)
#define FOR_Room(var) FOR_T(Room, var, NUM_Rooms)
#define FOR_Hour(var) FOR_T(HourOfDay, var, NUM_Hours)

#define RAND_T(T, rnd01, f_floor, MAX) clamp((T)f_floor(lerp(rnd01, 0, MAX)), 0, MAX-1)
#define RAND_Subject(rnd01, f_floor) RAND_T(Subject, rnd01, f_floor, NUM_Subjects)
#define RAND_Group(rnd01, f_floor) RAND_T(Group, rnd01, f_floor, NUM_Groups)
#define RAND_Room(rnd01, f_floor) RAND_T(Room, rnd01, f_floor, NUM_Rooms)
#define RAND_Hour(rnd01, f_floor) RAND_T(HourOfDay, rnd01, f_floor, NUM_Hours)

String NAMES_Professors[NUM_Professors] = {"John Wick", "Ice Bear", "Mr Potato", "H.P. Lovecraft", "Godzilla", "Dumbledore"};
String NAMES_Subjects[NUM_Subjects] = {"Wizardry", "Gun Practice", "Fun Geometry", "Cooking"};
String NAMES_Groups[NUM_Groups] = {"Potheads", "Chillheads", "Rawheads", "Drunkheads", "Funheads", /*"Numbheads", "Trampheads"*/};
String NAMES_Rooms[NUM_Rooms] = {"F1R1", "F1R2", "F1R3", "F2R1", "F2R2", "F2R3", /*"F3R1", "F3R2", "F3R3",*/ "Basement", "Rooftop", "Garden"};

Professor GroupSubject2Professor[NUM_Groups][NUM_Subjects] = {
	{5, 0, 3, 1},
	{5, 0, 4, 2},
	{4, 0, 4, 2},
	//{5, 0, 3, 2},
	//{4, 1, 2, 2},
	{5, 1, 3, 2},
	{4, 1, 3, 1}
};

int Subject2Duration[NUM_Subjects] = {1, 1, 1, 1};
