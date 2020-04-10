#include "Control.h"

Control& Control::operator=(const Control& control)
{
	mLeft = control.mLeft;
	mTop = control.mTop;
	mRight = control.mRight;
	mBottom = control.mBottom;
	mState = control.mState;
	return *this;
}
