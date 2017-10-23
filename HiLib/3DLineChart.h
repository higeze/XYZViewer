#pragma once
#include "Chart.h"
#include "3DLineChartResource.h"

class COGLChartProperty;
class CChartLegend;
class COGLChart;
class COGLSeries;
class CStackPanel;
template<WORD TIBB> class CImage;


/**
 * C3DLineChart
 */

class C3DLineChart:public CPanel
{
protected:
	std::shared_ptr<COGLChartProperty> m_spChartProperty;
	std::shared_ptr<CDockPanel> m_spRightPane;
	std::shared_ptr<CImage<IDB_3DLINECHARTOPERATION>> m_spImage;
	std::shared_ptr<CChartLegend> m_spLegend;
	std::shared_ptr<COGLChart> m_spOGLChart;

	std::shared_ptr<std::vector<std::shared_ptr<COGLSeries>>> m_spvspSeries;

	boost::signals2::signal<void(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>&)> SeriesChanged;
public:
	/**
	 *  Constructor
	 */
	C3DLineChart(std::shared_ptr<COGLChartProperty>& spChartProperty);
	/**
	 *  Destructor
	 */
	virtual ~C3DLineChart(){}	
	/**
	 *  Getter and Setter
	 */
	std::shared_ptr<CDockPanel> GetRightPanePtr(){return m_spRightPane;}
	std::shared_ptr<COGLChart> GetChartPtr(){return m_spOGLChart;}
	std::shared_ptr<std::vector<std::shared_ptr<COGLSeries>>> GetSeriesPtr(){return m_spvspSeries;}
	void SetSeriesPtr(std::shared_ptr<std::vector<std::shared_ptr<COGLSeries>>> spvsp){m_spvspSeries=spvsp;}



	void OnPaint(PaintEventArgs& e);
	void OnLButtonDblClk(MouseEventArgs& e);
	void OnLButtonDown(MouseEventArgs& e);
	void OnLButtonUp(MouseEventArgs& e);
	void OnMButtonDown(MouseEventArgs& e);
	void OnMButtonUp(MouseEventArgs& e);
	void OnMouseMove(MouseEventArgs& e);
	void OnMouseWheel(MouseWheelEventArgs& e);

	void OnKeyDown(KeyEventArgs& e);
	void OnContextMenu(ContextMenuEventArgs& e);
	//void OnMouseLeave(MouseEventArgs& e);

	void AddSeries(std::shared_ptr<COGLSeries> spSeries);
	UINT GetSize()const { return m_spvspSeries->size(); }
	void ClearSeries();
	//void UpdateAxis();
};
