#pragma once

#include"../Device/Input.h"
namespace ButiEngine {

	class GameDevice
	{
	public:
		~GameDevice();
		static void Initialize();
		static float WorldSpeed;
		static std::unique_ptr<Input>& GetInput();
		static void SetIsEnd(const bool arg_isEnd);
		static bool GetIsEnd();
		static void SetReturnCode(const std::int32_t arg_returnCode);
		static std::int32_t GetReturnCode();
	private:
		GameDevice();
		static std::unique_ptr<Input> input;
	};
}
