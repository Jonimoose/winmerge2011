/**
 * @file  PropEditor.h
 *
 * @brief Declaration file for PropEditor propertyheet
 */

/**
 * @brief Property page for editor options.
 *
 * Editor options affect to editor behavior. For example syntax highlighting
 * and tabs.
 */
class PropEditor : public OptionsPanel
{
// Construction
public:
	PropEditor();

// Implement IOptionsPanel
	virtual void ReadOptions() override;
	virtual void WriteOptions() override;
	virtual void UpdateScreen() override;

// Dialog Data
	BOOL    m_bHiliteSyntax;
	int     m_nTabType;
	int     m_nTabSize;
	BOOL    m_bAutomaticRescan;
	BOOL    m_bAllowMixedEol;
	BOOL    m_bHonorModelines;
	BOOL    m_bHonorEditorConfig;
	BOOL    m_bViewLineDifferences;
	BOOL    m_bCharLevel;
	int     m_nBreakType;
	String m_breakChars;

// Implementation
protected:
	template<DDX_Operation>
			bool UpdateData();
	virtual BOOL OnInitDialog() override;
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);
};
