#ifndef INT3_H
#define INT3_H

class int3
{
public:
	int x,y,z;
	inline int3():x(0),y(0),z(0){}; //c-tor, x/y/z initialized to 0
	inline int3(const int & X, const int & Y, const int & Z):x(X),y(Y),z(Z){}; //c-tor
	inline ~int3(){} // d-tor - does nothing
	inline int3 operator+(const int3 & i) const
		{return int3(x+i.x,y+i.y,z+i.z);}
	inline int3 operator+(const int i) const //increases all components by int
		{return int3(x+i,y+i,z+i);}
	inline int3 operator-(const int3 & i) const
		{return int3(x-i.x,y-i.y,z-i.z);}
	inline int3 operator-(const int i) const
		{return int3(x-i,y-i,z-i);}
	inline int3 operator-() const //increases all components by int
		{return int3(-x,-y,-z);}
	inline void operator+=(const int3 & i)
	{
		x+=i.x;
		y+=i.y;
		z+=i.z;
	}	
	inline void operator+=(const int & i)
	{
		x+=i;
		y+=i;
		z+=i;
	}
	inline void operator-=(const int3 & i)
	{
		x-=i.x;
		y-=i.y;
		z-=i.z;
	}	
	inline void operator-=(const int & i)
	{
		x+=i;
		y+=i;
		z+=i;
	}	
	inline bool operator==(const int3 & i) const
		{return (x==i.x) && (y==i.y) && (z==i.z);}	
	inline bool operator!=(const int3 & i) const
		{return !(*this==i);}
	inline bool operator<(const int3 & i) const
	{
		if (z<i.z)
			return true;
		if (z>i.z)
			return false;
		if (y<i.y)
			return true;
		if (y>i.y)
			return false;
		if (x<i.x)
			return true;
		if (x>i.x)
			return false;
		return false;
	}
};
inline std::istream & operator>>(std::istream & str, int3 & dest)
{
	str>>dest.x>>dest.y>>dest.z;
	return str;
}
inline std::ostream & operator<<(std::ostream & str, int3 & sth)
{
	return str<<sth.x<<' '<<sth.y<<' '<<sth.z;
}
#endif //INT3_H