#include "Input.h"
#include <algorithm>

namespace sark{

	//=====================================================================
	//					Input::Keyboard implementation
	//=====================================================================

	Input::Keyboard::Keyboard(){
		for (uinteger i = 0; i < MAX_CODE_COUNT; i++)
			mCodes[i] = CODE_NONE;
		mCodeCount = 0;
	}

	void Input::Keyboard::RegisterKeyCombHandler(KeyCombHandler keyCombHandler,
		KeyCode code1, KeyCode code2, KeyCode code3, KeyCode code4){
		KeyCode codes[] = { code1, code2, code3, code4 };
		KeyComb comb = BuildKeyComb(codes);
		mCombHandlers[comb] = keyCombHandler;
	}

	Input::Keyboard::KeyComb Input::Keyboard::BuildKeyComb(KeyCode codes[MAX_CODE_COUNT]){
		//sort 4 element as order by desc
		//1. sort seperate two part
		if (codes[0] < codes[1]){
			std::swap(codes[0], codes[1]);
		}
		if (codes[2] < codes[3]){
			std::swap(codes[2], codes[3]);
		}
		//2. merge sort the two part
		if (codes[0] < codes[2]){
			std::swap(codes[0], codes[2]);
			std::swap(codes[1], codes[3]);
		}
		//3. sort the rest part
		if (codes[1] < codes[3]){
			KeyCode tmp = codes[1];
			codes[1] = codes[2];
			codes[2] = codes[3];
			codes[3] = tmp;
		}
		else if (codes[1] < codes[2]){
			std::swap(codes[1], codes[2]);
		}
		
		//generate key comb code
		KeyComb combCode = 0;
		integer keycodeBit = 8 * sizeof(KeyCode);
		for (uinteger i = 0; i < MAX_CODE_COUNT; i++){
			combCode = combCode << keycodeBit;
			combCode |= codes[i];
		}
		return combCode;
	}

	void Input::Keyboard::Down(KeyCode code){
		if (mCodeCount >= MAX_CODE_COUNT)
			return;

		// push input code into the mCodes only if code is not existed in mCodes.
		bool bExist = false;
		for (uinteger i = 0; i < mCodeCount; i++){
			if (mCodes[i] == code){
				bExist = true;
				break;
			}
		}
		if (!bExist)
			mCodes[mCodeCount++] = code;

		// if the conbination of mCodes is registered in mCombHandlers, then invoke handler
		KeyComb comb = BuildKeyComb(mCodes);
		KeyCombHandlerContainer::iterator finder = mCombHandlers.find(comb);
		if (finder != mCombHandlers.end())
			finder->second();
	}
	void Input::Keyboard::Up(KeyCode code){
		// pull input code from mCodes
		for (uinteger i = 0; i < mCodeCount; i++){
			if (mCodes[i] == code){
				if (mCodeCount == 1){
					mCodes[i] = CODE_NONE;
				}
				else{
					mCodes[i] = mCodes[mCodeCount - 1];
					mCodes[mCodeCount - 1] = CODE_NONE;
				}
				mCodeCount--;
				break;
			}
		}
	}



	//=====================================================================
	//					Input::Mouse implementation
	//=====================================================================

	Input::Mouse::Mouse()
		: mActiveStates(STATE_NONE)
	{}

	// register mouse event handler.
	// it overrides the handler for the same event
	void Input::Mouse::RegisterMouseHandler(MouseEvent eventCode, EventHandler handler){
		if (eventCode >= EVENT_COUNT)
			return;

		mEventHandlers[eventCode] = handler;
	}

	void Input::Mouse::MovementUpdate(uint32 x, uint32 y){
		if ((mActiveStates&STATE_LBUTTON) != 0){
			TriggerEvent(EVENT_LBUTTON_DRAG, Position2(x, y), 0);
		}
		else if ((mActiveStates&STATE_MBUTTON) != 0){
			TriggerEvent(EVENT_MBUTTON_DRAG, Position2(x, y), 0);
		}
		else if ((mActiveStates&STATE_RBUTTON) != 0){
			TriggerEvent(EVENT_RBUTTON_DRAG, Position2(x, y), 0);
		}
		else{
			TriggerEvent(EVENT_MOVE, Position2(x, y), 0);
		}
	}

	void Input::Mouse::LButtonUpdate(uint32 x, uint32 y){
		if ((mActiveStates&STATE_LBUTTON) == 0){// down
			mActiveStates |= STATE_LBUTTON;
		}
		else{// up
			mActiveStates ^= STATE_LBUTTON;
			TriggerEvent(EVENT_LBUTTON_CLICK, Position2(x, y), 0);
		}
	}

	void Input::Mouse::MButtonUpdate(uint32 x, uint32 y){
		if ((mActiveStates&STATE_MBUTTON) == 0){// down
			mActiveStates |= STATE_MBUTTON;
		}
		else{// up
			mActiveStates ^= STATE_MBUTTON;
			TriggerEvent(EVENT_MBUTTON_CLICK, Position2(x, y), 0);
		}
	}

	void Input::Mouse::RButtonUpdate(uint32 x, uint32 y){
		if ((mActiveStates&STATE_RBUTTON) == 0){// down
			mActiveStates |= STATE_RBUTTON;
		}
		else{// up
			mActiveStates ^= STATE_RBUTTON;
			TriggerEvent(EVENT_RBUTTON_CLICK, Position2(x, y), 0);
		}
	}

	void Input::Mouse::WheelUpdate(uint32 x, uint32 y, real delta){
		if ((mActiveStates&STATE_MBUTTON) != 0)
			return;
		TriggerEvent(EVENT_WHEEL, Position2(x, y), delta);
	}

	bool Input::Mouse::IsLButtonDown() const{
		return ((mActiveStates&STATE_LBUTTON) != 0);
	}
	bool Input::Mouse::IsMButtonDown() const{
		return ((mActiveStates&STATE_MBUTTON) != 0);
	}
	bool Input::Mouse::IsRButtonDown() const{
		return ((mActiveStates&STATE_RBUTTON) != 0);
	}


	Input::Keyboard Input::keyboard;
	Input::Mouse Input::mouse;
}