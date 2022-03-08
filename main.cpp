
#include "Interface.hpp"

int main() {
	//Interface configuration
	Interface interface(800,600);

	//Interface update
	try
	{
		interface.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}