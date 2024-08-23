#include "Application.h"


#include <iostream>


int main() {
	auto* app = new Application();
	app->run();
	delete app;

	std::cout << "Hello, World!" << std::endl;
	return 0;
}
