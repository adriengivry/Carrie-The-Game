#ifndef __VECTOR2D__
#define __VECTOR2D__

#define _USE_MATH_DEFINES

#include <SFML/System/Vector2.hpp>
#include <math.h>

namespace AGMath
{
	enum InitMode : bool
	{
		CARTESIAN = true,
		POLAR = false,
	};

	template<class T>
	class Vector2D
	{
	public:
		Vector2D();
		Vector2D(const T p_data1, const T p_data2, const bool p_initMode = InitMode::CARTESIAN);
		Vector2D(const Vector2D<T>& p_toCopy);
		Vector2D<T> operator=(const Vector2D<T>& p_toCopy);
		~Vector2D() {}

		void Set(const T p_data1, const T p_data2, const bool p_initMode = InitMode::CARTESIAN);

		void InitCartesian(const T p_x, const T p_y);
		void InitPolar(const T p_length, const T p_angle);

		const T& X() const { return __x; }
		const T& Y() const { return __y; }

		void X(const T p_data) { __x = p_data; }
		void Y(const T p_data) { __y = p_data; }

		T& X() { return __x; }
		T& Y() { return __y; }

		Vector2D<T> operator+(const Vector2D<T>& p_toAdd);
		Vector2D<T> operator+=(const Vector2D<T>& p_toAdd);
		Vector2D<T> operator-(const Vector2D<T>& p_toSub);
		Vector2D<T> operator-=(const Vector2D<T>& p_toSub);
		Vector2D<T> operator*(const T& p_coefficient);

		friend std::ostream& operator<<(std::ostream& p_stream, const Vector2D<T>& p_toDisplay)
		{
			p_stream << "(" << p_toDisplay.X() << "," << p_toDisplay.Y() << ")" << std::endl;
			return p_stream;
		}

		Vector2D<T> Add(const Vector2D<T>& p_toAdd);
		Vector2D<T> Sub(const Vector2D<T>& p_toSub);
		void Move(const Vector2D<T>& p_toAdd);
		float DotProduct(const Vector2D<T>& p_dotProductSecondOperand) const;
		float CrossProduct(const Vector2D<T>& p_crossProductSecondOperand) const;
		float DistanceTo(const Vector2D<T>& p_otherVector) const;

		float AngleTo(const Vector2D<T>& p_otherVector) const;

		float GetMagnitude() const;
		float GetAngle() const;

		void Normalize();

		bool IsNormalized() const;
		bool IsNull() const;
		bool IsOrthogonalTo(const Vector2D<T>& p_otherVector) const;
		bool IsParallelTo(const Vector2D<T>& p_otherVector) const;

		sf::Vector2<T> ToSFVector() const;
	private:
		T __x;
		T __y;
	};

	template <class T>
	Vector2D<T>::Vector2D()
	{
		__x = 0;
		__y = 0;
	}

	template <class T>
	Vector2D<T>::Vector2D(const T p_data1, const T p_data2, const bool p_initMode)
	{
		Set(p_data1, p_data2, p_initMode);
	}

	template <class T>
	Vector2D<T>::Vector2D(const Vector2D<T>& p_toCopy) :
		__x(p_toCopy.X()),
		__y(p_toCopy.Y()) {}

	template <class T>
	Vector2D<T> Vector2D<T>::operator=(const Vector2D<T>& p_toCopy)
	{
		Vector2D<T> temp(__x, __y);
		__x = p_toCopy.X();
		__y = p_toCopy.Y();
		return temp;
	}

	template <class T>
	void Vector2D<T>::Set(const T p_data1, const T p_data2, const bool p_initMode)
	{
		if (p_initMode == InitMode::CARTESIAN)
			InitCartesian(p_data1, p_data2);
		else if (p_initMode == InitMode::POLAR)
			InitPolar(p_data1, p_data2);
	}

	template <class T>
	void Vector2D<T>::InitCartesian(const T p_x, const T p_y)
	{
		__x = p_x;
		__y = p_y;
	}

	template <class T>
	void Vector2D<T>::InitPolar(const T p_length, const T p_angle)
	{
		__x = p_length * cos(p_angle * M_PI / 180);
		__y = p_length * sin(p_angle * M_PI / 180);
	}

	template <class T>
	Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& p_toAdd)
	{
		return Add(p_toAdd);
	}

	template <class T>
	Vector2D<T> Vector2D<T>::operator+=(const Vector2D<T>& p_toAdd)
	{
		Vector2D<T> temp(__x, __y);
		Move(p_toAdd);
		return temp;
	}

	template <class T>
	Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& p_toSub)
	{
		return Sub(p_toSub);
	}

	template <class T>
	Vector2D<T> Vector2D<T>::operator-=(const Vector2D<T>& p_toSub)
	{
		Vector2D<T> temp(__x, __y);
		Move(p_toSub);
		return temp;
	}

	template <class T>
	Vector2D<T> Vector2D<T>::operator*(const T& p_coefficient)
	{
		Vector2D<T> toReturn;
		toReturn.X(__x * p_coefficient);
		toReturn.Y(__y * p_coefficient);
		return toReturn;
	}

	template <class T>
	Vector2D<T> Vector2D<T>::Add(const Vector2D<T>& p_toAdd)
	{
		Vector2D<T> toReturn;
		toReturn.X(__x + p_toAdd.X());
		toReturn.Y(__y + p_toAdd.Y());
		return toReturn;
	}

	template <class T>
	Vector2D<T> Vector2D<T>::Sub(const Vector2D<T>& p_toAdd)
	{
		Vector2D<T> toReturn;
		toReturn.X(__x - p_toAdd.X());
		toReturn.Y(__y - p_toAdd.Y());
		return toReturn;
	}

	template <class T>
	void Vector2D<T>::Move(const Vector2D<T>& p_toAdd)
	{
		__x += p_toAdd.X();
		__y += p_toAdd.Y();
	}

	template <class T>
	float Vector2D<T>::DotProduct(const Vector2D<T>& p_dotProductSecondOperand) const
	{
		return __x * p_dotProductSecondOperand.X() + __y * p_dotProductSecondOperand.Y();
	}

	template <class T>
	float Vector2D<T>::CrossProduct(const Vector2D<T>& p_crossProductSecondOperand) const
	{
		return __x * p_crossProductSecondOperand.Y() - __y * p_crossProductSecondOperand.X();
	}

	template <class T>
	float Vector2D<T>::DistanceTo(const Vector2D<T>& p_otherVector) const
	{
		return sqrt(pow((p_otherVector.X() - __x), 2) + pow((p_otherVector.Y() - __y), 2));
	}

	template <class T>
	float Vector2D<T>::AngleTo(const Vector2D<T>& p_otherVector) const
	{
		return atan2(p_otherVector.Y() - Y(), p_otherVector.X() - X()) * 180 / M_PI;
	}

	template <class T>
	float Vector2D<T>::GetMagnitude() const
	{
		return sqrt(pow(__x, 2) + pow(__y, 2));
	}

	template <class T>
	float Vector2D<T>::GetAngle() const
	{
		return atan2(__y, __x) * 180 / M_PI;
	}

	template <class T>
	void Vector2D<T>::Normalize()
	{
		T magnitude = GetMagnitude();
		__x /= magnitude;
		__y /= magnitude;
	}

	template <class T>
	bool Vector2D<T>::IsNormalized() const
	{
		return GetMagnitude() == 1;
	}

	template <class T>
	bool Vector2D<T>::IsNull() const
	{
		return __x == 0 && __y == 0;
	}

	template <class T>
	bool Vector2D<T>::IsOrthogonalTo(const Vector2D<T>& p_otherVector) const
	{
		return DotProduct(p_otherVector) == 0;
	}

	template <class T>
	bool Vector2D<T>::IsParallelTo(const Vector2D<T>& p_otherVector) const
	{
		const float angularDifference = abs(GetAngle() - p_otherVector.GetAngle());
		return angularDifference == 180 || angularDifference == 0;
	}

	template <class T>
	sf::Vector2<T> Vector2D<T>::ToSFVector() const
	{
		return sf::Vector2<T>(__x, __y);
	}
}

#endif // !__VECTOR2D__
