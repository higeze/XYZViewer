#pragma once
#include "OGLChartWnd.h"
#include "OGLChart.h"
#include "3DLineChart.h"
#include "OGLAxis.h"

template<class _Elem,
	class _Traits,
	class _Alloc> inline
	std::basic_istream<_Elem, _Traits>& safegetline(
		std::basic_istream<_Elem, _Traits>& _Istr,
		std::basic_string<_Elem, _Traits, _Alloc>& _Str)
{
    if ( getline( _Istr, _Str ) ) {
       if ( _Str.size() && _Str[_Str.size()-1] == '\r' ) {
           _Str = _Str.substr( 0, _Str.size() - 1 );
       }
    }
    return _Istr;
}

class CXYZVWnd : public COGLChartWnd
{
private:
	typedef boost::escaped_list_separator<char> BOOST_ESCAPED_LIST_SEP;
	typedef boost::tokenizer< boost::escaped_list_separator<char> >BOOST_TOKENIZER_ESCAPED_LIST;
private:
	std::vector<CColor> m_colors;
public:
	CXYZVWnd():COGLChartWnd(std::make_shared<C3DLineChart>(std::make_shared<COGLChartProperty>()))
	{
		m_cwa
		.dwExStyle(WS_EX_ACCEPTFILES);
		AddMsgHandler(WM_DROPFILES,&CXYZVWnd::OnDropFiles,this);

		m_spChart->GetChartPtr()->GetXAxisPtr()->SetMaximum(10);
		m_spChart->GetChartPtr()->GetXAxisPtr()->SetMinimum(0);
		m_spChart->GetChartPtr()->GetXAxisPtr()->SetTick(1);

		m_spChart->GetChartPtr()->GetYAxisPtr()->SetMaximum(10);
		m_spChart->GetChartPtr()->GetYAxisPtr()->SetMinimum(0);
		m_spChart->GetChartPtr()->GetYAxisPtr()->SetTick(1);

		m_spChart->GetChartPtr()->GetZAxisPtr()->SetMaximum(10);
		m_spChart->GetChartPtr()->GetZAxisPtr()->SetMinimum(0);
		m_spChart->GetChartPtr()->GetZAxisPtr()->SetTick(1);

		m_colors.emplace_back(RGB(192, 57, 43));
		m_colors.emplace_back(RGB(230, 126, 34));
		m_colors.emplace_back(RGB(241, 196, 15));
		m_colors.emplace_back(RGB(26, 188, 156));
		m_colors.emplace_back(RGB(46, 204, 113));
		m_colors.emplace_back(RGB(52, 152, 219));
		m_colors.emplace_back(RGB(155, 89, 182));
		m_colors.emplace_back(RGB(52, 73, 94));



	}
	virtual ~CXYZVWnd(void){}

	LRESULT OnDropFiles(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

};

