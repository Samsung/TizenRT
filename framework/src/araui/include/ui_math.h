#ifndef __UI_MATH__
#define __UI_MATH__

extern float __cosdeg[720];
extern float __sindeg[720];

#define sind(deg) (__sindeg[((deg) % 360) + 360])
#define cosd(deg) (__cosdeg[((deg) % 360) + 360])

#endif
