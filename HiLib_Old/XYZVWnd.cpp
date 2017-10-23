#include "XYZVWnd.h"
#include "OGLAxis.h"
#include "OGLSeries.h"
#include "3DLineChart.h"
#include "MyFile.h"

LRESULT CXYZVWnd::OnDropFiles(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	HDROP hDrop=(HDROP)wParam;
	UINT uiFileCount=::DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
	std::vector<std::wstring> vwstrPath;
	for(UINT i=0;i<uiFileCount;i++){
		std::wstring wstrFilePath;
		::DragQueryFile(hDrop,i,GetBuffer(wstrFilePath,MAX_PATH),MAX_PATH);
		ReleaseBuffer(wstrFilePath);
		vwstrPath.push_back(wstrFilePath);
	}
	::DragFinish(hDrop);

	//Load Files

	if(!vwstrPath.empty()){
		for each(auto& path in vwstrPath){

		std::string strLines=CFile::ReadAllString<char>(tstring_cast<TCHAR>(wstr2str(path).c_str(),path.c_str()));

		auto spSeries=std::make_shared<COGLSeries>(
			m_spChart->GetChartPtr().get(),
			std::make_shared<COGLSeriesProperty>(8, L"Arial", PS_SOLID, 2, m_colors[m_spChart->GetSize()%m_colors.size()]));

		boost::filesystem::path p(path);
		spSeries->SetName(p.filename().wstring());

		std::stringstream ss(strLines);
		std::string line;
		bool isFirst = true;
		int i = 0;
		BOOST_ESCAPED_LIST_SEP sep_tab('\\', '\t', boost::keep_empty_tokens);
		while(safegetline(ss, line)){
			BOOST_TOKENIZER_ESCAPED_LIST tokens1(line, sep_tab);
			auto iter = tokens1.begin();
			if(isFirst){
				m_spChart->GetChartPtr()->GetXAxisPtr()->SetName(*iter++); 
				m_spChart->GetChartPtr()->GetYAxisPtr()->SetName(*iter++); 
				m_spChart->GetChartPtr()->GetZAxisPtr()->SetName(*iter++); 	
				isFirst = false;
			}else{
				auto a=*iter++;
				auto b=*iter++;
				auto c=*iter++;

				spSeries->m_points.emplace_back(boost::lexical_cast<float>(a), boost::lexical_cast<float>(b), boost::lexical_cast<float>(c));
			}
			
		}
		m_spChart->AddSeries(spSeries);

		//for(int i=0;i<5;i++){
		//	for(int j=0;j<5;j++){
		//		auto k = i*j;
		//		CVertex vertex;
		//		try{
		//			vertex.SetVertex(boost::lexical_cast<float>(i),
		//							boost::lexical_cast<float>(j),
		//							boost::lexical_cast<float>(k));
		//		}catch(...){
		//		}
		//		spSeries->m_mVertex[i][j]=CVertex(vertex);
		//	}
		//}
		//spChart->AddSeries(spSeries);
		}
	}
	return 0;
}