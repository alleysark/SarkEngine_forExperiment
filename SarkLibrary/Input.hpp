#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include "core.h"

namespace sarklib{

	// input static class management input system as general
	class Input{
	public:

		// ------------	keyboard properties	------------------
		class Key{
		public:
			typedef ubyte key_code_t;
			const key_code_t CODE_ESC = 0;

			typedef ubyte key_state_t;
			const key_state_t STATE_DOWN = 0;
			const key_state_t STATE_PRESSED = 0;
			const key_state_t STATE_UP = 0;

		public:
			key_code_t code;
			key_state_t state;

		public:
			Key() :code(0), state(0){}
			Key(const Key& key) : code(key.code), state(key.state){}
			Key(key_code_t _code, key_state_t _state) : code(_code), state(_state){}
		};


		// ------------	mouse properties	------------------
		class Mouse{
		public:
			typedef ubyte mouse_state_t;
			const mouse_state_t STATE_MOVEMENT = 0;

			const mouse_state_t STATE_LBUTTON_DOWN = 0;
			const mouse_state_t STATE_LBUTTON_UP = 0;

			const mouse_state_t STATE_MBUTTON_DOWN = 0;
			const mouse_state_t STATE_MBUTTON_UP = 0;

			const mouse_state_t STATE_RBUTTON_DOWN = 0;
			const mouse_state_t STATE_RBUTTON_UP = 0;

			const mouse_state_t STATE_WHEEL_DOWN = 0;
			const mouse_state_t STATE_WHEEL_UP = 0;

		public:
			Position2 position;
			mouse_state_t state;

		public:
			Mouse() :position(0), state(0){}
			Mouse(const Mouse& mouse) :position(mouse.position), state(mouse.state){}
			Mouse(const Position2& _position, mouse_state_t _state) :position(_position), state(_state){}
		};

	private:
		// it can't be instantiated
		Input(){}
		Input(const Input& input){}

	public:
		static Key lastKey;
		static Mouse lastMouse;

	};

}
#endif