#ifndef _TOOLKITDOC_H
class CToolkitDoc : public CDocument
{
	DECLARE_DYNCREATE(CToolkitDoc)
public:
	BOOL m_bLoaded;
protected:
	CToolkitDoc();
};
#endif
