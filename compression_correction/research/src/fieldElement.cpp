#include "fieldElement.hpp"

template <int p>
fieldElement<p>::fieldElement() : val(0) {}

template <int p>
fieldElement<p>::fieldElement(int v) : val(v) {}
	
template <int p>
fieldElement<p> fieldElement<p>::operator+(const fieldElement<p> rhs) const
{
	return fieldElement<p>(val^rhs.val);
}

template <int p>
fieldElement<p> fieldElement<p>::operator-(const fieldElement<p> rhs) const
{
	return *this + rhs;
}

template <int p>
fieldElement<p> fieldElement<p>::operator*(const fieldElement<p> rhs) const
{
	return fieldElement<p>((val*rhs.val)%p);
}

template <int p>
fieldElement<p> fieldElement<p>::operator/(const fieldElement<p> rhs) const
{
	return fieldElement<p>((val/rhs.val)%p);
}

template <int p>
fieldElement<p> fieldElement<p>::operator=(const fieldElement<p> rhs)
{
	val = rhs.val;
	return *this;
}

template <int p>
fieldElement<p> fieldElement<p>::operator=(const int v)
{
	v = val;
	return *this;
}