#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cstdlib>
#include<sstream> 
#include "File_Head.h"

using namespace std;
using namespace tinyxml2;

const string parentPath = "E:\\Public dataset\\CALTECH\\code3.2.1\\data-USA";

//把txt转化为xml文件;
int txt2xml(string txtName,string setName,string volName)
{
	//set00\\V000\\I00029.txt;
	//取得txtPath的最后10位作为文件名;
	//string txtName = txtPath.substr(txtPath.length()-10,6);
	txtName = txtName.substr(0,6);

	unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument());
	//XMLDocument* doc = new XMLDocument();
	//先添加xml头文件;

	//annotation
	XMLNode* annotation(doc->InsertEndChild( doc->NewElement( "annotation" ) ));
	//folder
	XMLNode* folder(annotation->InsertFirstChild(doc->NewElement( "folder" )));
	folder->InsertFirstChild(doc->NewText(volName.c_str()));
	//filename
	XMLNode* imageFile(annotation->InsertAfterChild(folder,doc->NewElement( "filename" )));
	imageFile->InsertFirstChild(doc->NewText((txtName+".jpg").c_str()));
	//size
	XMLNode* imgSize(annotation->InsertAfterChild(imageFile,doc->NewElement( "size" )));
	XMLNode* width(imgSize->InsertFirstChild(doc->NewElement( "width" )));
	width->InsertFirstChild(doc->NewText("640"));
	XMLNode* height(imgSize->InsertAfterChild(width,doc->NewElement( "height" )));
	height->InsertFirstChild(doc->NewText("480"));
	XMLNode* depth(imgSize->InsertAfterChild(height,doc->NewElement( "depth" )));
	depth->InsertFirstChild(doc->NewText("3"));
	
	//下面开始根据txt生成object了;
	ifstream fin((parentPath +"\\annotations\\"+ setName+"\\"+volName+"\\" + txtName+".txt").c_str() );
	string txtStr;
	//第一行跳过;

	getline(fin,txtStr);
	
	while (fin>>txtStr)//读入peopel或者person;
	{
		//先读取数据,判断这个boundingBox是否符合要求;width>15,height>20 且后面那一位不要为1
		int xminInt,yminInt,width,height,txtNum;
		fin>>xminInt;//xmin、ymin、width、height;
		fin>>yminInt;
		fin>>width;
		fin>>height;
		fin>>txtNum;
		if (width>=10&&height>=20&&txtNum==0)
		{

			//读入成功就新增一个object;
			XMLNode* object(annotation->InsertEndChild(doc->NewElement( "object" )));
			//新增name
			XMLNode* name(object->InsertEndChild(doc->NewElement( "name" )));
			name->InsertFirstChild(doc->NewText("Pedestrian"));
			//新增pos
			XMLNode* pose(object->InsertEndChild(doc->NewElement( "pose" )));
			pose->InsertFirstChild(doc->NewText("unspecified"));
			//新增truncated
			XMLNode* truncated(object->InsertEndChild(doc->NewElement( "truncated" )));
			truncated->InsertFirstChild(doc->NewText("0.00"));
			//新增difficult
			XMLNode* difficult(object->InsertEndChild(doc->NewElement( "difficult" )));
			difficult->InsertFirstChild(doc->NewText("0"));
			//新增bndbox
			XMLNode* bndbox(object->InsertEndChild(doc->NewElement( "bndbox" )));

			XMLNode* xmin(bndbox->InsertEndChild(doc->NewElement( "xmin" )));
			stringstream ss;ss<<xminInt;ss>>txtStr;ss.clear();
			xmin->InsertFirstChild(doc->NewText(txtStr.c_str()));
		
			XMLNode* ymin(bndbox->InsertEndChild(doc->NewElement( "ymin" )));
			ss<<yminInt;ss>>txtStr;ss.clear();
			ymin->InsertFirstChild(doc->NewText(txtStr.c_str()));
		
			XMLNode* xmax(bndbox->InsertEndChild(doc->NewElement( "xmax" )));
			ss<<width+xminInt;ss>>txtStr;ss.clear();
			xmax->InsertFirstChild(doc->NewText(txtStr.c_str()));
		
			XMLNode* ymax(bndbox->InsertEndChild(doc->NewElement( "ymax" )));
			ss<<height+yminInt;ss>>txtStr;ss.clear();
			ymax->InsertFirstChild(doc->NewText(txtStr.c_str()));

		}
		getline(fin,txtStr);//读取剩下没用的东西;

	}


	//doc->Print();
	doc->SaveFile( (parentPath +"\\xmlAnn\\"+ setName+"\\"+volName+"\\" + txtName+".xml").c_str() );//第二个参数如果为true,那么在文件中就看不出结构;

	
	return 0;
}

int main()
{
	//首先遍历annotations下面找到所有的set集合(生成对应的xml文件夹);
	//然后对每个set遍历找到所有的V文件夹,然后遍历每个文件夹里的txt文件生成xml文件;
	//txt2xml("E:\\Public dataset\\CALTECH\\code3.2.1\\data-USA\\annotations\\set00\\V000\\I00299.txt");
	//得到所有的set的名字;
	vector<string> setVec = getSubDir(parentPath+"\\annotations");
	for (string setName:setVec)
	{
		//先生成xml的set文件夹;
		createDir(parentPath+"\\xmlAnn\\"+setName);
		//在遍历得到这个文件夹下面的V文件夹;
		vector<string> volumVec = getSubDir(parentPath+"\\annotations\\"+setName);
		for (string volum:volumVec)
		{
			//生成对应的xml文件夹;
			createDir(parentPath+"\\xmlAnn\\"+setName+"\\"+volum);
			//下面遍历V文件夹下面的每个txt;
			vector<string> txtFileVec = getSubFile(parentPath+"\\annotations\\"+setName+"\\"+volum);
			for (string txtFileName:txtFileVec)
			{
				txt2xml(txtFileName,setName,volum);
			}
		}


	}
	return 0;
}