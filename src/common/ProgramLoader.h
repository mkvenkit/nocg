#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace nocg {

class ProgramLoader
{
public:

	static unsigned int load(vector<string> shaderFiles);

private:

	static bool getenv(const string& var, string& value);

};

} // namespace nocg