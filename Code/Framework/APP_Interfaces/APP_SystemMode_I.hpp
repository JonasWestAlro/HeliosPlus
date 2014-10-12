#pragma once
#include "Framework.hpp"

struct APP_SystemMode_I : APP_Interface<APP_SystemMode_I>{
	using APP_Interface::APP_Interface;

	uint8_t  mode;
	uint8_t  state;
};
