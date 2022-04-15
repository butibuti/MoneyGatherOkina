#include<string>
#include"GameParts/GamePartsInterface.h"
namespace ButiEngine {
IApplication::ApplicationInitData InputApplicationInitData(const std::string& arg_filePath="ButiEngine.ini");
void OutputApplicationInitData(const IApplication::ApplicationInitData& arg_outputData, const std::string& arg_filePath = "ButiEngine.ini");

Value_ptr<IApplication> CreateApplicationInstance( const IApplication::ApplicationInitData& arg_appInitData);

}