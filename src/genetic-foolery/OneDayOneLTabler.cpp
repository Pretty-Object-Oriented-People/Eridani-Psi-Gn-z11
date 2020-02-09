#include "GenoFun.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <openga/openga.hpp>
#include <json-c/json.h>

namespace ODOLT {

#include "OneDayOneLTablerHPars.h"

using std::string;
using std::cout;
using std::endl;

struct DayTimetable {

	Subject GroupHour2Subject[NUM_Groups][NUM_Hours];
	Room GroupHour2Room[NUM_Groups][NUM_Hours];

	void init(){
		std::fill(&GroupHour2Subject[0][0], &GroupHour2Subject[0][0] + NUM_Groups*NUM_Hours, Subject_None);
		std::fill(&GroupHour2Room[0][0], &GroupHour2Room[0][0] + NUM_Groups*NUM_Hours, Room_None);
	}

	//GA

	bool isValid() const {
		bool RoomHour2Occupancy[NUM_Rooms][NUM_Hours] = {};
		bool ProfessorHour2Occupancy[NUM_Professors][NUM_Hours] = {};
		FOR_Group(g){
			int actualSubject2Duration[NUM_Subjects] = {};
			FOR_Hour(h){
				let s = GroupHour2Subject[g][h];
				let r = GroupHour2Room[g][h];
				let p = GroupSubject2Professor[g][s];
				if(s == Subject_None && r == Room_None) continue; //Valid: empty hour
				if(s != Subject_None && r != Room_None){
					if(RoomHour2Occupancy[r][h]) return false; //Invalid: Room conflict
					if(ProfessorHour2Occupancy[p][h]) return false; //Invalid: Professor conflict
					//Valid -> Mark occupied
					RoomHour2Occupancy[r][h] = true;
					ProfessorHour2Occupancy[p][h] = true;
					//Incr subject duration
					actualSubject2Duration[s]++;
				} else return false; //Invalid: Room without Subject or Subject without Room
			}
			FOR_Subject(s) if(actualSubject2Duration[s] != Subject2Duration[s]) return false; //Actual subject duration does not match requirements
		}
		//No conflicts, Yay! ^v^
		return true;
	}

	double averageDayLength() const {
		double totalDayLength = 0;
		FOR_Group(g){
			HourOfDay firstHour, lastHour;
			for(firstHour = 0; GroupHour2Subject[g][firstHour] == Subject_None; firstHour++);
			for(lastHour = NUM_Hours-1; GroupHour2Subject[g][lastHour] == Subject_None; lastHour--);
			totalDayLength += lastHour-firstHour+1; //Last hour is the start of the last subject, so +1 for duration of the last hour
		}
		return totalDayLength / NUM_Groups;
	}

	//Export

	string to_string() const {
		std::string str = "{ ";
		str += "avgDayLength: " + std::to_string(averageDayLength());
		return str + " }";
	}

	json_object* toJSON() const {
		let json = json_object_new_object();
		{
			let gs = json_object_new_array();
			FOR_Group(g){
				let hs = json_object_new_array();
				FOR_Hour(h){
					let sr = json_object_new_object();
					json_object_object_add(sr, "subject", json_object_new_int(GroupHour2Subject[g][h]));
					json_object_object_add(sr, "room", json_object_new_int(GroupHour2Room[g][h]));
					json_object_array_add(hs, sr);
				}
				json_object_array_add(gs, hs);
			}
			json_object_object_add(json, "groups_hours", gs);
		}
		return json;
	}

};

struct IntermediateCost {

	double avgDayLength;

};

typedef EA::Genetic<DayTimetable,IntermediateCost> GA_Type;
typedef EA::GenerationType<DayTimetable,IntermediateCost> Generation_Type;

#define RND_Subject() RAND_Subject(rnd01(), std::floor)
#define RND_Group() RAND_Group(rnd01(), std::floor)
#define RND_Room() RAND_Room(rnd01(), std::floor)
#define RND_Hour() RAND_Hour(rnd01(), std::floor)

#define INIT_MAX_TRIES 1E6
#define INIT_MAX_TRIES_LOCAL (NUM_Hours*NUM_Rooms)
std::function<void(void)> requestStop;
bool userStopRequested = false;

void init_genes(DayTimetable& timetable, const std::function<double(void)> &rnd01){
	long tries = 0;
	retry: if(tries++ == INIT_MAX_TRIES){
		cout << "Could not initialize genes after ludicrous number of tries - solution impossible" << endl;
		requestStop();
		return;
	}
	timetable.init(); //Everything empty
	if(userStopRequested) return;
	bool RoomHour2Occupancy[NUM_Rooms][NUM_Hours] = {};
	bool ProfessorHour2Occupancy[NUM_Professors][NUM_Hours] = {};
	FOR_Group(g){
		FOR_Subject(s){
			let p = GroupSubject2Professor[g][s];
			for(let hn = 0; hn < Subject2Duration[s]; hn++){
				HourOfDay h;
				Room r;
				long ltries = 0;
				while(true){
					if(userStopRequested) return;
					if(ltries++ == INIT_MAX_TRIES_LOCAL) goto retry;
					h = RND_Hour();
					if(timetable.GroupHour2Subject[g][h] != Subject_None) continue;
					if(ProfessorHour2Occupancy[p][h]) continue;
					r = RND_Room();
					if(RoomHour2Occupancy[r][h]) continue;
					break;
				}
				timetable.GroupHour2Subject[g][h] = s;
				timetable.GroupHour2Room[g][h] = r;
				ProfessorHour2Occupancy[p][h] = true;
				RoomHour2Occupancy[r][h] = true;
			}
		}
	}
}

bool eval_solution(const DayTimetable& timetable, IntermediateCost &c){
	if(userStopRequested) return true;
	if(!timetable.isValid()) return false; //Avoid extra computations if solution is invalid
	c.avgDayLength = timetable.averageDayLength();
	return true;
}

#define MUTATION_MAX_TRIES 5E6
#define MAX_NUM_Mutations (NUM_Rooms*NUM_Hours)

DayTimetable mutate(const DayTimetable& X_base, const std::function<double(void)> &rnd01, double shrink_scale){
	if(userStopRequested) return X_base;
	DayTimetable X_new;
	long tries = 0;
	do {
		X_new = X_base;
		if(tries++ >= MUTATION_MAX_TRIES) goto overtried;
		int numMutations = shrink_scale * MAX_NUM_Mutations;
		for(int m = 0; m < numMutations; m++){
			//Next mutation: {swap rooms (between groups), swap subjects (within group), do nothing}
			let nm = rnd01();
			if(nm < 0.5){ //Swap Rooms; This operation guarantees validity conservation
				let h = RND_Hour();
				Room r1, r2;
				r1 = RND_Room();
				do r2 = RND_Room(); while(r1 == r2);
				Group g1 = Group_None, g2 = Group_None;
				FOR_Group(g){
					if(X_new.GroupHour2Room[g][h] == r1) g1 = g;
					if(X_new.GroupHour2Room[g][h] == r2) g2 = g;
					if(g1 != Group_None && g2 != Group_None) break;
				}
				//If both are not none, rooms are swapped; if 1 is not none, its' room is changed to a different empty room; otherwise nothing happens (swap empty rooms)
				if(g2 != Group_None) X_new.GroupHour2Room[g2][h] = r1;
				if(g1 != Group_None) X_new.GroupHour2Room[g1][h] = r2;
			} else if(nm < 0.95){ //Swap Subjects; This operation does not guarantee validity conservation
				let g = RND_Group();
				HourOfDay h1, h2;
				h1 = RND_Hour();
				do h2 = RND_Hour(); while(h1 == h2);
				Subject s1 = X_new.GroupHour2Subject[g][h1], s2 = X_new.GroupHour2Subject[g][h2];
				if(s1 != Subject_None && s2 != Subject_None) X_new.GroupHour2Subject[g][h1] = s2, X_new.GroupHour2Subject[g][h2] = s1; //Both are not none, so swap subjects, keep rooms
				else if(s1 != Subject_None) X_new.GroupHour2Subject[g][h2] = s1, X_new.GroupHour2Room[g][h2] = X_new.GroupHour2Room[g][h1], X_new.GroupHour2Subject[g][h1] = Subject_None, X_new.GroupHour2Room[g][h1] = Room_None; //s2 is none, so move subject and room to h2 (clear h1)
				else if(s2 != Subject_None) X_new.GroupHour2Subject[g][h1] = s2, X_new.GroupHour2Room[g][h1] = X_new.GroupHour2Room[g][h2], X_new.GroupHour2Subject[g][h2] = Subject_None, X_new.GroupHour2Room[g][h2] = Room_None; //s1 is none, so move subject and room to h1 (clear h2)
				//else do nothing (swap empty hours)
			} //Do nothing
		}
	} while(!X_new.isValid());
	return X_new;
	overtried:
	X_new = X_base;
	return X_new;
}

#define CROSSOVER_MAX_TRIES 1E6

DayTimetable crossover(const DayTimetable& X1, const DayTimetable& X2, const std::function<double(void)> &rnd01){
	if(userStopRequested) return X1;
	DayTimetable X_new;
	long tries = 0;
	do {
		X_new = {};
		if(tries++ >= CROSSOVER_MAX_TRIES) goto overtried;
		FOR_Group(g){
			//Take from X1, Take from X2, or sub-cross
			let nc = rnd01();
			if(nc < 0.1) FOR_Hour(h){
				X_new.GroupHour2Room[g][h] = X1.GroupHour2Room[g][h];
				X_new.GroupHour2Subject[g][h] = X1.GroupHour2Subject[g][h];
			} else if(nc < 0.2) FOR_Hour(h){
				X_new.GroupHour2Room[g][h] = X2.GroupHour2Room[g][h];
				X_new.GroupHour2Subject[g][h] = X2.GroupHour2Subject[g][h];
			} else FOR_Hour(h){
				X_new.GroupHour2Room[g][h] = rnd01() < 0.5 ? X1.GroupHour2Room[g][h] : X2.GroupHour2Room[g][h];
				X_new.GroupHour2Subject[g][h] = rnd01() < 0.5 ? X1.GroupHour2Subject[g][h] : X2.GroupHour2Subject[g][h];
			}
		}
	} while(!X_new.isValid());
	//Crossover may not be conform AT ALL!!!
	return X_new;
	overtried:
	cout << "Reached crossover tries limit" << endl;
	X_new = rnd01() < 0.5 ? X1 : X2;
	return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X){
	// finalize the cost
	double final_cost=0.0;
	final_cost+=X.middle_costs.avgDayLength;
	return final_cost;
}

std::ofstream odolt_output_file;
json_object* odolt_best_arr;

void SO_report_generation(int generation_number, const EA::GenerationType<DayTimetable, IntermediateCost> &last_generation, const DayTimetable& best_genes){
	cout
		<<"Generation ["<<generation_number<<"], "
		<<"Best="<<last_generation.best_total_cost<<", "
		<<"Average="<<last_generation.average_cost<<", "
		<<"Best genes=("<<best_genes.to_string()<<")"<<", "
		<<"Exe_time="<<last_generation.exe_time
		<<std::endl;

	odolt_output_file
		<<generation_number<<"\t"
		<<last_generation.average_cost<<"\t"
		<<last_generation.best_total_cost<<"\t"
		<<best_genes.to_string()<<"\n";
	
	if(odolt_best_arr) json_object_array_add(odolt_best_arr, best_genes.toJSON());
}

bool checkHyperParams(){
	{
		int tsd = 0;
		FOR_Subject(s) tsd += Subject2Duration[s];
		if(tsd > NUM_Hours){
			cout << "Total subject duration is more than hours in a day" << endl;
			return false;
		}
		if(tsd * NUM_Groups > NUM_Rooms*NUM_Hours){
			cout << "More group-hours are requested than available room-hours" << endl;
			return false;
		}
	}
	{
		int tph[NUM_Professors] = {};
		FOR_Group(g) FOR_Subject(s) tph[GroupSubject2Professor[g][s]] += Subject2Duration[s];
		FOR_Professor(p) if(tph[p] > NUM_Hours){
			cout << "Total work hours for " << NAMES_Professors[p] << " is more than hours in a day" << endl;
			return false;
		}
	}
	return true;
}

int OneDayOneLTablerRun(){
	if(!checkHyperParams()) return 1;

	odolt_best_arr = json_object_new_array();

	odolt_output_file.open("odoltabler-results.txt");
	odolt_output_file<<"step"<<"\t"<<"cost_avg"<<"\t"<<"cost_best"<<"\t"<<"solution_best"<<"\n";

	EA::Chronometer timer;
	timer.tic();

	GA_Type ga_obj;
	ga_obj.problem_mode=EA::GA_MODE::SOGA;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.dynamic_threading=true;
	ga_obj.verbose=false;
	ga_obj.population=200;
	ga_obj.generation_max=1000;
	ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
	ga_obj.init_genes=init_genes;
	ga_obj.eval_solution=eval_solution;
	ga_obj.mutate=mutate;
	ga_obj.crossover=crossover;
	ga_obj.SO_report_generation=SO_report_generation;
	ga_obj.best_stall_max=5;
	ga_obj.elite_count=50;
	ga_obj.crossover_fraction=0.2;
	ga_obj.mutation_rate=0.6;
	requestStop = [&ga_obj](){
		userStopRequested = true;
		ga_obj.user_request_stop = true;
	};
	ga_obj.solve();

	cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;

	odolt_output_file.close();

	let json = json_object_new_object();
	json_object_object_add(json, "best_of_gens", odolt_best_arr);
	{ let ps = json_object_new_array(); FOR_Professor(p) json_object_array_add(ps, json_object_new_string(NAMES_Professors[p])); json_object_object_add(json, "professors", ps); }
	{ let ss = json_object_new_array(); FOR_Subject(s) json_object_array_add(ss, json_object_new_string(NAMES_Subjects[s])); json_object_object_add(json, "subjects", ss); }
	{ let gs = json_object_new_array(); FOR_Group(g) json_object_array_add(gs, json_object_new_string(NAMES_Groups[g])); json_object_object_add(json, "groups", gs); }
	{ let rs = json_object_new_array(); FOR_Room(r) json_object_array_add(rs, json_object_new_string(NAMES_Rooms[r])); json_object_object_add(json, "rooms", rs); }
	{
		let gs2p = json_object_new_array();
		FOR_Group(g){
			let s2p = json_object_new_array();
			FOR_Subject(s) json_object_array_add(s2p, json_object_new_int(GroupSubject2Professor[g][s]));
			json_object_array_add(gs2p, s2p);
		}
		json_object_object_add(json, "group_subject_professors", gs2p);
	}
	json_object_object_add(json, "hours_in_a_day", json_object_new_int(NUM_Hours));
	json_object_to_file("odoltabler-report.json", json);
	json_object_put(json);

	return 0;
}

}