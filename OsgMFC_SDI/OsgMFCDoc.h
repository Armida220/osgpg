// OsgMFCDoc.h : COsgMFCDoc ��Ľӿ�
//


#pragma once

class COsgMFCDoc : public CDocument
{
protected: // �������л�����
	COsgMFCDoc();
	DECLARE_DYNCREATE(COsgMFCDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~COsgMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


