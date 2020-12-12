#pragma once

namespace udan::ecs
{
	//template<typename ...Args>
	//using System = std::function<void(Args&& ...args)>;
	typedef std::function<void(...)> System;
	
}
