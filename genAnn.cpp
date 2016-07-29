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

//��txtת��Ϊxml�ļ�;
int txt2xml(string txtName,string setName,string volName)
{
	//set00\\V000\\I00029.txt;
	//ȡ��txtPath�����10λ��Ϊ�ļ���;
	//string txtName = txtPath.substr(txtPath.length()-10,6);
	txtName = txtName.substr(0,6);

	unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument());
	//XMLDocument* doc = new XMLDocument();
	//�����xmlͷ�ļ�;

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
	
	//���濪ʼ����txt����object��;
	ifstream fin((parentPath +"\\annotations\\"+ setName+"\\"+volName+"\\" + txtName+".txt").c_str() );
	string txtStr;
	//��һ������;

	getline(fin,txtStr);
	
	while (fin>>txtStr)//����peopel����person;
	{
		//�ȶ�ȡ����,�ж����boundingBox�Ƿ����Ҫ��;width>15,height>20 �Һ�����һλ��ҪΪ1
		int xminInt,yminInt,width,height,txtNum;
		fin>>xminInt;//xmin��ymin��width��height;
		fin>>yminInt;
		fin>>width;
		fin>>height;
		fin>>txtNum;
		if (width>=10&&height>=20&&txtNum==0)
		{

			//����ɹ�������һ��object;
			XMLNode* object(annotation->InsertEndChild(doc->NewElement( "object" )));
			//����name
			XMLNode* name(object->InsertEndChild(doc->NewElement( "name" )));
			name->InsertFirstChild(doc->NewText("Pedestrian"));
			//����pos
			XMLNode* pose(object->InsertEndChild(doc->NewElement( "pose" )));
			pose->InsertFirstChild(doc->NewText("unspecified"));
			//����truncated
			XMLNode* truncated(object->InsertEndChild(doc->NewElement( "truncated" )));
			truncated->InsertFirstChild(doc->NewText("0.00"));
			//����difficult
			XMLNode* difficult(object->InsertEndChild(doc->NewElement( "difficult" )));
			difficult->InsertFirstChild(doc->NewText("0"));
			//����bndbox
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
		getline(fin,txtStr);//��ȡʣ��û�õĶ���;

	}


	//doc->Print();
	doc->SaveFile( (parentPath +"\\xmlAnn\\"+ setName+"\\"+volName+"\\" + txtName+".xml").c_str() );//�ڶ����������Ϊtrue,��ô���ļ��оͿ������ṹ;

	
	return 0;
}

int main()
{
	//���ȱ���annotations�����ҵ����е�set����(���ɶ�Ӧ��xml�ļ���);
	//Ȼ���ÿ��set�����ҵ����е�V�ļ���,Ȼ�����ÿ���ļ������txt�ļ�����xml�ļ�;
	//txt2xml("E:\\Public dataset\\CALTECH\\code3.2.1\\data-USA\\annotations\\set00\\V000\\I00299.txt");
	//�õ����е�set������;
	vector<string> setVec = getSubDir(parentPath+"\\annotations");
	for (string setName:setVec)
	{
		//������xml��set�ļ���;
		createDir(parentPath+"\\xmlAnn\\"+setName);
		//�ڱ����õ�����ļ��������V�ļ���;
		vector<string> volumVec = getSubDir(parentPath+"\\annotations\\"+setName);
		for (string volum:volumVec)
		{
			//���ɶ�Ӧ��xml�ļ���;
			createDir(parentPath+"\\xmlAnn\\"+setName+"\\"+volum);
			//�������V�ļ��������ÿ��txt;
			vector<string> txtFileVec = getSubFile(parentPath+"\\annotations\\"+setName+"\\"+volum);
			for (string txtFileName:txtFileVec)
			{
				txt2xml(txtFileName,setName,volum);
			}
		}


	}
	return 0;
}