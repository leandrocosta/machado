#include "Transaction.h"
#include "Class.h"
#include "ItemList.h"
#include "Pattern.h"
#include "PatternList.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint64 Transaction::msSeqTransactionID	= 0	;


Transaction::Transaction (Class *pClass) : ItemSet (), mTransactionID (GetSeqTransactionID ()), mpClass (pClass)
{

}

Transaction::~Transaction ()
{
	LOGMSG (HIGH_LEVEL, "Transaction::~Transaction () - p [%p]\n", this);

	RemoveAll ();
}

const uint64 Transaction::GetSeqTransactionID ()
{
	uint64 transactionID = msSeqTransactionID;
	msSeqTransactionID++;

	return transactionID;
}

const uint64& Transaction::GetTransactionID () const
{
	return mTransactionID;
}

Class* Transaction::GetClass ()
{
	return mpClass;
}

const string& Transaction::GetClassValue () const
{
	return mpClass->GetValue ();
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
			++itPattern;

		++itTransaction;
	}

	if (itPattern != itPatternEnd)
		bRet = false;

	return bRet;
}

PatternList* Transaction::GetFrequentPatternList (
		const float32 &support, const uint64 &projection_size,
		const uint32 &min_rule_len, const uint32 &max_rule_len) const
{
	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - items [%llu]\n", GetSize ());

	PatternList*	pFrequentPatternList	= new PatternList ()	;
	ItemList*	pFrequentItemList	= new ItemList ()	;

	LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - add single-item patterns\n");

	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
	{
		Item *pItem = static_cast<Item *>(*it);

		if ((float32) pItem->GetProjectionFrequence () / projection_size >= support)
		{
			Pattern *pPattern = new Pattern (pItem);
			pPattern->SetSupport ((float32) pPattern->GetFrequence () / projection_size);
			pFrequentPatternList->PushBack (pPattern);
			pFrequentItemList->PushBack (pItem);
		}
	}

	uint64 numFrequentItems		= pFrequentItemList->GetSize ();
	uint32 frequentItemBackID	= static_cast<Item *>(pFrequentItemList->GetBack ())->GetItemID ();

	uint64 iTryPattern = 0;

	LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - add combined-item patterns\n");

	while (iTryPattern < pFrequentPatternList->GetSize ())
	{
		Pattern *pPattern = static_cast<Pattern *>(pFrequentPatternList->GetAt (iTryPattern));

		uint64 pattern_size = pPattern->GetSize ();

		if (pattern_size < max_rule_len)
		{
			uint32 patternItemBackID = static_cast<Item *>(pPattern->GetBack ())->GetItemID ();

			if (patternItemBackID < frequentItemBackID)
			{
				for (uint64 i = pattern_size; i < numFrequentItems; i++)
				{
					Item *pItem = static_cast<Item *>(pFrequentItemList->GetAt (i));

					if (patternItemBackID < pItem->GetItemID ())
					{
						Pattern *pNewPattern = new Pattern (pPattern, pItem);

						if ((float32) pNewPattern->GetFrequence () / projection_size >= support)
						{
							pNewPattern->SetSupport ((float32) pNewPattern->GetFrequence () / projection_size);
							pFrequentPatternList->PushBack (pNewPattern);
						}
						else
							delete pNewPattern;
					}
				}
			}

			iTryPattern++;
		}
		else
			break;
	}

	LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - remove short patterns\n");

	PatternList::STLPatternList_it it		= pFrequentPatternList->GetBegin ()	;
	PatternList::STLPatternList_it itPatternEnd	= pFrequentPatternList->GetEnd ()	;

	while (it != itPatternEnd)
	{
		if (static_cast<const Pattern*>(*it)->GetSize () < min_rule_len)
		{
			LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - delete pattern [%s], frequence [%llu]\n", static_cast<const Pattern*>(*it)->GetPrintableString ().c_str (), static_cast<const Pattern*>(*it)->GetFrequence ());
			delete *it;
			it = pFrequentPatternList->Erase (it);
			itPatternEnd = pFrequentPatternList->GetEnd ();
		}
		else
			break;
	}

	pFrequentItemList->RemoveAll ();
	delete pFrequentItemList;

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - patterns [%llu]\n", pFrequentPatternList->GetSize ());

	return pFrequentPatternList;
}

void Transaction::AddTransactionToItemsProjectionTransactionLists ()
{
	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
		static_cast<Item *>(*it)->AddProjectTransaction (this);
}

void Transaction::Print () const
{
	LOGMSG (LOW_LEVEL, "Transaction::Print () - mTransactionID [%llu], class [%s], items [%s]\n",  mTransactionID, mpClass->GetValue ().c_str (), GetPrintableString ().c_str ());
}
