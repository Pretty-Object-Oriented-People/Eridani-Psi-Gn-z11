// main.cpp

#include <string>
#include <iostream>
#include <fstream>
#include <openga/openga.hpp>

#include <util/Macros.h>

#include <json-c/json.h>

using std::string;
using std::cout;
using std::endl;

const int BOXC = 7;

const double boxSX[BOXC] = {5, 1, 3, 2, 2, 3, 1}; 
const double boxSY[BOXC] = {5, 7, 4, 2, 3, 2, 3};

const double containerW = 10, containerH = 10;

struct MySolution {
	
	double boxPX[BOXC];
	double boxPY[BOXC];

	string to_string() const
	{
		double minX, minY, maxX, maxY;
		getBB(minX, minY, maxX, maxY);
		std::string str = "{ ";
		str += "aabb: (" + std::to_string(minX) + ", " + std::to_string(minY) + ") -> (" + std::to_string(maxX) + ", " + std::to_string(maxY) + ")";
		str += ", boxes: [";
		for(let i = 0; i < BOXC; i++) str.append((i > 0 ? ", (" : "(") + std::to_string(boxPX[i] - minX) + "," + std::to_string(boxPY[i] - minY) + ")");
		return str + "] }";
	}

	json_object* toJSON() const {
		double minX, minY, maxX, maxY;
		getBB(minX, minY, maxX, maxY);
		let json = json_object_new_object();
		{
			let area = json_object_new_object();
			json_object_object_add(area, "x", json_object_new_double(maxX - minX));
			json_object_object_add(area, "y", json_object_new_double(maxY - minY));
			json_object_object_add(json, "area", area);
		}
		{
			let boxes = json_object_new_array();
			for(let i = 0; i < BOXC; i++){
				let boxJ = json_object_new_object();
				json_object_object_add(boxJ, "x", json_object_new_double(boxPX[i] - minX));
				json_object_object_add(boxJ, "y", json_object_new_double(boxPY[i] - minY));
				json_object_array_add(boxes, boxJ);
			}
			json_object_object_add(json, "boxes", boxes);
		}
		return json;
	}

	#define intervalNotOvelap(x1, sx1, x2, sx2) (x2+sx2 < x1 || x1+sx1 < x2)
	#define bbNotOverlap(x1, y1, sx1, sy1, x2, y2, sx2, sy2) (intervalNotOvelap(x1, sx1, x2, sx2) || intervalNotOvelap(y1, sy1, y2, sy2)) 
	#define bbOverlap(b1i, b2i) !(bbNotOverlap(boxPX[b1i], boxPY[b1i], boxSX[b1i], boxSY[b1i], boxPX[b2i], boxPY[b2i], boxSX[b2i], boxSY[b2i]))

	void getBB(double& minX, double& minY, double& maxX, double& maxY) const {
		minX = maxX = boxPX[0];
		minY = maxY = boxPY[0];
		for(let i = 0; i < BOXC; i++){
			minX = min(minX, boxPX[i]);
			minY = min(minY, boxPY[i]);
			maxX = max(maxX, boxPX[i] + boxSX[i]);
			maxY = max(maxY, boxPY[i] + boxSY[i]);
		}
	}

	bool checkValid() const {
		/*double minX, minY, maxX, maxY;
		getBB(minX, minY, maxX, maxY);
		if(minX < 0 || maxX > containerW || minY < 0 || maxY > containerH) return false;*/
		for(let i = 0; i < BOXC; i++) for(let j = i+1; j < BOXC; j++) if(bbOverlap(i, j)) return false;
		return true;
	}

	double bbArea() const {
		double minX, minY, maxX, maxY;
		getBB(minX, minY, maxX, maxY);
		return (maxX-minX)*(maxY-minY);
	}

};

struct MyMiddleCost {
	// This is where the results of simulation
	// is stored but not yet finalized.
	double areaBB;
};

typedef EA::Genetic<MySolution,MyMiddleCost> GA_Type;
typedef EA::GenerationType<MySolution,MyMiddleCost> Generation_Type;

void init_genes(MySolution& p,const std::function<double(void)> &rnd01){
	// rnd01() gives a random number in 0~1
	for(let i = 0; i < BOXC; i++){
		p.boxPX[i] = 0.0+containerW*rnd01();
		p.boxPY[i] = 0.0+containerH*rnd01();
	}
}

bool eval_solution(const MySolution& p, MyMiddleCost &c){
	c.areaBB = p.bbArea();
	return p.checkValid();
}

MySolution mutate(const MySolution& X_base, const std::function<double(void)> &rnd01, double shrink_scale){
	MySolution X_new;
	// bool in_range;
	// do{
		// in_range=true;
		X_new=X_base;
		for(let i = 0; i < BOXC; i++){
			X_new.boxPX[i] += (rnd01()-rnd01())*containerW*shrink_scale;
			X_new.boxPY[i] += (rnd01()-rnd01())*containerH*shrink_scale;
			// in_range &= X_new.boxPX[i] >= 0 && X_new.boxPX[i] < containerW && X_new.boxPY[i] >= 0 && X_new.boxPY[i] < containerH;
		}
	// } while(!in_range);
	return X_new;
}

MySolution crossover(const MySolution& X1, const MySolution& X2, const std::function<double(void)> &rnd01){
	MySolution X_new;
	for(let i = 0; i < BOXC; i++){
		let rX = rnd01();
		X_new.boxPX[i] = rX*X1.boxPX[i] + (1.0-rX)*X2.boxPX[i];
		let rY = rnd01();
		X_new.boxPY[i] = rY*X1.boxPY[i] + (1.0-rY)*X2.boxPY[i];
	}
	return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X){
	// finalize the cost
	double final_cost=0.0;
	final_cost+=X.middle_costs.areaBB;
	return final_cost;
}

std::ofstream output_file;
json_object* best_arr;

void SO_report_generation(int generation_number, const EA::GenerationType<MySolution,MyMiddleCost> &last_generation, const MySolution& best_genes){
	cout
		<<"Generation ["<<generation_number<<"], "
		<<"Best="<<last_generation.best_total_cost<<", "
		<<"Average="<<last_generation.average_cost<<", "
		<<"Best genes=("<<best_genes.to_string()<<")"<<", "
		<<"Exe_time="<<last_generation.exe_time
		<<endl;

	output_file
		<<generation_number<<"\t"
		<<last_generation.average_cost<<"\t"
		<<last_generation.best_total_cost<<"\t"
		<<best_genes.to_string()<<"\n";
	
	if(best_arr) json_object_array_add(best_arr, best_genes.toJSON());
}

int main(){
	best_arr = json_object_new_array();

	output_file.open("results.txt");
	output_file<<"step"<<"\t"<<"cost_avg"<<"\t"<<"cost_best"<<"\t"<<"solution_best"<<"\n";

	EA::Chronometer timer;
	timer.tic();

	GA_Type ga_obj;
	ga_obj.problem_mode=EA::GA_MODE::SOGA;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.dynamic_threading=true;
	ga_obj.verbose=false;
	ga_obj.population=10000;
	ga_obj.generation_max=1000;
	ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
	ga_obj.init_genes=init_genes;
	ga_obj.eval_solution=eval_solution;
	ga_obj.mutate=mutate;
	ga_obj.crossover=crossover;
	ga_obj.SO_report_generation=SO_report_generation;
	ga_obj.best_stall_max=10;
	ga_obj.elite_count=50;
	ga_obj.crossover_fraction=0.8;
	ga_obj.mutation_rate=0.95;
	ga_obj.solve();

	cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<endl;

	output_file.close();

	let json = json_object_new_object();
	json_object_object_add(json, "best_positions", best_arr);
	{
		let boxes = json_object_new_array();
		for(let i = 0; i < BOXC; i++){
			let box = json_object_new_object();
			json_object_object_add(box, "x", json_object_new_double(boxSX[i]));
			json_object_object_add(box, "y", json_object_new_double(boxSY[i]));
			json_object_array_add(boxes, box);
		}
		json_object_object_add(json, "boxes", boxes);
	}
	{
		let container = json_object_new_object();
		json_object_object_add(container, "x", json_object_new_double(containerW));
		json_object_object_add(container, "y", json_object_new_double(containerH));
		json_object_object_add(json, "container", container);
	}
	json_object_to_file("report.json", json);
	json_object_put(json);

	return 0;
}