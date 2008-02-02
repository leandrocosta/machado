#include "Transaction.h"
#include "Class.h"
#include "ItemList.h"
#include "Pattern.h"
#include "PatternList.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint32 Transaction::msSeqTransactionID	= 0	;


Transaction::Transaction (const Class *pClass) : ItemSet (), mTransactionID (GetSeqTransactionID ()), mpClass (pClass)
{

}

Transaction::~Transaction ()
{
	LOGMSG (MEDIUM_LEVEL, "Transaction::~Transaction () - p [%p]\n", this);

	RemoveAll ();
}

const uint32 Transaction::GetSeqTransactionID ()
{
	uint32 transactionID = msSeqTransactionID;
	msSeqTransactionID++;

	return transactionID;
}

const uint32& Transaction::GetTransactionID () const
{
	return mTransactionID;
}

const Class* Transaction::GetClass () const
{
	return mpClass;
}

const bool Transaction::IsCoveredBy (const Pattern *pPattern) const
{
	bool bRet = true;

	ItemList::STLItemList_cit itTransaction		= GetBegin ();
	ItemList::STLItemList_cit itTransactionEnd	= GetEnd ();

	ItemList::STLItemList_cit itPattern		= pPattern->GetBegin ();
	ItemList::STLItemList_cit itPatternEnd		= pPattern->GetEnd ();

	while (itTransaction != itTransactionEnd && itPattern != itPatternEnd)
	{
		if (*itTransaction > *itPattern)
			break;

		if (*itTransaction == *itPattern)
			itPattern++;

		itTransaction++;
	}

	if (itPattern != itPatternEnd)
		bRet = false;

	return bRet;
}

PatternList* Transaction::GetFrequentPatternList (
		const float32 &support, const uint64 &projection_size,
		const uint32 &min_rule_len, const uint32 &max_rule_len) const
{
	LOGMSG (LOW_LEVEL, "PatternList::GetFrequentPatternList () - items [%llu]\n", GetSize ());

	PatternList *pFrequentPatternList = new PatternList ();

	LOGMSG (MEDIUM_LEVEL, "PatternList::GetFrequentPatternList () - add single-item patterns\n");

	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; it++)
	{
		Item *pItem = static_cast<Item *>(*it);

		if ((float32) pItem->GetFrequence () / projection_size >= support)
		{
			Pattern *pPattern = new Pattern (pItem);
			pPattern->SetSupport ((float32) pPattern->GetFrequence () / projection_size);
			pFrequentPatternList->PushBack (pPattern);
		}
	}

//	pFrequentPatternList->Sort ();

	uint32 numFrequentOneItemPatterns = pFrequentPatternList->GetSize ();
	uint32 iTryPattern = 0;

	LOGMSG (MEDIUM_LEVEL, "PatternList::GetFrequentPatternList () - add combined-item patterns\n");

	while (iTryPattern < pFrequentPatternList->GetSize ())
	{
		Pattern *pPattern = pFrequentPatternList->GetAt (iTryPattern);

		if (pPattern->GetSize () < max_rule_len)
		{
			Item *pItemBack = static_cast<Item *>(pPattern->GetBack ());

			for (uint32 i = 0; i < numFrequentOneItemPatterns; i++)
			{
				Item *pItem = static_cast<Item *>(static_cast<Pattern *>(pFrequentPatternList->GetAt (i))->GetAt (0));

				if (pItemBack->GetItemID () < pItem->GetItemID ())
				{
					Pattern *pNewPattern = new Pattern (pPattern);
					pNewPattern->AddItem (pItem);

					pNewPattern->SetSupport ((float32) pNewPattern->GetFrequence () / projection_size);

					if ((float32) pNewPattern->GetFrequence () / projection_size >= support)
					{
						LOGMSG (MEDIUM_LEVEL, "PatternList::GetFrequentPatternList () - add pattern [%s], frequence [%llu]\n", pNewPattern->GetPrintableString ().c_str (), pNewPattern->GetFrequence ());

						pFrequentPatternList->PushBack (pNewPattern);
					}
					else
					{
						LOGMSG (MEDIUM_LEVEL, "PatternList::GetFrequentPatternList () - doesn't add pattern [%s], frequence [%llu]\n", pNewPattern->GetPrintableString ().c_str (), pNewPattern->GetFrequence ());

						delete pNewPattern;
					}
				}
			}
		}

		iTryPattern++;
	}

	LOGMSG (MEDIUM_LEVEL, "PatternList::GetFrequentPatternList () - remove short patterns\n");

	PatternList::STLPatternList_it it		= pFrequentPatternList->GetBegin ()	;
	PatternList::STLPatternList_it itEndPattern	= pFrequentPatternList->GetEnd ()	;

	while (it != itEndPattern)
	{
		if (static_cast<const Pattern*>(*it)->GetSize () < min_rule_len)
		{
			LOGMSG (MEDIUM_LEVEL, "PatternList::GetFrequentPatternList () - delete pattern [%s], frequence [%llu]\n", static_cast<const Pattern*>(*it)->GetPrintableString ().c_str (), static_cast<const Pattern*>(*it)->GetFrequence ());
			delete *it;
			it = pFrequentPatternList->Erase (it);
			itEndPattern = pFrequentPatternList->GetEnd ();
		}
		else
			it++;
	}

	LOGMSG (LOW_LEVEL, "PatternList::GetFrequentPatternList () - return pFrequentPatternList [%p], patterns [%llu]\n", pFrequentPatternList, pFrequentPatternList->GetSize ());

	return pFrequentPatternList;
}

void Transaction::Print () const
{
	LOGMSG (LOW_LEVEL, "Transaction::Print () - mTransactionID [%u], class [%s], items [%s]\n",  mTransactionID, mpClass->GetValue ().c_str (), GetPrintableString ().c_str ());
}
