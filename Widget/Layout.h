#pragma once

#include <string>


namespace SEVIAN::WIDGET {
	class Widget;
	class Layout
	{

	public:
		virtual ~Layout () = default;

		virtual void arrange ( Widget* widget ) = 0;

		std::string test = "hello! from layout";
	};
}