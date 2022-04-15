#ifndef GFELEM
#define GFELEM

template <int p>
class fieldElement
{
	private:
		int val;
		
	public:
		fieldElement() : val(0) {}
		fieldElement(int v) : val(v) {}
	
		fieldElement<p> operator+(const fieldElement<p> rhs) const { return fieldElement<p>(val^rhs.val); }
		fieldElement<p> operator-(const fieldElement<p> rhs) const { return *this + rhs; }
		fieldElement<p> operator*(const fieldElement<p> rhs) const { return fieldElement<p>((val*rhs.val)%p); }
		fieldElement<p> operator/(const fieldElement<p> rhs) const { return fieldElement<p>((val/rhs.val)%p); }
		
		fieldElement<p> operator=(const fieldElement<p> rhs) { val = rhs.val; }
		fieldElement<p> operator=(const int v) { val = v; }
		
		int val() const { return val; }
};

#endif