#include "MyColor.h"

CHSV RGB2HSV(const CRGB& rgb)
{
	int h=0,s=0,v=0;
	int r=rgb.GetR();
	int g=rgb.GetG();
	int b=rgb.GetB();
	int mx=max(max(r,g),b);
	int mn=min(min(r,g),b);
	int d=mx-mn;
	v=mx;
	if (mx==0){
		s=0;
		h=0;
	}else{
		s=255*d/mx;

		if(d==0){
			h=0;
		}else if(r==mx){
			h=60*(g-b)/d;
		}else if(g==mx){
			h=120+60*(b-r)/d;
		}else{
			h=240+60*(r-g)/d;
		}	
	}
	return CHSV(h,s,v);

}

CRGB HSV2RGB(const CHSV& hsv)
{
	unsigned char c1,c2,c3;
	if(hsv.GetS()==0){
		return CRGB(hsv.GetV(),hsv.GetV(),hsv.GetV());
	}else{
		int t=(hsv.GetH()*6)%360;
		c1=(hsv.GetV()*(255-hsv.GetS()))/255;
		c2=(hsv.GetV()*(255-(hsv.GetS()*t)/360))/255;
		c3=(hsv.GetV()*(255-(hsv.GetS()*(360-t))/360))/255;
		switch (hsv.GetH()/60) {
		case 0: return CRGB(hsv.GetV(),c3,c1);break;
		case 1: return CRGB(c2,hsv.GetV(),c1);break;
		case 2: return CRGB(c1,hsv.GetV(),c3);break;
		case 3: return CRGB(c1,c2,hsv.GetV());break;
		case 4: return CRGB(c3,c1,hsv.GetV());break;
		case 5:	default: return CRGB(hsv.GetV(),c1,c2);break;
		}
	}
}
