#pragma once
#include <stdexcept>

namespace script
{
	// Register is the basic storage type used by the interpreter
	// It can represent any deriving class or the implicit numerical values (i/u/d)
	class Register
	{
	public:
		virtual ~Register() = default;

		template<class T>
		auto cast() -> T*
		{
			T *other = dynamic_cast<T*>(this);
			
			if (!other)
				throw std::runtime_error("Register is of invalid type");
			
			return other;
		}

		template<class T>
		auto try_cast() -> T*
		{
			return dynamic_cast<T*>(this);
		}

		union
		{
			signed long i;
			unsigned long u;
			double d;
		} value;
	};
}

