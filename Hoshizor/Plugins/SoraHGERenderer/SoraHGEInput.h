#ifndef SORA_HGE_INPUT_H_
#define SORA_HGE_INPUT_H_

#include "SoraInput.h"
#include "SoraException.h"
#include "hge/hge.h"

namespace sora {

	class SoraHGEInput: public SoraInput {
	public:
		SoraHGEInput();
		~SoraHGEInput();
		 /*///////////////////////////////////////
		 // Mouse Events                         /
		 *////////////////////////////////////////
		void	 getMousePos(float *x, float *y);
		void	 setMousePos(float x, float y);
		float getMousePosX();
		float getMousePosY();
		int	 getMouseWheel();
		bool	 isMouseOver();

		 /*//////////////////////////////////////////
		 // Keyboard Events                         /
		 *///////////////////////////////////////////
		bool	keyDown(int32 key);
		bool	keyUp(int32 key);
		int32	getKeyState(int32 key);
		char*	getKeyName(int32 key);
		bool	getKeyEvent(SoraKeyEvent& ev);

		 /*//////////////////////////////////////////
		 // Joy Events                         /
		 *///////////////////////////////////////////
		bool	joyKeyPressed(int32 key);
		bool	joyKeyDown(int32 key);
		bool	joyKeyUp(int32 key);
		bool	joyKeyState(int32 key, unsigned char state);
		bool	setJoyKey(int32 key);
		bool	hasJoy();

		void	setWindowHandle(SoraHandle handle) {}

	private:
		SoraHGEInput(const SoraHGEInput&) {}

		HGE* hge;
	};

} // namespace sora

#endif