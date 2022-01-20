#include <vector>
#include <fstream>
#include <sstream>

#include "mhua.h"

using namespace std;


const pair<vector<vector<double>>, vector<vector<double>>> ReadFromFile(const string FILENAME = "input.txt") {
	// Read X array and Y from file where
	// two ints are in the first line: amount of X and Y variables for each next line
	// lines with X and Y measurements to the end
	ifstream in(FILENAME);
	if(in.is_open()) {
		int n_for_x, n_for_y;
		in >> n_for_x >> n_for_y;
		
		vector<vector<double>> X, Y(n_for_y);
		while(!in.eof()) {
			X.push_back({});
			double tmp;
			for(int i=0; i<n_for_x; i++) {
				in >> tmp;
				X.back().push_back(tmp);
			}
			for(int i=0; i<n_for_y; i++) {
				in >> tmp;
				Y.at(i).push_back(tmp);
			}
		}
//		cout<<"Data readed"<<endl;
		in.close();
		return {X, Y};
	}
	else {
		cout<<"Can't open input file"<<endl;
	}
	return {};
}


int main() {
	cout<<"RHR by I.Lupuna"<<endl;
	cout<<"MGUA method"<<endl;
	cout<<endl;
	
	try {
		cout<<"Write filename for input data (first line is 'x y', where 'x' is count of Xn in every line and 'y' is count of Yn in every line"<<endl;
		string filename;
		cout<<"Filename: ";
		cin>> filename;
		const auto [X, Y] = ReadFromFile(filename);
		
		if(X.empty() || Y.empty()) {
			cout<<"No data in file"<<endl;
			return 0;
		}
		cout<<"Data:"<<endl;
		cout<<"Type\tt(mm)\tn(1/hv)\tD(mm)\ts(mm/1)\t |\tPx(H)\tPy(H)\tPz(H)"<<endl;
		for(int i=0; i<X.size(); i++) {
			for(const auto el:X.at(i)) {
				cout<< el <<"\t";
			}
			cout<<" |\t";
			for(int j=0; j<Y.size(); j++) {
				cout<< Y.at(j).at(i) <<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
		
		vector<MGUA> S;
		for(const auto& yi:Y) {
			cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<endl;
			cout<<"Finding Y"<< S.size() <<" optimal model"<<endl;
			S.push_back({X, yi});
			S.back().Start();
		}
		
		for(int i=0; i<S.size(); i++) {
			cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<endl;
			cout<<"Free Check for Y"<< i <<":"<<endl;
			S.at(i).FreeCheck();
		}
	}
	catch (const exception& e) {
		cout<< e.what() <<endl;
	}
	catch (...) {
		cout<<"Idk what is wrong"<<endl;
	}
		
	return 0;
}
