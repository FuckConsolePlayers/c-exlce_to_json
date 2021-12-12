#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <nlohmann/json.hpp>
std::vector<std::string> separate(std::string s,char separator) {
	std::vector<std::string> vec;
	std::string temp = "";
	for (char ch : s) {
		if (ch != separator) {
			temp = temp + ch;
		}
		else {
			vec.push_back(temp);
			temp = "";
		}
	}
	return vec;
}
void work(bool& first,std::string line, std::vector<std::string>& names, std::fstream& fil) {
	if (first == false) {
		std::vector<std::string> vec = separate(line, ',');
		int count = 0;
		nlohmann::json j{};
		for (std::string separated : vec) {
			if (separated[0] != '"') {
				separated.erase(remove(separated.begin(), separated.end(), ' '), separated.end());
				try {
					j[names[count]] = stoi(separated);
				}
				catch (const std::exception& ex) {
					separated.erase(0, 2);
					j[names[count]] = separated;
				}
			}
			else {
				j[names[count]] = separated.substr(1, separated.size() - 1);
			}
			count++;
		}
		if (j != nlohmann::detail::value_t::null) {
			fil << j;
			if (count != vec.size()-1) {
				fil << ",\n";
			}
			else {
				fil << "\n";
			}
		}
	}
	else {
		names = separate(line, ',');
	}
	first = false;
}

int main() {
	std::fstream fil;
	fil.open("excle_table.json", std::ios_base::app | std::ios_base::in);

	std::ifstream file;
	file.open("C:\\Users\\User\\source\\repos\\capp\\data2.csv");
	bool first = true;
	std::vector<std::string> names;
	fil << '[';
	while (file.good()) {
		std::string line;
		getline(file, line);
		work(first,line,names,fil);
		//std::thread th = std::thread(work, first, maps, line, names);
		//th.join();
	}
	fil << ']';
	return 0;
}