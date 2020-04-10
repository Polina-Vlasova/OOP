#include "Label.h"

Label & Label::operator=(const Label & label)
{
	mLegend = label.mLegend;
	mMultiLine = label.mMultiLine;
	return *this;
}

void Label::draw(HDC& hDC)
{
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, Menu::sTheme == Menu::Theme::Dark ? RGB(210, 210, 210) : RGB(90, 90, 90));
	UINT uFormat = DT_VCENTER;
	if (!mMultiLine)
		uFormat |= DT_SINGLELINE; 
	DrawText(hDC, Menu::s2ws(mLegend).c_str(), mLegend.size(), &Rect(), uFormat);
}
