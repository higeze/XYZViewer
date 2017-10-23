#include "DcmParameter.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CDcmParameter);

std::string ParameterType2String(const Parameter::Type& type, bool germalType)
{
	if(germalType){
		switch(type)
		{
		case Parameter::Festwert:
			return "FESTWERT";
		case Parameter::FestwertBool:
			return "FESTWERT";
		case Parameter::FestwertEnum:
			return "FESTWERT";
		case Parameter::Festwerteblock:
			return "FESTWERTEBLOCK";
		case Parameter::Kennlinie:
			return "KENNLINIE";
		case Parameter::Kennfeld:
			return "KENNFELD";
		case Parameter::None:
			return "NONE";
		case Parameter::Undefined:
			return "UNDEFINED";
		default:
			return "NONE";
		}
	}else{
		switch(type)
		{
		case Parameter::Festwert:
			return "Scalar";
		case Parameter::FestwertBool:
			return "Bool";
		case Parameter::FestwertEnum:
			return "Enum";
		case Parameter::Festwerteblock:
			return "Array";
		case Parameter::Kennlinie:
			return "Curve";
		case Parameter::Kennfeld:
			return "Map";
		case Parameter::None:
			return "None";
		case Parameter::Undefined:
			return "Undefined";
		default:
			return "None";
		}
	}
}

std::ostream& operator <<(std::ostream& os, const std::shared_ptr<CDcmParameter>& spPara)
{
	spPara->Output(os);
	return os;
}

std::ostream& operator <<(std::ostream& os, CDcmParameter* pPara)
{
	pPara->Output(os);
	return os;
}

