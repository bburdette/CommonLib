#include "StringUtils.h"

std::string::size_type replace_all(std::string& s,
							const std::string& from,
							const std::string& to)
{
	std::string::size_type cnt(std::string::npos);

	if(from != to && !from.empty())
	{
		std::string::size_type pos1(0);
		std::string::size_type pos2(0);
		const std::string::size_type from_len(from.size());
		const std::string::size_type to_len(to.size());
		cnt = 0;

		while((pos1 = s.find(from, pos2)) != std::string::npos)
		{
			s.replace(pos1, from_len, to);
			pos2 = pos1 + to_len;
			++cnt;
		}
	}

	return cnt;
}
