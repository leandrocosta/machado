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
	mItemCoverageArray = NULL;
}

Transaction::~Transaction ()
{
	if (mItemCoverageArray)
		delete[] mItemCoverageArray;

	RemoveAll ();
}

const uint32 Transaction::GetSeqTransactionID ()
{
	uint32 transactionID = msSeqTransactionID++;

	return transactionID;
}

const uint32 Transaction::GetMaxTransactionID ()
{
	return msSeqTransactionID - 1;
}

const uint32& Transaction::GetTransactionID () const
{
	return mTransactionID;
}

Class* Transaction::GetClass ()
{
	return mpClass;
}

const uint32& Transaction::GetClassID () const
{
	return mpClass->GetClassID ();
}

const string& Transaction::GetClassValue () const
{
	return mpClass->GetValue ();
}

const bool Transaction::IsCoveredBy (const Pattern *pPattern) const
{
	bool bRet = true;

	ItemList::STLItemList_cit itEnd = pPattern->GetEnd ();

	for (ItemList::STLItemList_cit it = pPattern->GetBegin (); it != itEnd; ++it)
	{
		if (! mItemCoverageArray [static_cast<const Item *>(*it)->GetItemID ()])
		{
			bRet = false;
			break;
		}
	}

	return bRet;
}

const bool Transaction::IsCoveredBy (const Item *pItem) const
{
	return mItemCoverageArray [pItem->GetItemID ()];
}

const bool Transaction::IsCoveredByItem (const uint32 &rItemID) const
{
	return mItemCoverageArray [rItemID];
}

PatternList* Transaction::GetPatternList (
		const float32 &support, const uint32 &projection_size,
		const uint32 &min_rule_len, const uint32 &max_rule_len,
		const bool &rUseMaximalPatterns) const
{
	PatternList *pPatternList = NULL;

	if (rUseMaximalPatterns)
	{
		PatternList *pFrequentPatternList = GetFrequentPatternList (support, projection_size, Item::GetMaxItemID () + 1);
		pPatternList = GetMaximalFrequentPatternList (pFrequentPatternList);

		PatternList::STLPatternList_cit itEnd = pFrequentPatternList->GetEnd ();

		for (PatternList::STLPatternList_cit it = pFrequentPatternList->GetBegin (); it != itEnd; ++it)
		{
			Pattern *pPattern = static_cast<Pattern *>(*it);

			if (! pPatternList->FindByPtr (pPattern))
				delete pPattern;
		}

		pFrequentPatternList->RemoveAll ();
		delete pFrequentPatternList;
	}
	else
	{
		pPatternList = GetFrequentPatternList (support, projection_size, max_rule_len);

		LOGMSG (MEDIUM_LEVEL, "Transaction::GetPatternList () - remove short patterns\n");

		PatternList::STLPatternList_it it		= pPatternList->GetBegin ()	;
		PatternList::STLPatternList_it itPatternEnd	= pPatternList->GetEnd ()	;

		while (it != itPatternEnd)
		{
			if (static_cast<const Pattern*>(*it)->GetSize () < min_rule_len)
			{
				LOGMSG (MEDIUM_LEVEL, "Transaction::GetPatternList () - delete pattern [%s], frequence [%u]\n", static_cast<const Pattern*>(*it)->GetPrintableString ().c_str (), static_cast<const Pattern*>(*it)->GetFrequence ());
				delete *it;
				it = pPatternList->Erase (it);
				itPatternEnd = pPatternList->GetEnd ();
			}
			else
				break;
		}
	}

	Pattern::ResetSeqPatternID ();
	PatternList::STLPatternList_cit itEnd = pPatternList->GetEnd ();

	for (PatternList::STLPatternList_cit it = pPatternList->GetBegin (); it != itEnd; ++it)
		static_cast<Pattern *>(*it)->SetPatternID ();

	return pPatternList;
}

PatternList* Transaction::GetFrequentPatternList (
		const float32 &support, const uint32 &projection_size,
		const uint32 &rMaxPatternSize) const
{
	uint32 size = GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - items [%u], support [%f], projection_size [%u], rMaxPatternSize [%u]\n", size, support, projection_size, rMaxPatternSize);

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

			pPattern->SetPatternID ();
			pPattern->SetSupport ((float32) pPattern->GetFrequence () / projection_size);
			pFrequentPatternList->PushBack (pPattern);

			pFrequentItemList->PushBack (pItem);
		}
	}

	uint32 numFrequentItems = pFrequentItemList->GetSize ();

	if (numFrequentItems > 0)
	{
		uint32 frequentItemBackID = static_cast<Item *>(pFrequentItemList->GetBack ())->GetItemID ();

		uint32 iTryPattern = 0;

		while (iTryPattern < pFrequentPatternList->GetSize ())
		{
			uint32 pattern_size = static_cast<const Pattern *>(pFrequentPatternList->GetAt (iTryPattern))->GetSize ();

			if (pattern_size < rMaxPatternSize)
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
									pNewPattern->SetPatternID ();
									pNewPattern->SetSupport ((float32) pNewPattern->GetFrequence () / projection_size);
									pFrequentPatternList->PushBack (pNewPattern);
									pPattern->AddChildPattern (pNewPattern);
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
	}

	pFrequentItemList->RemoveAll ();
	delete pFrequentItemList;

	uint32 frequent_size = pFrequentPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - patterns [%u]\n", frequent_size);

	return pFrequentPatternList;
}

PatternList* Transaction::GetMaximalFrequentPatternList (const PatternList *pFrequentPatternList) const
{
	uint32 size = GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetMaximalFrequentPatternList () - items [%u]\n", size);

//	LOGMSG (HIGH_LEVEL, "DataBase::ClassifyTranscation () - frequent patterns:\n");
//	pFrequentPatternList->Print ();

	PatternList *pMaximalFrequentPatternList = NULL;

	PatternList::STLPatternList_cit it	= pFrequentPatternList->GetBegin ();
	PatternList::STLPatternList_cit itEnd	= pFrequentPatternList->GetEnd ();

	Pattern *pPattern = static_cast<Pattern *>(*(it++));

	pMaximalFrequentPatternList = pPattern->GetMaximalPatternList ();

	while (it != itEnd)
	{
		pPattern = static_cast<Pattern *>(*(it++));

		PatternList *pCandidatePatternList = pPattern->GetMaximalPatternList ();

		PatternList::STLPatternList_cit itCandEnd = pCandidatePatternList->GetEnd ();

		for (PatternList::STLPatternList_cit itCand = pCandidatePatternList->GetBegin (); itCand != itCandEnd; ++itCand)
		{
			/*
			 * Insert candidate into maximal pattern
			 * list if there is no super-pattern of
			 * candidate in the list yet.
			 */

			Pattern *pCandidatePattern = static_cast<Pattern *>(*itCand);

			if (! pMaximalFrequentPatternList->FindSuperPatternOf (pCandidatePattern))
				pMaximalFrequentPatternList->PushBack (pCandidatePattern);
		}

		pCandidatePatternList->RemoveAll ();
		delete pCandidatePatternList;

		if (pPattern->GetSize () > 1)
			break;
	}
	
	uint32 maximal_size = pMaximalFrequentPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetMaximalFrequentPatternList () - patterns [%u]\n", maximal_size);

	return pMaximalFrequentPatternList;
}

void Transaction::MakeItemCoverageArray (const uint32 &num_items)
{
//	LOGMSG (MAX_LEVEL, "Transaction::MakeItemCoverageArray () - transaction [%u], num_items [%u]\n", mTransactionID, num_items);

	mItemCoverageArray = new bool [num_items];

	uint32 i = 0;

	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
	{
		const Item *pItem = static_cast<const Item *>(*it);

//		LOGMSG (MAX_LEVEL, "Transaction::MakeItemCoverageArray () - item [%u]\n", pItem->GetItemID ());

		const uint32 &rItemID = pItem->GetItemID ();

		while (i < rItemID)
			mItemCoverageArray [i++] = false;

		mItemCoverageArray [i++] = true;
	}

	while (i < num_items)
		mItemCoverageArray [i++] = false;

//	LOGMSG (MAX_LEVEL, "Transaction::MakeItemCoverageArray () - end\n");
}

void Transaction::AddTransactionToItemsProjectionTransactionLists ()
{
	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
		static_cast<Item *>(*it)->AddProjectionTransaction (this);
}

void Transaction::Print () const
{
	LOGMSG (LOW_LEVEL, "Transaction::Print () - mTransactionID [%u], class [%s], items [%s]\n",  mTransactionID, mpClass->GetValue ().c_str (), GetPrintableString ().c_str ());
}
