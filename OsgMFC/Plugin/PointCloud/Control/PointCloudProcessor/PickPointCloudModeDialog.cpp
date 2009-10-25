// PickPointCloudModeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "PickPointCloudModeDialog.h"


// PickPointCloudModeDialog �Ի���

IMPLEMENT_DYNAMIC(PickPointCloudModeDialog, CPropertyPage)

PickPointCloudModeDialog::PickPointCloudModeDialog()
	: CPropertyPage(PickPointCloudModeDialog::IDD)
	, m_triangleArea(0)
{

}

PickPointCloudModeDialog::~PickPointCloudModeDialog()
{
}

void PickPointCloudModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TRIANGLE_AREA, m_triangleArea);
}


BEGIN_MESSAGE_MAP(PickPointCloudModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PICK_POINTCLOUD, &PickPointCloudModeDialog::OnBnClickedButtonPickPointcloud)
	ON_BN_CLICKED(IDC_BUTTON_TRIAREA, &PickPointCloudModeDialog::OnBnClickedButtonTriarea)
	ON_BN_CLICKED(IDC_BUTTON_GETSIGNEDPOINTS, &PickPointCloudModeDialog::OnBnClickedButtonGetsignedpoints)
END_MESSAGE_MAP()


// PickPointCloudModeDialog ��Ϣ�������

void PickPointCloudModeDialog::OnBnClickedButtonPickPointcloud()
{
	//AfxMessageBox("����ѡ�����ģʽ��");
	ctrl->SetMode(Controller::PICK_POINT_CLOUD);
}

void PickPointCloudModeDialog::OnBnClickedButtonTriarea()
{
	double area = ctrl->GetTriangleArea();
	if(area<0) {
		AfxMessageBox("����δѡ���κ���������");
	}
	m_triangleArea = area;
	UpdateData(FALSE);
}

void PickPointCloudModeDialog::OnBnClickedButtonGetsignedpoints()
{
	osg::ref_ptr<osg::Vec3Array> signedPoints = ctrl->GetSignedPoints();
	if(!signedPoints.valid()) {
		AfxMessageBox("��ǰ����û�б�ǵ����δѡ�е��ƣ�");
		return;
	}

	AfxMessageBox("��ѡ�񱣴�·��");

	CFileDialog dlg(TRUE, "ѡ���ļ�", NULL, 0, "All(*.*)|*.*|");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("δѡ�񱣴�·����");
		return;
	}
	CString name = dlg.GetPathName();

	setlocale(LC_ALL, "chs");
	std::ofstream out(string(name).c_str());

	for(unsigned int i=0; i<signedPoints->size(); ++i) {
		osg::Vec3& pt = signedPoints->at(i);
		out<<pt.x()<<" "<<pt.y()<<" "<<pt.z()<<endl;
	}

	out.close();

	AfxMessageBox("����ɹ���");
}
