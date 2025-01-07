#include "ScoreCounter.h"

ScoreCounter::~ScoreCounter()
{
}

void ScoreCounter::OnNotifyScoreChange(int score)
{
	this->score += score;
	SetText("Score: " + std::to_string(this->score));
}
