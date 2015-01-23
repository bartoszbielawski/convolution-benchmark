#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <vector>
#include <string>

using sample = double;
using longSample = long double;

using convFun = std::function<longSample (sample*, sample*, int, int)>;

class Implementation
{
	public:
		Implementation(const std::string& name, convFun function, int unrolled=1, bool separated=false, bool forcedLong = false):
		name(name), function(function), unrolled(unrolled), separated(separated), forcedLong(forcedLong) {}
		std::string		name;
		convFun 		function;
		int 			unrolled;
		bool			separated;
		bool			forcedLong; 
};

class Implementations
{
	public:
		static Implementations& getInstance() {static Implementations impls; return impls;}
		void addImplementation(const Implementation& i) {implementations.push_back(i);}
		const std::vector<Implementation>& getImplementations() {return implementations;}
 
	private:
		std::vector<Implementation> implementations;
};


class RegisterImplementation
{
	public:
		RegisterImplementation(const std::string& name, convFun function, int unrolled=1, bool separated=false, bool forcedLong = false)
		{
			Implementations::getInstance().addImplementation(Implementation(name, function, unrolled, separated, forcedLong));
		}
};


#endif //TYPES_H