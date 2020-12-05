#pragma once

namespace udan::core
{
	class IClock
	{
	public:
		virtual ~IClock() = default;
		virtual float Tick() = 0;
	};
}
