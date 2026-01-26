#pragma once

#include <windowSystemm/window.h>
#include <sushi/sushiInput.h>

namespace enjinn
{
	::sushi::SushiInput toSushi(const enjinn::Input &in);
	::sushi::SushiController toSushi(const enjinn::Controller &in);
	::sushi::SushiButton toSushi(const enjinn::Button &in);

};

