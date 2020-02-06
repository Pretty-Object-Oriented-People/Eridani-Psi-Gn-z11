// main.cpp

#include <string>
#include <iostream>
#include <fstream>
#include <openga/openga.hpp>

#include <util/Macros.h>

using std::string;
using std::cout;
using std::endl;

const double box1SX = 5;
const double box1SY = 5;
const double box2SX = 1;
const double box2SY = 7;
const double box3SX = 3;
const double box3SY = 4;

struct MySolution {
	double box1X;
	double box1Y;
	double box2X;
	double box2Y;
	double box3X;
	double box3Y;

#define intervalNotOvelap(x1, sx1, x2, sx2) (x1 < x2+sx2 || x2 < x1+sx1)
#define bbNotOverlap(x1, y1, sx1, sy1, x2, y2, sx2, sy2) (intervalNotOvelap(x1, sx1, x2, sx2) || intervalNotOvelap(y1, sy1, y2, sy2)) 

	bool checkOverlap(){
		return bbNotOverlap(box1X, box1SX, box1Y, box1SY, box2X, box2SX, box2Y, box2SY) && bbNotOverlap(box1X, box1SX, box1Y, box1SY, box3X, box3SX, box3Y, box3SY) && bbNotOverlap(box2X, box2SX, box2Y, box2SY, box3X, box3SX, box3Y, box3SY);
	}

	string to_string() const
	{
		return 
			string("{")
			+  "box1X:"+std::to_string(box1X)
			+", box1Y:"+std::to_string(box1Y)
			+", box2X:"+std::to_string(box2X)
			+", box2Y:"+std::to_string(box2Y)
			+", box3X:"+std::to_string(box3X)
			+", box3Y:"+std::to_string(box3Y)
			+"}";
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
	p.box1X=0.0+10*rnd01();
	p.box1Y=0.0+10*rnd01();
	p.box2X=0.0+10*rnd01();
	p.box2Y=0.0+10*rnd01();
	p.box3X=0.0+10*rnd01();
	p.box3Y=0.0+10*rnd01();
}

bool eval_solution(const MySolution& p, MyMiddleCost &c){
	const double& box1X=p.box1X;
	const double& box1Y=p.box1Y;
	const double& box2X=p.box2X;
	const double& box2Y=p.box2Y;
	const double& box3X=p.box3X;
	const double& box3Y=p.box3Y;

	c.areaBB=(max(box1X+box1SX, max(box2X+box2SX, box3X+box3SX)) - min(box1X, min(box2X, box3X))) * (max(box1Y+box1SY, max(box2Y+box2SY, box3Y+box3SY)) - min(box1Y, min(box2Y, box3Y)));
	return true; // solution is accepted
}

MySolution mutate(const MySolution& X_base, const std::function<double(void)> &rnd01, double shrink_scale){
	MySolution X_new;
	bool in_range;
	do{
		in_range=true;
		X_new=X_base;
		X_new.box1X+=0.2*(rnd01()-rnd01())*shrink_scale;
		in_range=in_range&&(X_new.box1X>=0.0 && X_new.box1X<10.0);
		X_new.box1Y+=0.2*(rnd01()-rnd01())*shrink_scale;
		in_range=in_range&&(X_new.box1Y>=0.0 && X_new.box1Y<10.0);
		X_new.box2X+=0.2*(rnd01()-rnd01())*shrink_scale;
		in_range=in_range&&(X_new.box2X>=0.0 && X_new.box2X<10.0);
		X_new.box2Y+=0.2*(rnd01()-rnd01())*shrink_scale;
		in_range=in_range&&(X_new.box2Y>=0.0 && X_new.box2Y<10.0);
		X_new.box3X+=0.2*(rnd01()-rnd01())*shrink_scale;
		in_range=in_range&&(X_new.box3X>=0.0 && X_new.box3X<10.0);
		X_new.box3Y+=0.2*(rnd01()-rnd01())*shrink_scale;
		in_range=in_range&&(X_new.box3Y>=0.0 && X_new.box3Y<10.0);
	} while(!in_range);
	return X_new;
}

MySolution crossover(const MySolution& X1, const MySolution& X2, const std::function<double(void)> &rnd01){
	MySolution X_new;
	double r;
	r=rnd01();
	X_new.box1X=r*X1.box1X+(1.0-r)*X2.box1X;
	r=rnd01();
	X_new.box1Y=r*X1.box1Y+(1.0-r)*X2.box1Y;
	r=rnd01();
	X_new.box2X=r*X1.box2X+(1.0-r)*X2.box2X;
	r=rnd01();
	X_new.box2Y=r*X1.box2Y+(1.0-r)*X2.box2Y;
	r=rnd01();
	X_new.box3X=r*X1.box3X+(1.0-r)*X2.box3X;
	r=rnd01();
	X_new.box3Y=r*X1.box3Y+(1.0-r)*X2.box3Y;
	return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X){
	// finalize the cost
	double final_cost=0.0;
	final_cost+=X.middle_costs.areaBB;
	return final_cost;
}

std::ofstream output_file;

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
}

int main(){
	output_file.open("results.txt");
	output_file<<"step"<<"\t"<<"cost_avg"<<"\t"<<"cost_best"<<"\t"<<"solution_best"<<"\n";

	EA::Chronometer timer;
	timer.tic();

	GA_Type ga_obj;
	ga_obj.problem_mode=EA::GA_MODE::SOGA;
	ga_obj.multi_threading=false;
	ga_obj.verbose=false;
	ga_obj.population=200;
	ga_obj.generation_max=1000;
	ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
	ga_obj.init_genes=init_genes;
	ga_obj.eval_solution=eval_solution;
	ga_obj.mutate=mutate;
	ga_obj.crossover=crossover;
	ga_obj.SO_report_generation=SO_report_generation;
	ga_obj.best_stall_max=10;
	ga_obj.elite_count=10;
	ga_obj.crossover_fraction=0.7;
	ga_obj.mutation_rate=0.2;
	ga_obj.best_stall_max=10;
	ga_obj.elite_count=10;
	ga_obj.solve();

	cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<endl;

	output_file.close();
	return 0;
}