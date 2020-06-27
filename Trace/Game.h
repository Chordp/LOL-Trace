#pragma once
class Game
{
private:
	Game() {};
public:
	static auto GetIns()
	{
		static auto _Game = new Game();
		return _Game;
	}

};

