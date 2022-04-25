#pragma once
#include <memory>
namespace ButiEngine {

	class Timer :public IObject
	{
	public:
		Timer(float arg_maxCountFrame);
		virtual bool Update() = 0;
		bool IsOn();
		void Start();
		void Stop();
		void Initialize()override;
		void PreInitialize()override;
		void SetCount(std::int32_t arg_nowCount);
		void ChangeCountFrame(std::int32_t arg_maxCount);
		void Reset();
		inline float GetPercent() const {
			if (maxCountFrame == 0)return 0;
			return (float)nowCountFrame / (float)maxCountFrame;
		}
		inline float GetRemainFrame()const {
			return maxCountFrame - nowCountFrame;
		}
	protected:
		float maxCountFrame;
		float nowCountFrame;
		bool isOn;
	};

	class AbsoluteTimer :public Timer
	{
	public:
		AbsoluteTimer(float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
		inline bool Update() override {
			if (!isOn) {
				return false;
			}
			nowCountFrame++;
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = 0;
				return true;
			}
			return false;
		}
	};

	class RelativeTimer :public Timer
	{
	public:
		RelativeTimer() :Timer(0) {}
		RelativeTimer(float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
		inline bool Update()override {
			if (!isOn) {
				return false;
			}
			nowCountFrame += 1; //Game::GetInstance()->GetGameTime()->GetSpeed();
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = 0;
				return true;
			}
			return false;
		}

		Value_ptr<RelativeTimer> Clone() {
			auto cloned = ObjectFactory::Create<RelativeTimer>(maxCountFrame);

			cloned->isOn = isOn;

			return cloned;
		}

		void ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(nowCountFrame);
			archive(maxCountFrame);
			archive(isOn);
		}
	};

	template <class T>
	class TimeBomb :public Timer
	{

	public:
		using TsFunction = void (T::*)();
		TimeBomb(float arg_maxCountFrame, Value_ptr<T> arg_vlp_instance) :Timer(arg_maxCountFrame) {
			vlp_instance = arg_vlp_instance;
		}
		inline bool Update()override {
			if (!isOn) {
				return false;
			}
			nowCountFrame += 1; //Game::GetInstance()->GetGameTime()->GetSpeed();
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = 0;
				(*(vlp_instance.lock()).*bombFunc)();
				return true;
			}
			return false;
		}
		inline bool Update_UnExecute() {
			if (!isOn) {
				return false;
			}
			nowCountFrame += 1; //Game::GetInstance()->GetGameTime()->GetSpeed();
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = maxCountFrame;
				return false;
			}
			return false;
		}
		TsFunction bombFunc;

	private:
		Value_weak_ptr<T> vlp_instance;
	};
}

