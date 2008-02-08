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


Transaction::Transaction (Class *pClass) : ItemSet (), mTransactionID (GetSeqTransactionID ()), mpClass (pClass)
{

}

Transaction::~Transaction ()
{
//	LOGMSG (HIGH_LEVEL, "Transaction::~Transaction () - p [%p]\n", this);

	RemoveAll ();
}

const uint32 Transaction::GetSeqTransactionID ()
{
	uint32 transactionID = msSeqTransactionID++;

	return transactionID;
}

const uint32& Transaction::GetTransactionID () const
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
		const float32 &support, const uint32 &projection_size,
		const uint32 &min_rule_len, const uint32 &max_rule_len) const
{
	uint32 size = GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - items [%u]\n", size);

	PatternList*	pFrequentPatternList	= new PatternList ()	;
	ItemList*	pFrequentItemList	= new ItemList ()	;

	LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - add patterns size [1]\n");

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

	uint32 numFrequentItems		= pFrequentItemList->GetSize ();
	uint32 frequentItemBackID	= static_cast<Item *>(pFrequentItemList->GetBack ())->GetItemID ();

	uint32 iTryPattern = 0;

	while (iTryPattern < pFrequentPatternList->GetSize ())
	{
		uint32 pattern_size = static_cast<const Pattern *>(pFrequentPatternList->GetAt (iTryPattern))->GetSize ();

		if (pattern_size < max_rule_len)
		{
			LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - add patterns size [%u]\n", pattern_size+1);

			uint32 iNumTryPatterns = pFrequentPatternList->GetSize ();

			do
			{
				Pattern *pPattern = static_cast<Pattern *>(pFrequentPatternList->GetAt (iTryPattern));
				uint32 pattern_size = pPattern->GetSize ();

				uint32 patternItemBackID = static_cast<Item *>(pPattern->GetBack ())->GetItemID ();

				if (patternItemBackID < frequentItemBackID)
				{
					for (uint32 i = pattern_size; i < numFrequentItems; i++)
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
			} while (iTryPattern < iNumTryPatterns);
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
			LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - delete pattern [%s], frequence [%u]\n", static_cast<const Pattern*>(*it)->GetPrintableString ().c_str (), static_cast<const Pattern*>(*it)->GetFrequence ());
			delete *it;
			it = pFrequentPatternList->Erase (it);
			itPatternEnd = pFrequentPatternList->GetEnd ();
		}
		else
			break;
	}

	pFrequentItemList->RemoveAll ();
	delete pFrequentItemList;

	uint32 frequent_size = pFrequentPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - patterns [%u]\n", frequent_size);

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
	LOGMSG (LOW_LEVEL, "Transaction::Print () - mTransactionID [%u], class [%s], items [%s]\n",  mTransactionID, mpClass->GetValue ().c_str (), GetPrintableString ().c_str ());
}
