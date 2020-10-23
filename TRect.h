#ifndef BRect_h
#define BRect_h

template <class T>
class TRect
{
public:
	T left, top, right, bottom;

	TRect()
	:left(0), top(0), right(0), bottom(0)
	{}

	TRect(T l, T t, T r, T b)
	:left(l), top(t), right(r), bottom(b)
	{}

	template <class U>
	TRect& operator=(const TRect<U> &aTru)
	{
		left = aTru.left; top = aTru.top; 
		right = aTru.right; bottom = aTru.bottom; 

		return *this;
	}

	inline T Width() const { return right - left; }
	inline T Height() const { return bottom - top; }
};

//class BRect : public TRect<float>
//{
//};

typedef TRect<float> FRect;
typedef TRect<int> IRect;
// typedef TRect<float> BRect;

//class BRect
//{
//public:
//	float left, top, right, bottom;
//	BRect(float l, float t, float r, float b)
//	:left(l), top(t), right(r), bottom(b)
//	{}
//
//	inline float Width() const { return right - left; }
//	inline float Height() const { return bottom - top; }
//};

template <class T>
TRect<T> ResizeRectPerimeter(const TRect<T> &aBr, const T &aTPerimeter)
{
	// Return a resized rect with perimeter = aLPerimeter, centered around (0, 0)

	T lFOldPerimeter = (aBr.Width() + aBr.Height()) * 2;
	
	float lFRatio = aTPerimeter;
	lFRatio /= lFOldPerimeter;

	T lLNewX = (aBr.Width() * lFRatio) / 2;
	T lLNewY = (aBr.Height() * lFRatio) / 2;

	TRect<T> lTrResult(-lLNewX, -lLNewY, lLNewX, lLNewY);
	
	return lTrResult;
}


#endif
