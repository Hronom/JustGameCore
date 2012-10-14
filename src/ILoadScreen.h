#ifndef ILOADSTATE_H
#define ILOADSTATE_H

namespace JGC
{
	class ILoadScreen
	{
	public:
		ILoadScreen() {};
		virtual ~ILoadScreen() {};

		virtual void show() = 0;
		virtual void hide() = 0;

		virtual void setProgress(int xProgressValue, std::string xText) = 0;
	};
}

#endif

