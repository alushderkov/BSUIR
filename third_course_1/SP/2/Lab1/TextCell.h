#pragma once
#include "Cell.h"
#include <string>
class TextCell : public Cell
{
private:
	std::wstring _cellText;
public:
	void DrawCell() override;
};

