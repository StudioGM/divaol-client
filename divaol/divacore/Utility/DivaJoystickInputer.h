/*
 *  DivaJoystickInputer.h
 *
 *  Created by Hyf042 on 1/18/13.
 *  Copyright 2013 Hyf042. All rights reserved.
 *
 *  Joystick Inputer for windows
 */

#ifndef DIVA_JOYSTICK_INPUTER
#define DIVA_JOYSTICK_INPUTER

#include "Core/DivaCommon.h"
#include "SoraSprite.h"
#include "Core/DivaCore.h"
#include "Lib/Base/Base.h"

#ifdef OS_WIN32
#include <dinput.h>
#include <windows.h>
#endif

namespace divacore
{
	class JoystickState
	{
	public:
		enum JoystickKey 
		{
			JOYSTICK_KEY_0,
			JOYSTICK_KEY_1,
			JOYSTICK_KEY_2,
			JOYSTICK_KEY_3,
			JOYSTICK_KEY_4,
			JOYSTICK_KEY_5,
			JOYSTICK_KEY_6,
			JOYSTICK_KEY_7,
			JOYSTICK_KEY_8,
			JOYSTICK_KEY_9,
			JOYSTICK_KEY_10,
			JOYSTICK_KEY_11,
			JOYSTICK_KEY_12,
			JOYSTICK_KEY_13,
			JOYSTICK_KEY_14,
			JOYSTICK_KEY_LEFT,
			JOYSTICK_KEY_RIGHT,
			JOYSTICK_KEY_UP,
			JOYSTICK_KEY_DOWN,
			JOYSTICK_KEY_AXIS
		};
		enum JoystickType
		{
			JOYSTICK_TYPE_PRESSED,
			JOYSTICK_TYPE_UP,
			JOYSTICK_TYPE_DOWN,
			JOYSTICK_TYPE_MOVE
		};
		static const int BUTTONS_NUM = 15;

	public:
		bool keyState[BUTTONS_NUM+4];

	public:
		JoystickState()
		{
			memset(keyState,0,sizeof(keyState));
		}
	};
#ifdef OS_WIN32
	class DxJoystickInputer
	{
	public:
		typedef Base::Function<void(int,int,int,int,int)> Listener;
	private:
		// total number of buttons in a joystick
		static const int PRESS_THREHOLD = 127;
		static const int AXIS_THREHOLD = 10;

		// directinput
		LPDIRECTINPUT8        mLpdi; 
		std::vector<LPDIRECTINPUTDEVICE8>  mDevices;
		std::vector<JoystickState> mStates;

		// devices
		std::vector<GUID> mGUID;     
		std::vector<Base::String> mJoystickName;

		std::vector<Listener> listeners;
	private:
		// DirectInput enum callback function
		static BOOL CALLBACK Inputer_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID inputer_ptr) 
		{
			DxJoystickInputer *inputer = (DxJoystickInputer*)inputer_ptr;
			inputer->mGUID.push_back(lpddi->guidInstance);
			inputer->mJoystickName.push_back(Base::String(lpddi->tszProductName).strip());

			return(DIENUM_CONTINUE);
		}
	public:
		DxJoystickInputer() {Clear();}
		virtual ~DxJoystickInputer() {Release();}

		void Clear()
		{
			mLpdi = NULL;
			mJoystickName.clear();
			mGUID.clear();
			mDevices.clear();
			mStates.clear();
		}

		// Initialize direct input
		bool Init()
		{
			Clear();
			if (DirectInput8Create((HINSTANCE)GetModuleHandle(0),DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&mLpdi,NULL) != DI_OK)
				return false;
			ResetDevices();
			LOGGER->log("Device %d", mDevices.size());
			for(int i = 0; i  < mDevices.size(); i++)
				LOGGER->log("%s", mJoystickName[i]);
			return true;
		}

		void Release()
		{
			if(mLpdi)
			{
				mLpdi->Release();
				for(int i = 0; i < mDevices.size(); i++)
					mDevices[i]->Release();
			}
			Clear();
		}

		void ResetDevices()
		{
			EnumDevices();
			CreateDevices();
		}

		
		void Poll()
		{
			DIJOYSTATE   joy_state; 
			JoystickState	tmpState;;

			for(int i = 0; i < mDevices.size(); i++) 
			{
				mDevices[i]->Poll();
				mDevices[i]->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joy_state);
				JoystickState &state = mStates[i];

				for(int j = 0; j < JoystickState::BUTTONS_NUM; j++)
					tmpState.keyState[j] = (joy_state.rgbButtons[j] > PRESS_THREHOLD);
				
				tmpState.keyState[JoystickState::JOYSTICK_KEY_LEFT] = joy_state.lX < -AXIS_THREHOLD;
				tmpState.keyState[JoystickState::JOYSTICK_KEY_RIGHT] = joy_state.lX > AXIS_THREHOLD;
				tmpState.keyState[JoystickState::JOYSTICK_KEY_UP] = joy_state.lY < -AXIS_THREHOLD;
				tmpState.keyState[JoystickState::JOYSTICK_KEY_DOWN] = joy_state.lY > AXIS_THREHOLD;

				for(int j = 0; j < JoystickState::BUTTONS_NUM+4; j++)
					if(tmpState.keyState[j])
					{
						publishEvent(j+JoystickState::JOYSTICK_KEY_0, JoystickState::JOYSTICK_TYPE_DOWN, i);
						if(!state.keyState[j]) {
							publishEvent(j+JoystickState::JOYSTICK_KEY_0, JoystickState::JOYSTICK_TYPE_PRESSED, i);
							state.keyState[j] = true;

						}
					}
					else
					{
						if(state.keyState[j]) {
							publishEvent(j+JoystickState::JOYSTICK_KEY_0, JoystickState::JOYSTICK_TYPE_UP, i);
							state.keyState[j] = false;
						}
					}

				if(joy_state.lX != 0 || joy_state.lY != 0)
					publishEvent(JoystickState::JOYSTICK_KEY_AXIS, JoystickState::JOYSTICK_TYPE_MOVE, i, joy_state.lX, joy_state.lY);
			}
		}

		void publishEvent(int key, int type, int deviceNum, int dx = 0, int dy = 0)
		{
			for(std::vector<Listener>::iterator ptr = listeners.begin(); ptr != listeners.end(); ptr++)
				(*ptr)(key, type, deviceNum, dx, dy);
		}

		DxJoystickInputer& operator+=(const Listener &listener)
		{
			listeners.push_back(listener);
			return *this;
		}

		DxJoystickInputer& operator-=(const Listener &listener)
		{
			for(std::vector<Listener>::iterator ptr = listeners.begin(); ptr != listeners.end(); ptr++)
				if(*ptr == listener)
				{
					listeners.erase(ptr);
					break;
				}
			return *this;
		}

	private:
		void EnumDevices()
		{
			mLpdi->EnumDevices(DI8DEVCLASS_GAMECTRL, 
                  &Inputer_Enum_Joysticks, 
                  (void*)this, 
                  DIEDFL_ATTACHEDONLY); 
		}
		void CreateDevices()
		{
			std::vector<Base::String> newDeviceName;
			for(int i = 0; i < mGUID.size(); i++)
			{
				LPDIRECTINPUTDEVICE8 lpdijoy;

				// create device
				if (mLpdi->CreateDevice(mGUID[i], &lpdijoy, NULL)!=DI_OK)
				   continue;

				// set cooperation level
				if (lpdijoy->SetCooperativeLevel((HWND)sora::SoraCore::Ptr->getMainWindowHandle(), 
									 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
				   continue;

				// set data format
				if (lpdijoy->SetDataFormat(&c_dfDIJoystick)!=DI_OK)
				   continue;

				// set the range of the joystick
				DIPROPRANGE joy_axis_range;

				// first x axis
				joy_axis_range.lMin = -24;
				joy_axis_range.lMax = 24;

				joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
				joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
				joy_axis_range.diph.dwObj        = DIJOFS_X;
				joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;

				lpdijoy->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

				// now y-axis
				joy_axis_range.lMin = -24;
				joy_axis_range.lMax = 24;

				joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
				joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
				joy_axis_range.diph.dwObj        = DIJOFS_Y;
				joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;

				lpdijoy->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

				// and now the dead band

				DIPROPDWORD dead_band; // here's our property word

				dead_band.diph.dwSize       = sizeof(dead_band);
				dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);
				dead_band.diph.dwObj        = DIJOFS_X;
				dead_band.diph.dwHow        = DIPH_BYOFFSET;

				// 10% will be used on both sides of the range +/-
				dead_band.dwData            = 1000;

				// finally set the property
				lpdijoy->SetProperty(DIPROP_DEADZONE,&dead_band.diph);

				dead_band.diph.dwSize       = sizeof(dead_band);
				dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);
				dead_band.diph.dwObj        = DIJOFS_Y;
				dead_band.diph.dwHow        = DIPH_BYOFFSET;

				// 10% will be used on both sides of the range +/-
				dead_band.dwData            = 1000;

				// finally set the property
				lpdijoy->SetProperty(DIPROP_DEADZONE,&dead_band.diph);


				// acquire the joystick
				if (lpdijoy->Acquire()!=DI_OK)
				   continue;

				mDevices.push_back(lpdijoy);
				mStates.push_back(JoystickState());
				newDeviceName.push_back(mJoystickName[i]);
			}
			mJoystickName = newDeviceName;
		}
	};
#endif
}

#endif