#ifndef __I_UNCOPIABLE_HPP__
#define __I_UNCOPIABLE_HPP__

namespace sark {

	class IUncopiable {
	public:
		IUncopiable() {}

	private:
		IUncopiable(const IUncopiable&) {}
		IUncopiable& operator=(const IUncopiable&) { return *this; }
	};

}
#endif