#include "CG.h"

namespace CG
{
	Poly::CSettings::CSettings()
		: mBounds(false)
	{}

	bool Poly::CSettings::IsBoundsEnabled(){
		return mBounds;
	}

	Poly::Settings& Poly::CSettings::SetBounds(bool enable = true){
		mBounds = enable;
		return *this;
	};

	Poly::Settings& Poly::GetEnviromentSettings(){
		static CSettings mEnviromentSettings;
		return mEnviromentSettings;
	}

	Poly::Poly() 
		: mColor(Color::white())
	{}

	void Poly::draw(){

		Settings &settings = GetEnviromentSettings();

		gl::ScopedColor scpColor(mColor);
		for (Line& line : mLines){
			gl::drawLine(line.v1, line.v2);
		}
		
		if (settings.IsBoundsEnabled()){

		}
	}
}