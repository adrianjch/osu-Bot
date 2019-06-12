/*#include <iostream>
#include <fstream>


int main() {
	std::rename("hhWhV09", "hhWhV09.png");

	std::remove("hhWhV09.png");

	std::ofstream myFile("hhWhV09");
	myFile.close();

	std::ifstream in("from.txt", std::ios::in | std::ios::binary);
	std::ofstream out("to.txt", std::ios::out | std::ios::binary);
	out << in.rdbuf();
	std::remove("from.txt");

	return 0;
}*/