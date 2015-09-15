int _main_NOTUSED(int /*_argc*/, char** /*_argv*/)
{
	/*

#define  _1 SEMT::Expr<SEMT::Integer<1> >()
#define  _2 SEMT::Expr<SEMT::Integer<2> >()
#define  _3 SEMT::Expr<SEMT::Integer<3> >()
#define  _4 SEMT::Expr<SEMT::Integer<4> >()
#define  _9 SEMT::Expr<SEMT::Integer<9> >()
#define  _10 SEMT::Expr<SEMT::Integer<10> >()
#define  _32 SEMT::Expr<SEMT::Integer<12> >()

	auto Nc = _1 / _32 * (_1 + Xi) * (_1 + Eta) * (_9 *(Xi * Xi + Eta * Eta) - _10);
	auto Ns = _9 / _32 * (_1 + Xi) * (_1 - Eta * Eta) * (_1 + _9 * Eta);

	auto dNcdx = deriv_t(Nc, Xi);
	auto dNcdy = deriv_t(Nc, Eta);

	auto dNsdx = deriv_t(Ns, Xi);
	auto dNsdy = deriv_t(Ns, Eta);

	std::cout << dNcdx << std::endl << std::endl;
	std::cout << dNcdy << std::endl << std::endl;
	std::cout << dNsdx << std::endl << std::endl;
	std::cout << dNsdy << std::endl << std::endl;
	*/

	//SEMT::Array x_0(2);
	//x_0[0] = 0.1;
	//x_0[1] = 0.1;

	//std::cout << f << std::endl;
	//std::cout << SEMT::deriv_t(f, x) << std::endl;

    /*
	//typedef std::map<std::string, tfem::ElementPropertiesPtr> ElementLibrary;
	TiXmlNode* elementPropertiesElementList = xmldata::GetNodeByPath(hRoot.Node(), "ElementProperties");
	xmldata::foreachChild(elementPropertiesElementList->ToElement(), "elementType", [](TiXmlElement* nodeElement)
	{
		std::string id;
		std::string type = "Flat2DStructural";
		int nodesCount = 3;
		std::string gaussIntegration = "3x3";

		bool hasId = xmldata::ParseValue(nodeElement, "id", id);
		bool hasType = xmldata::ParseValue(nodeElement, "type", type);
		bool hasNodesCount = xmldata::ParseValue(nodeElement, "NodesCount", nodesCount);
		bool hasGaussIntegration = xmldata::ParseValue(nodeElement, "gaussIntegration", gaussIntegration);

		fem::GaussIntegration giType = fem::GI_3X3;
		if (gaussIntegration == "3x3")
		{
			giType = fem::GI_3X3;
		}
		else if (gaussIntegration == "4x4")
		{
			giType = fem::GI_4X4;
		}
		else if (gaussIntegration == "5x5")
		{
			giType = fem::GI_5X5;
		}
		else
		{
			LOGE("Wrong itegration type: %s", gaussIntegration.c_str());
		}
	});
	*/
	
	return 0;
}
