#pragma once

namespace DebugHelper
{
	inline void PrintFloat3(const XMFLOAT3& val, const std::string& name = "")
	{
		std::cout << name << ": {" << val.x << ", " << val.y << ", " << val.z << "}\n";
	}

	inline void PrintFloat4(const XMFLOAT4& val, const std::string& name = "")
	{
		std::cout << name << ": {" << val.x << ", " << val.y << ", " << val.z << ", " << val.w << "}\n";
	}
}