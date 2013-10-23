/** 
 * @file  CompareStats.cpp
 *
 * @brief Implementation of CompareStats class.
 */
// ID line follows -- this is updated by SVN
// $Id$

#include "StdAfx.h"
#include "DiffContext.h"
#include "CompareStats.h"

/** 
 * @brief Constructor, initializes critical section.
 */
CompareStats::CompareStats()
	: m_hEvent(CreateEvent(NULL, TRUE, FALSE, NULL))
{
	Reset();
}

/** 
 * @brief Destructor, deletes critical section.
 */
CompareStats::~CompareStats()
{
	CloseHandle(m_hEvent);
}

/** 
 * @brief Add compared item.
 * @param [in] code Resultcode to add.
 */
void CompareStats::AddItem(const DIFFITEM *di)
{
	RESULT res = GetColImage(di);
	InterlockedIncrement(&m_counts[res]);
	InterlockedIncrement(&m_nComparedItems);
	assert(m_nComparedItems <= m_nTotalItems);
	m_pCurDiffItem = di;
}

/** 
 * @brief Return count by resultcode.
 * @param [in] result Resultcode to return.
 * @return Count of items for given resultcode.
 */
int CompareStats::GetCount(CompareStats::RESULT result) const
{
	return m_counts[result];
}

String CompareStats::GetLeftPath(CDiffContext *ctxt) const
{
	return ctxt->GetLeftFilepathAndName(m_pCurDiffItem);
}

String CompareStats::GetRightPath(CDiffContext *ctxt) const
{
	return ctxt->GetRightFilepathAndName(m_pCurDiffItem);
}

/** 
 * @brief Reset comparestats.
 * Use this function to reset stats before new compare.
 */
void CompareStats::Reset()
{
	m_nTotalItems = 0;
	m_nComparedItems = 0;
	ZeroMemory(m_counts, sizeof m_counts);
	m_pCurDiffItem = NULL;
	Continue();
}

/**
 * @brief Return image index appropriate for this row
 */
CompareStats::RESULT CompareStats::GetColImage(const DIFFITEM *di)
{
	// Must return an image index into image list created above in OnInitDialog
	if (di->isResultError())
		return DIFFIMG_ERROR;
	if (di->isResultAbort())
		return DIFFIMG_ABORT;
	if (di->isResultFiltered())
		return di->isDirectory() ? DIFFIMG_DIRSKIP : DIFFIMG_SKIP;
	if (di->isSideLeftOnly())
		return di->isDirectory() ? DIFFIMG_LDIRUNIQUE : DIFFIMG_LUNIQUE;
	if (di->isSideRightOnly())
		return di->isDirectory() ? DIFFIMG_RDIRUNIQUE : DIFFIMG_RUNIQUE;
	if (di->isResultSame())
	{
		if (di->isDirectory())
			return DIFFIMG_DIRSAME;
		if (di->isText())
			return DIFFIMG_TEXTSAME;
		if (di->isBin())
			return DIFFIMG_BINSAME;
		return DIFFIMG_SAME;
	}
	// diff
	if (di->isResultDiff())
	{
		if (di->isDirectory())
			return DIFFIMG_DIRDIFF;
		if (di->isText())
			return DIFFIMG_TEXTDIFF;
		if (di->isBin())
			return DIFFIMG_BINDIFF;
		return DIFFIMG_DIFF;
	}
	return di->isDirectory() ? DIFFIMG_DIR : DIFFIMG_ABORT;
}
