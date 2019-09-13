#include <iostream>
#include <vector>

struct flow {
	int input;
	int output;
};
int*** matrix;
void exec_way(std::vector<int> way);
bool approve_way(std::vector<int> way);
int get_min_of_way(std::vector<int> way);
int get_road(int from, int to);
bool iterate_ways(std::vector<std::vector<int>> &ways, int to, int N);
void find_max(int from, int to, int N);
void input_matrix(int n);
flow get_point_flow(int point, int N);


int main() {
	int n;
	{
		std::cin>>n;
		matrix=new int**[2];
	}
	input_matrix(n);
	find_max(0,1,n);
	flow flow = get_point_flow(0,n);
	std::cout<<flow.output;
}

void input_matrix(int n) {
	matrix[0]=new int*[n];
	matrix[1]=new int*[n];
	for(int i=0;i<n;i++) {
		matrix[0][i]=new int[n];
		matrix[1][i]=new int[n];
		for(int j=0;j<n;j++) {
			std::cin>>matrix[0][i][j];
			matrix[1][i][j]=0;
		}
	}
}

int get_road(int from, int to) {
	return matrix[0][from][to];
}

int get_min_of_way(std::vector<int> way) {
	int min = get_road(way.at(0),way.at(1));
	for (int i=0;i<way.size()-1;i++) {
		if (get_road(way.at(i),way.at(i+1))<min) {
			min = get_road(way.at(i),way.at(i+1));
		}
	}
	return min;
}

bool approve_way(std::vector<int> way) {
    if (way.size()==1) return true;
	return get_min_of_way(way)>0;
}

void exec_way(std::vector<int> way) {
	if (!approve_way(way)) return;
	int flow = get_min_of_way(way);
	for (int i=0;i<way.size()-1;i++) {
		matrix[0][way.at(i)][way.at(i+1)]-=flow;
		matrix[0][way.at(i+1)][way.at(i)]+=flow;
		matrix[1][way.at(i)][way.at(i+1)]+=flow;
	}
}

flow get_point_flow(int point, int n) {
	flow result = {0,0};
	for (int i=0;i<n;i++) {
		result.input+=matrix[1][i][point];
        result.output+=matrix[1][point][i];
	}
	return result;
}

void find_max(int from, int to, int N) {
	std::vector<std::vector<int> > ways;
	{
		std::vector<int> _init;
		_init.push_back(from);
		ways.push_back(_init);
	}
	while (iterate_ways(ways,to,N)) {
        std::vector<int> _init;
        _init.push_back(from);
        ways.clear();
        ways.push_back(_init);
    }
}

bool iterate_ways(std::vector<std::vector<int>> &ways, int to, int N) {
    bool nodes[N];
    for (int i=0;i<N;i++) nodes[i]=true;
    nodes[ways.back().back()]=false;
    while (!ways.empty()) {
        std::vector<int> current_way;
        current_way = ways.back();
		ways.pop_back();
		if (!approve_way(current_way)) continue;
		std::vector<int> duplicate;
		std::vector<int> possibilities;
		possibilities.clear();
		for (int i=0;i<N;i++) {
            if ((get_road(current_way.back(),i) > 0) && nodes[i]) { possibilities.push_back(i); nodes[i]=false;}
		}
		for (int i=0;i<possibilities.size();i++) {
			bool flag = false;
			for (int j=0;j<current_way.size();j++) if (possibilities.at(i)==current_way.at(j)) flag=true;
			if (flag) continue;
			duplicate=current_way;
			duplicate.push_back(possibilities.at(i));
			if (duplicate.back()==to) {
				exec_way(duplicate);
				return true;
			}
            if (ways.empty()) {
                ways.push_back(duplicate);
            } else {
                ways.insert(ways.begin(),duplicate);
            }
		}
	}
    return !ways.empty();
}
